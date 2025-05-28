/////////////////////////<Source Code Embedded Notices>/////////////////////////
//
// INTEL CONFIDENTIAL
// Copyright (C) Intel Corporation All Rights Reserved.
//
// The source code contained or described herein and all documents related to
// the source code ("Material") are owned by Intel Corporation or its suppliers
// or licensors. Title to the Material remains with Intel Corporation or its
// suppliers and licensors. The Material contains trade secrets and proprietary
// and confidential information of Intel or its suppliers and licensors. The
// Material is protected by worldwide copyright and trade secret laws and
// treaty provisions. No part of the Material may be used, copied, reproduced,
// modified, published, uploaded, posted, transmitted, distributed, or disclosed
// in any way without Intel's prior express written permission.
//
// No license under any patent, copyright, trade secret or other intellectual
// property right is granted to or conferred upon you by disclosure or delivery
// of the Materials, either expressly, by implication, inducement, estoppel or
// otherwise. Any license under such intellectual property rights must be
// express and approved by Intel in writing.
//
/////////////////////////<Source Code Embedded Notices>/////////////////////////

#include <ProbePlugin.h>
#include <BundleOperations.h>
#include <JtagStateBasedOperations.h>
#include <StateportOperations.h>
#include <TraceOperations.h>

#include <algorithm>
#include <array>
#include <memory>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string_view>
#include <variant>
#include <optional>
#include <string>
#include <cstring>
#include <numeric>
#include <atomic>
#include <unordered_map>

using namespace std::literals::string_view_literals;

namespace
{
    template<size_t N>
    void CopyStringToArray(char (&dest)[N], const std::string_view source)
    {
        dest[source.copy(dest, N - 1)] = '\0';
    }

    void CopyStringToArray(char* dest, size_t N, const std::string_view source)
    {
        dest[source.copy(dest, N - 1)] = '\0';
    }

    template<class T>
    struct unwrap_reference { using type = T; };
    template<class U>
    struct unwrap_reference<std::reference_wrapper<U>> { using type = U&; };
    template<typename T>
    using unwrap_reference_t = typename unwrap_reference<T>::type;

    // True if T matches U after removing references and pointers from T
    template<typename T, typename U>
    constexpr bool is_decay_equ = std::is_same_v<std::decay_t<T>, U> ||
                                  std::is_same_v<std::decay_t<unwrap_reference_t<T>>, U>  ||
                                  (std::is_reference_v<T> && is_decay_equ<std::remove_reference_t<T>, U>);

    static_assert(is_decay_equ<std::reference_wrapper<int>&, int>, "Example use of is_decay_equ");
}

class ConfigHolder
{
    std::unordered_map<std::string_view, std::string> _configEntries;
public:
    ConfigHolder() = default;

    ConfigHolder(std::initializer_list<std::pair<const std::string_view, std::string>> entries) :
        _configEntries(entries)
    {
    }

    std::vector<const char*> GetTypes() const
    {
        std::vector<const char*> keys(_configEntries.size());
        std::transform(_configEntries.begin(), _configEntries.end(), keys.begin(), [](auto const& pair)
                       {
                           return pair.first.data();
                       });
        return keys;
    }

    std::optional<std::string> TryGet(const std::string_view configType)
    {
        auto iter = _configEntries.find(configType);
        if (iter == _configEntries.end())
        {
            return std::nullopt;
        }
        return iter->second;
    }

    bool TrySet(const std::string_view configType, const std::string_view value)
    {
        auto iter = _configEntries.find(configType);
        if (iter == _configEntries.end())
        {
            return false;
        }
        iter->second = value;
        return true;
    }

};

class ReferencePluginLogger
{
    std::atomic<PluginStreamLogCallbackHandler> _logCallBackFunction { nullptr };
public:
    void SetLogHandler(PluginStreamLogCallbackHandler logCallBackFunction)
    {
        _logCallBackFunction = logCallBackFunction;
    }

    bool IsStreamUsed(PPI_Stream stream)
    {
        return stream == PPI_INTERNAL_STREAM;
    }

    void Log(OpenIPC_DeviceId deviceId, PPI_NotificationLevel level, const char* message)
    {
        auto callback = _logCallBackFunction.load();
        if (callback != nullptr)
        {
            callback(deviceId, PPI_INTERNAL_STREAM, level, message);
        }
    }

} PLUGIN_LOGGER;

// Simple class to simulate a shift register.
class ShiftRegister
{
public:
    explicit ShiftRegister(size_t size, std::vector<uint8_t> initialValue) :
        _size(size),
        _value(std::move(initialValue))
    {
        assert(size > 0);
    }

    std::vector<uint8_t> Shift(const std::vector<uint8_t>& input, size_t bitSize)
    {
        assert(input.size() == (bitSize + 7) / 8);
        std::vector<uint8_t> output;
        if (bitSize < 1)
        {
            return output;
        }
        if (bitSize == _size)
        {
            // Exact shift
            output = std::move(_value);
            _value = input;
            return output;
        }

        output.resize((bitSize + 7) / 8, 0);
        if (bitSize > _size)
        {
            // Over-shift

            const size_t bitCountOverBy = bitSize - _size;

            // output[0:_size] = _value[0:_size]
            std::copy_n(_value.begin(), (_size + 7) / 8, output.begin());
            // output[_size:bitsize] = input[0:bitsize - _size]
            for (size_t bitOffset = 0; bitOffset < bitCountOverBy; bitOffset++)
            {
                SetNthBit(output, _size + bitOffset, GetNthBit(input, bitOffset));
            }
            // _value[0:_size] = input[(bitsize - _size): bitsize]
            for (size_t bitOffset = 0; bitOffset < _size; bitOffset++)
            {
                SetNthBit(_value, bitOffset, GetNthBit(input, bitOffset + bitCountOverBy));
            }
        }
        else
        {
            // Under-shift
            const size_t bitCountUnderBy = _size - bitSize;
            // output[0:bitSize] = _value[0:bitSize]
            std::copy_n(_value.begin(), (bitSize + 7) / 8, output.begin());

            // _value[0:_size-bitSize] = _value[bitSize:_size]
            for (size_t bitOffset = 0; bitOffset < bitCountUnderBy; bitOffset++)
            {
                SetNthBit(_value, bitOffset, GetNthBit(_value, bitOffset + bitSize));
            }

            // _value[_size-bitSize:_size] = input[0:bitSize]
            for (size_t bitOffset = 0; bitOffset < bitSize; bitOffset++)
            {
                // Filling remaining value bits from input
                SetNthBit(_value, bitSize + bitOffset, GetNthBit(input, bitOffset));
            }
        }
        return output;
    }

    std::vector<uint8_t>& GetValue()
    {
        return _value;
    }

private:
    void SetNthBit(std::vector<uint8_t>& vec, size_t n, bool value)
    {
        if (value)
        {
            vec[n / 8] |= (1 << (n % 8)); // Set bit
        }
        else
        {
            vec[n / 8] &= ~(1 << (n % 8)); // Clear bit
        }
    }

    bool GetNthBit(const std::vector<uint8_t>& vec, size_t n)
    {
        return ((vec[n / 8] >> (n % 8)) & 1) == 1;
    }

    size_t _size;
    std::vector<uint8_t> _value;
};

class PluginHostMethods
{
    PluginEventCallbackHandler _eventHandlerFunction { nullptr };
    PluginNotificationCallbackHandler _notificationHandlerFunction { nullptr };
public:
    void SetEventHandler(PluginEventCallbackHandler eventHandlerFunction)
    {
        _eventHandlerFunction = eventHandlerFunction;
    }

    void SetNotificationHandler(PluginNotificationCallbackHandler notificationHandlerFunction)
    {
        _notificationHandlerFunction = notificationHandlerFunction;
    }

};
static PluginHostMethods PLUGIN_HOST_METHODS;

// ==== Operations/Bundles ====
namespace ReferenceBundleJtagOperations
{
    struct GoToState
    {
        JtagStateEncode GotoState;
        uint32_t NumberOfClocksInState;
        bool WaitForTrigger;
        bool ErrorOnTimeout;
    };
    struct IrScan
    {
        uint32_t BitCount;
        std::vector<uint8_t> InBits;
        uint8_t* OutBits;
    };
    struct DrScan
    {
        uint32_t BitCount;
        std::vector<uint8_t> InBits;
        uint8_t* OutBits;
    };
    using SomeOperation = std::variant<GoToState, IrScan, DrScan>;
}

class ReferenceJtagBundle
{
    std::vector<ReferenceBundleJtagOperations::SomeOperation> _operations;
public:
    ReferenceJtagBundle()  = default;
    ~ReferenceJtagBundle() = default;
    ReferenceJtagBundle(const ReferenceJtagBundle& other)     = delete;
    ReferenceJtagBundle(ReferenceJtagBundle&& other) noexcept = default;
    ReferenceJtagBundle& operator=(const ReferenceJtagBundle& other)     = delete;
    ReferenceJtagBundle& operator=(ReferenceJtagBundle&& other) noexcept = default;

    OpenIPC_Error AppendJtagGoToState(JtagStateEncode gotoState, uint32_t numberOfClocksInState, bool waitForTrigger, bool errorOnTimeout)
    {
        _operations.emplace_back(ReferenceBundleJtagOperations::GoToState { gotoState, numberOfClocksInState, waitForTrigger, errorOnTimeout });
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error AppendIrScan(uint32_t bitCount, std::vector<uint8_t>&& inBits, uint8_t* outBits)
    {
        _operations.emplace_back(ReferenceBundleJtagOperations::IrScan{ bitCount, std::move(inBits), outBits });
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error AppendDrScan(uint32_t bitCount, std::vector<uint8_t>&& inBits, uint8_t* outBits)
    {
        _operations.emplace_back(ReferenceBundleJtagOperations::DrScan{ bitCount, std::move(inBits), outBits });
        return OpenIPC_Error_No_Error;
    }

    std::vector<ReferenceBundleJtagOperations::SomeOperation>& GetOperations()
    {
        return _operations;
    }

};

const struct
{
    std::string Name = "fakestateport";
    std::string Description   = "An example stateport that can write and read one of four 32 bit values.";
    uint32_t AccessSizeInBits = 32;
    struct
    {
        std::string Name = "WhichOne";
        std::string Description = "Set to the index for the value to access. Valid values are 0 - 3.";
        uint32_t SizeInBits     = 2;
        std::optional<std::vector<uint8_t>> DefaultValue = std::nullopt;

        const PPI_InterfaceStatePortParameter* ToOpaqueHandle() const
        {
            return reinterpret_cast<const PPI_InterfaceStatePortParameter*>(this);
        }

        const auto* FromOpaqueHandle(const PPI_InterfaceStatePortParameter* handle) const
        {
            return reinterpret_cast<decltype(this)>(handle);
        }

    } Parameter0;

    std::vector<const PPI_InterfaceStatePortParameter*> GetParameters() const
    {
        return std::vector{ Parameter0.ToOpaqueHandle() };
    }

    const PPI_InterfaceStatePortDefinition* ToOpaqueHandle() const
    {
        return reinterpret_cast<const PPI_InterfaceStatePortDefinition*>(this);
    }

    const auto* FromOpaqueHandle(const PPI_InterfaceStatePortDefinition* handle) const
    {
        return reinterpret_cast<decltype(this)>(handle);
    }

} static ReferenceStatePortDefinition;

class ReferenceStatePortOperation
{
public:
    bool IsRead;
    std::optional<int> ValueIndex { std::nullopt };
    std::optional<uint32_t> WriteValue { std::nullopt };
    std::optional<std::pair<uint8_t*, uint32_t>> ReadBuffer { std::nullopt };
    ProbeStatePortError* ErrorBuffer { nullptr };

    explicit ReferenceStatePortOperation(bool isRead) :
        IsRead(isRead) {}

    OpenIPC_Error SetParameter(const decltype(ReferenceStatePortDefinition.Parameter0)* statePortParameter, const uint8_t* value, uint32_t size)
    {
        (void)statePortParameter; // we only have one parameter, so no need to check which one we are setting.
        if (size < 1)
        {
            return OpenIPC_Error_Invalid_Argument;
        }
        if (*value > 3)
        {
            return OpenIPC_Error_Invalid_Argument;
        }
        ValueIndex = std::make_optional<int>(static_cast<int>(*value));
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error SetWriteValue(const uint8_t* value, uint32_t size)
    {
        if (size < 4)
        {
            return OpenIPC_Error_Invalid_Argument;
        }
        uint32_t fullValue;
        std::memcpy(&fullValue, value, 4);
        WriteValue = std::make_optional<int>(fullValue);
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error SetReadBuffer(uint8_t* buffer, uint32_t size)
    {
        if (size != 4)
        {
            return OpenIPC_Error_Invalid_Argument;
        }
        ReadBuffer = std::make_optional<std::pair<uint8_t*, uint32_t>>(buffer, size);
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error SetErrorBuffer(ProbeStatePortError* errorBuffer)
    {
        ErrorBuffer = errorBuffer;
        return OpenIPC_Error_No_Error;
    }

    static ReferenceStatePortOperation* FromOpaqueHandle(PPI_InterfaceStatePortOperation* handle)
    {
        return reinterpret_cast<ReferenceStatePortOperation*>(handle);
    }

    static const ReferenceStatePortOperation* FromOpaqueHandle(const PPI_InterfaceStatePortOperation* handle)
    {
        return reinterpret_cast<const ReferenceStatePortOperation*>(handle);
    }

};

class ReferenceStatePortBundle
{
    std::vector<std::unique_ptr<ReferenceStatePortOperation>> _operations;
public:
    OpenIPC_Error AppendOperation(std::unique_ptr<ReferenceStatePortOperation> statePortOperation)
    {
        _operations.push_back(std::move(statePortOperation));
        return OpenIPC_Error_No_Error;
    }

    std::vector<std::unique_ptr<ReferenceStatePortOperation>>& GetOperations() { return _operations; }
};

using ReferenceBundle = std::variant<std::monostate, ReferenceJtagBundle, ReferenceStatePortBundle>;
ReferenceBundle* RetrieveBundle(PPI_ProbeBundleHandle handle)
{
    return static_cast<ReferenceBundle*>(handle);
}

// ==== Interfaces ====

class ReferenceJtagInterface
{
    bool _isInitializing { false };
    bool _isInitialized { false };
    JtagStateEncode _currentState { JtagRTI };

    std::vector<uint8_t> _idcodeIrValue { 0x2 };
    ShiftRegister _irRegister { 8, { 0x2 } };
    ShiftRegister _idcodeRegister { 32, { 0x79, 0x56, 0x34, 0x12 } }; // idcode = 0x12345679
    ShiftRegister _bypassRegister { 1, { 0 } };

public:
    ConfigHolder Configs;
    PPI_RefId InterfaceRefId;
    OpenIPC_DeviceId InterfaceDeviceId { OpenIPC_INVALID_DEVICE_ID };

    explicit ReferenceJtagInterface(PPI_RefId interfaceRefId) :
        InterfaceRefId(interfaceRefId)
    {

    }

    ~ReferenceJtagInterface() = default;
    ReferenceJtagInterface(const ReferenceJtagInterface& other)     = delete;
    ReferenceJtagInterface(ReferenceJtagInterface&& other) noexcept = default;
    ReferenceJtagInterface& operator=(const ReferenceJtagInterface& other)     = delete;
    ReferenceJtagInterface& operator=(ReferenceJtagInterface&& other) noexcept = default;

    OpenIPC_Error BeginInitialization(OpenIPC_DeviceId interfaceDeviceId) noexcept
    {
        if (_isInitializing)
        {
            return OpenIPC_Error_Already_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        InterfaceDeviceId = interfaceDeviceId;
        _isInitializing   = true;
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error FinishInitialization() noexcept
    {
        if (!_isInitializing)
        {
            return OpenIPC_Error_Not_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        _isInitializing = false;
        _isInitialized  = true;
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error DeInitialization() noexcept
    {
        if (!_isInitializing)
        {
            return OpenIPC_Error_Not_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        _isInitializing   = false;
        _isInitialized    = false;
        InterfaceDeviceId = OpenIPC_INVALID_DEVICE_ID;
        return OpenIPC_Error_No_Error;
    }

    PPI_EInterfaceType GetInterfaceType() const noexcept
    {
        return PPI_interfaceTypeJtag;
    }

    PPI_InterfaceJTAGCapabilities GetJtagCapabilities() noexcept
    {
        PPI_InterfaceJTAGCapabilities capabilities{};
        // Does the interface support using the TRST pin?
        capabilities.supportTRST = false;
        // Does the interface support cycling to TLR for a TAP reset. Note: supportTRST | supportTLR == 1
        capabilities.supportTLR = true;
        // Does the interface support staying in PauDR in a register interface
        capabilities.supportPauDRInRegisterInterface = false;

        return capabilities;
    }

    OpenIPC_Error ExecuteBundle(ReferenceJtagBundle& bundle)
    {
        PLUGIN_LOGGER.Log(InterfaceDeviceId, PPI_traceNotification, "Enter ReferenceJtagInterface.ExecuteBundle");
        OpenIPC_Error error = OpenIPC_Error_No_Error;
        for (auto& operation : bundle.GetOperations())
        {
            error = std::visit([&](auto& op)
                               {
                                   return ExecuteOperation(op);
                               }, operation);
        }
        return error;
    }

private:
    OpenIPC_Error ExecuteOperation(const ReferenceBundleJtagOperations::GoToState& op)
    {
        _currentState = op.GotoState;
        if (_currentState == JtagTLR)
        {
            _irRegister.Shift(_idcodeIrValue, 8);
        }
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error ExecuteOperation(const ReferenceBundleJtagOperations::IrScan& op)
    {
        _currentState = JtagShfIR;
        auto output = _irRegister.Shift(op.InBits, op.BitCount);
        assert((op.BitCount + 7) / 8 == output.size());
        if (op.OutBits)
        {
            std::copy_n(output.begin(), (op.BitCount + 7) / 8, op.OutBits);
        }
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error ExecuteOperation(const ReferenceBundleJtagOperations::DrScan& op)
    {
        _currentState = JtagShfDR;
        const auto& irRegisterValue = _irRegister.GetValue();
        if (std::equal(irRegisterValue.begin(), irRegisterValue.end(), _idcodeIrValue.begin(), _idcodeIrValue.end()))
        {
            auto idcodeRegisterCopy = _idcodeRegister; // copying so we don't write to the readonly register
            auto output = idcodeRegisterCopy.Shift(op.InBits, op.BitCount);
            assert((op.BitCount + 7) / 8 == output.size());
            if (op.OutBits)
            {
                std::copy_n(output.begin(), (op.BitCount + 7) / 8, op.OutBits);
            }
        }
        else
        {
            auto output = _bypassRegister.Shift(op.InBits, op.BitCount);
            assert((op.BitCount + 7) / 8 == output.size());
            if (op.OutBits)
            {
                std::copy_n(output.begin(), (op.BitCount + 7) / 8, op.OutBits);
            }
        }
        return OpenIPC_Error_No_Error;
    }

};

class ReferenceStatePortInterface
{
    bool _isInitializing { false };
    bool _isInitialized { false };

    std::array<uint32_t, 4> _valueStore {};
public:
    ConfigHolder Configs;
    PPI_RefId InterfaceRefId;
    OpenIPC_DeviceId InterfaceDeviceId { OpenIPC_INVALID_DEVICE_ID };

    explicit ReferenceStatePortInterface(PPI_RefId interfaceRefId) :
        InterfaceRefId(interfaceRefId)
    {

    }

    ~ReferenceStatePortInterface() = default;
    ReferenceStatePortInterface(const ReferenceStatePortInterface& other)     = delete;
    ReferenceStatePortInterface(ReferenceStatePortInterface&& other) noexcept = default;
    ReferenceStatePortInterface& operator=(const ReferenceStatePortInterface& other)     = delete;
    ReferenceStatePortInterface& operator=(ReferenceStatePortInterface&& other) noexcept = default;

    OpenIPC_Error BeginInitialization(OpenIPC_DeviceId interfaceDeviceId) noexcept
    {
        if (_isInitializing)
        {
            return OpenIPC_Error_Already_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        InterfaceDeviceId = interfaceDeviceId;
        _isInitializing   = true;
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error FinishInitialization() noexcept
    {
        if (!_isInitializing)
        {
            return OpenIPC_Error_Not_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        _isInitializing = false;
        _isInitialized  = true;
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error DeInitialization() noexcept
    {
        if (!_isInitializing)
        {
            return OpenIPC_Error_Not_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        _isInitializing   = false;
        _isInitialized    = false;
        InterfaceDeviceId = OpenIPC_INVALID_DEVICE_ID;
        return OpenIPC_Error_No_Error;
    }

    PPI_EInterfaceType GetInterfaceType() const noexcept
    {
        return PPI_interfaceTypeStatePort;
    }

    OpenIPC_Error ExecuteBundle(ReferenceStatePortBundle& bundle)
    {
        PLUGIN_LOGGER.Log(InterfaceDeviceId, PPI_traceNotification, "Enter ReferenceStatePortInterface.ExecuteBundle");
        OpenIPC_Error error = OpenIPC_Error_No_Error;

        for (const auto& operation : bundle.GetOperations())
        {
            if (operation->IsRead
                && operation->ValueIndex.has_value()
                && operation->ReadBuffer.has_value()
                && operation->ErrorBuffer != nullptr)
            {
                assert(operation->ValueIndex.value() < 4);
                assert(operation->ReadBuffer.value().second == 4);
                std::memcpy(operation->ReadBuffer.value().first, &_valueStore[operation->ValueIndex.value()], 4);
                *operation->ErrorBuffer = PROBE_STATE_PORT_ERROR_OK;
            }
            else if (!operation->IsRead
                     && operation->ValueIndex.has_value()
                     && operation->WriteValue.has_value()
                     && operation->ErrorBuffer != nullptr)
            {
                assert(operation->ValueIndex.value() < 4);
                _valueStore[operation->ValueIndex.value()] = operation->WriteValue.value();
                *operation->ErrorBuffer = PROBE_STATE_PORT_ERROR_OK;
            }
            else if (operation->ErrorBuffer != nullptr)
            {
                *operation->ErrorBuffer = OpenIPC_Error_Invalid_Argument;
                error = OpenIPC_Error_Invalid_Argument;
            }
            else
            {
                error = OpenIPC_Error_Invalid_Argument;
            }
            if (error != OpenIPC_Error_No_Error)
            {
                break;
            }
        }
        return error;
    }

    std::vector<const PPI_InterfaceStatePortDefinition*> GetDefinitions() const
    {
        return std::vector{ ReferenceStatePortDefinition.ToOpaqueHandle() };
    }

};

class ReferenceTraceInterface
{
    bool _isInitializing{ false };
    bool _isInitialized{ false };

    bool _isOpen{ false };
    bool _canRead{ false };
    bool _canWrite{ false };
public:
    // By default trace interfaces get their name from the type, and they are not given instanceIds.
    // Since names have to be unique across all devices, and we want multiple of these allowed in our
    // reference example, we are setting the name of the interface with a config. And the trailing
    // # character indicated an index should be generated to give each instance a unique number.
    ConfigHolder Configs { {"Name"sv, "Trace#"} };
    PPI_RefId InterfaceRefId;
    OpenIPC_DeviceId InterfaceDeviceId{ OpenIPC_INVALID_DEVICE_ID };

    explicit ReferenceTraceInterface(PPI_RefId interfaceRefId) :
        InterfaceRefId(interfaceRefId)
    {

    }

    ~ReferenceTraceInterface() = default;
    ReferenceTraceInterface(const ReferenceTraceInterface& other)     = delete;
    ReferenceTraceInterface(ReferenceTraceInterface&& other) noexcept = default;
    ReferenceTraceInterface& operator=(const ReferenceTraceInterface& other)     = delete;
    ReferenceTraceInterface& operator=(ReferenceTraceInterface&& other) noexcept = default;

    OpenIPC_Error BeginInitialization(OpenIPC_DeviceId interfaceDeviceId) noexcept
    {
        if (_isInitializing)
        {
            return OpenIPC_Error_Already_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        InterfaceDeviceId = interfaceDeviceId;
        _isInitializing   = true;
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error FinishInitialization() noexcept
    {
        if (!_isInitializing)
        {
            return OpenIPC_Error_Not_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        _isInitializing = false;
        _isInitialized  = true;
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error DeInitialization() noexcept
    {
        if (!_isInitializing)
        {
            return OpenIPC_Error_Not_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        _isInitializing   = false;
        _isInitialized    = false;
        InterfaceDeviceId = OpenIPC_INVALID_DEVICE_ID;
        return OpenIPC_Error_No_Error;
    }

    PPI_EInterfaceType GetInterfaceType() const noexcept
    {
        return PPI_interfaceTypeHtiTrace;
    }

    OpenIPC_Error OpenWindow(PPI_Trace_PortAccessMode accessMode)
    {
        if (_isOpen)
        {
            return OpenIPC_Error_InterfacePort_Window_Already_Open;
        }
        _isOpen   = true;
        _canRead  = accessMode == PPI_Trace_PortAccessRead || accessMode == PPI_Trace_PortAccessReadWrite;
        _canWrite = accessMode == PPI_Trace_PortAccessWrite || accessMode == PPI_Trace_PortAccessReadWrite;
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error CloseWindow()
    {
        if (!_isOpen)
        {
            return OpenIPC_Error_InterfacePort_Window_Not_Open;
        }
        _isOpen = false;
        return OpenIPC_Error_No_Error;
    }

    bool IsReadDataAvailable() const
    {
        return _isOpen && _canRead;
    }

    bool IsWindowOpen() const
    {
        return _isOpen;
    }

    OpenIPC_Error Read(uint8_t* output, uint32_t outputSize, uint32_t& actualBytesRead) const
    {
        if (!_isOpen)
        {
            return OpenIPC_Error_InterfacePort_Window_Not_Open;
        }
        if (!_canRead)
        {
            return OpenIPC_Error_InterfacePort_Operation_Invalid;
        }
        std::iota(output, output + outputSize, static_cast<uint8_t>(0));
        actualBytesRead = outputSize;
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error Write(const uint8_t* input, uint32_t inputSize, uint32_t& actualBytesWritten) const
    {
        if (!_isOpen)
        {
            return OpenIPC_Error_InterfacePort_Window_Not_Open;
        }
        if (!_canWrite)
        {
            return OpenIPC_Error_InterfacePort_Operation_Invalid;
        }
        // This implementation isn't actually writing the data anywhere, we just say we did.
        actualBytesWritten = inputSize;
        (void) input;
        return OpenIPC_Error_No_Error;
    }

};

using ReferenceInterface    = std::variant<ReferenceJtagInterface, ReferenceStatePortInterface, ReferenceTraceInterface>;
using ReferenceInterfaceRef = std::variant<std::reference_wrapper<ReferenceJtagInterface>, std::reference_wrapper<ReferenceStatePortInterface>, std::reference_wrapper<ReferenceTraceInterface>>;
using MaybeReferenceInterfaceRef = std::variant<std::monostate, std::reference_wrapper<ReferenceJtagInterface>, std::reference_wrapper<ReferenceStatePortInterface>, std::reference_wrapper<ReferenceTraceInterface>>;

// ==== Probe ====
class ReferenceProbe
{
    bool _isInitializing { false };
    bool _isInitialized { false };
    ReferenceJtagInterface _jtagInterface;
    ReferenceStatePortInterface _statePortInterface;
    ReferenceTraceInterface _traceInterface;
public:
    ConfigHolder Configs { { "RuntimeSetting"sv, "Default" } };
    static const PPI_char* const PROBE_TYPE;
    PPI_RefId ProbeRefId;
    OpenIPC_DeviceId ProbeDeviceId { OpenIPC_INVALID_DEVICE_ID };

    explicit ReferenceProbe(PPI_RefId probeRefId) :
        _jtagInterface(42),
        _statePortInterface(43),
        _traceInterface(44),
        ProbeRefId(probeRefId)
    {

    }

    ~ReferenceProbe() = default;
    ReferenceProbe(const ReferenceProbe& other)     = delete;
    ReferenceProbe(ReferenceProbe&& other) noexcept = default;
    ReferenceProbe& operator=(const ReferenceProbe& other)     = delete;
    ReferenceProbe& operator=(ReferenceProbe&& other) noexcept = default;

    OpenIPC_Error BeginInitialization(OpenIPC_DeviceId probeDeviceId) noexcept
    {
        if (_isInitializing)
        {
            return OpenIPC_Error_Already_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        ProbeDeviceId   = probeDeviceId;
        _isInitializing = true;
        return OpenIPC_Error_No_Error;
    }

    OpenIPC_Error FinishInitialization() noexcept
    {
        if (!_isInitializing)
        {
            return OpenIPC_Error_Not_Initializing;
        }
        if (_isInitialized)
        {
            return OpenIPC_Error_Already_Initialized;
        }
        _isInitializing = false;
        _isInitialized  = true;
        return OpenIPC_Error_No_Error;
    }

    PPI_ProbeInfo GetProbeInfo() noexcept
    {
        PPI_ProbeInfo info{};
        CopyStringToArray(info.type,             ReferenceProbe::PROBE_TYPE);
        CopyStringToArray(info.uniqueIdentifier, "example.0");
        CopyStringToArray(info.probeInfo,        "Reference version 1.0");
        return info;
    }

    std::vector<PPI_RefId> InterfaceGetRefIds() const noexcept
    {
        std::vector<PPI_RefId> interfaceRefIds {
            _jtagInterface.InterfaceRefId,
            _statePortInterface.InterfaceRefId,
            _traceInterface.InterfaceRefId
        };
        return interfaceRefIds;
    }

    MaybeReferenceInterfaceRef GetInterfaceByRefId(PPI_RefId interfaceRefId)
    {
        if (_jtagInterface.InterfaceRefId == interfaceRefId)
        {
            return std::ref(_jtagInterface);
        }
        if (_statePortInterface.InterfaceRefId == interfaceRefId)
        {
            return std::ref(_statePortInterface);
        }
        if (_traceInterface.InterfaceRefId == interfaceRefId)
        {
            return std::ref(_traceInterface);
        }
        return std::monostate();
    }

    MaybeReferenceInterfaceRef GetInterfaceByDeviceId(OpenIPC_DeviceId interfaceDeviceId)
    {
        if (_jtagInterface.InterfaceDeviceId == interfaceDeviceId)
        {
            return std::ref(_jtagInterface);
        }
        if (_statePortInterface.InterfaceDeviceId == interfaceDeviceId)
        {
            return std::ref(_statePortInterface);
        }
        if (_traceInterface.InterfaceDeviceId == interfaceDeviceId)
        {
            return std::ref(_traceInterface);
        }
        return std::monostate();
    }

};
const PPI_char* const ReferenceProbe::PROBE_TYPE = "Reference_FakeProbe";

// ==== Plugin ====
class ReferencePlugin
{
    PPI_RefId _pluginId;
    static inline const std::vector<const PPI_char*> _probeTypes {ReferenceProbe::PROBE_TYPE};
    std::vector<ReferenceProbe> _probes;
public:
    // Plugin level configs should be minimized to avoid polluting the root config scope.
    ConfigHolder Configs { { "ReferencePlugin.Setting"sv, "Value" } };

    static void PluginGetInfo(PPI_PluginApiVersion clientInterfaceVersion, PPI_PluginInfo& info) noexcept
    {
        info.clientInterfaceSupported  = clientInterfaceVersion == PPI_PLUGIN_API_VERSION;
        info.pluginInterfaceVersion    = PPI_PLUGIN_API_VERSION;
        info.requiresLockNotifications = 1;
        constexpr char pluginName[] = "ReferencePlugin";
        static_assert(sizeof(pluginName) <= PPI_MAX_INFO_LEN,     "Name should fit in PPI_PluginInfo");
        static_assert(pluginName[sizeof(pluginName) - 1] == '\0', "Name should end with null");

        CopyStringToArray(info.pluginName, pluginName);
    }

    explicit ReferencePlugin(PPI_RefId pluginId) :
        _pluginId(pluginId)
    {
        _addFakeProbe(); // Adding an initial probe, as though we discovered one
    }

    static void PluginGetCapabilities(PPI_PluginCapabilities& capabilities) noexcept
    {
        capabilities.supportAuto = true;
        capabilities.supportDynamicInitialization = true;
    }

    static std::vector<const PPI_char*> PluginGetProbeTypes() noexcept
    {
        // The lifetime of the strings must last until PPI_PluginDeinitialize, since there is no better defined lifetime.
        return _probeTypes;
    }

    std::vector<PPI_RefId> ProbeGetRefIds() const noexcept
    {
        std::vector<PPI_RefId> probeRefIds;
        probeRefIds.reserve(_probes.size());
        for (const auto& probe : _probes)
        {
            probeRefIds.push_back(probe.ProbeRefId);
        }
        return probeRefIds;
    }

    ReferenceProbe* GetProbeByRefId(PPI_RefId probeRefId) noexcept
    {
        const auto probeIter = std::find_if(_probes.begin(), _probes.end(),
                                            [probeRefId](const ReferenceProbe& probe)
                                            {
                                                return probe.ProbeRefId == probeRefId;
                                            });
        if (probeIter == _probes.end())
        {
            return nullptr;
        }
        return &*probeIter;
    }

    ReferenceProbe* GetProbeByDeviceId(OpenIPC_DeviceId probeDeviceId) noexcept
    {
        const auto probeIter = std::find_if(_probes.begin(), _probes.end(),
                                            [probeDeviceId](const ReferenceProbe& probe)
                                            {
                                                return probe.ProbeDeviceId == probeDeviceId;
                                            });
        if (probeIter == _probes.end())
        {
            return nullptr;
        }
        return &*probeIter;
    }

    ReferenceProbe* CreateProbe(const std::string_view probeType) noexcept
    {
        if(probeType != ReferenceProbe::PROBE_TYPE)
        {
            return nullptr;
        }
        _addFakeProbe();
        return &_probes.back();
    }

    MaybeReferenceInterfaceRef GetInterfaceByDeviceId(OpenIPC_DeviceId interfaceDeviceId) noexcept
    {
        MaybeReferenceInterfaceRef result = std::monostate();
        for (auto& probe : _probes)
        {
            result = probe.GetInterfaceByDeviceId(interfaceDeviceId);
            if (!std::holds_alternative<std::monostate>(result))
            {
                return result;
            }
        }
        return result;
    }

    OpenIPC_Error RemoveProbeByDeviceId(OpenIPC_DeviceId probeDeviceId) noexcept
    {
        const auto probeIter = std::find_if(_probes.begin(), _probes.end(),
                                            [probeDeviceId](const ReferenceProbe& probe)
                                            {
                                                return probe.ProbeDeviceId == probeDeviceId;
                                            });
        if (probeIter == _probes.end())
        {
            return OpenIPC_Error_Invalid_Device_ID;
        }
        _probes.erase(probeIter);
        return OpenIPC_Error_No_Error;
    }

private:
    uint32_t _getNextProbeRefId() const noexcept
    {
        static uint32_t lastProbeIndex = 0;
        const uint32_t  newProbeRefId  = _pluginId | (lastProbeIndex << 16);
        lastProbeIndex++;
        return newProbeRefId;
    }

    void _addFakeProbe() noexcept
    {
        _probes.emplace_back(_getNextProbeRefId());
    }

};
static std::unique_ptr<ReferencePlugin> EXAMPLE_PLUGIN_INSTANCE;

// ==== PPI Implementation ====
OpenIPC_Error PPI_PluginGetInfo(PPI_PluginApiVersion clientInterfaceVersion, PPI_PluginInfo* info)
{
    // This may be called before the plugin is initialized
    assert(info != nullptr);
    ReferencePlugin::PluginGetInfo(clientInterfaceVersion, *info);
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_PluginInitialize(PPI_RefId pluginId, const PPI_char* vendorinit)
{
    (void)vendorinit; // We do not use XML for initialization. Dynamic Initialization is preferred.
    EXAMPLE_PLUGIN_INSTANCE.reset(new ReferencePlugin(pluginId));

    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_PluginDeinitialize()
{
    if (EXAMPLE_PLUGIN_INSTANCE == nullptr)
    {
        return OpenIPC_Error_Plugin_Not_Initialized;
    }
    EXAMPLE_PLUGIN_INSTANCE.reset();
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_PluginGetCapabilities(PPI_PluginCapabilities* capabilities)
{
    // This may be called before the Plugin is Initialized
    assert(capabilities != nullptr);
    ReferencePlugin::PluginGetCapabilities(*capabilities);

    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_PluginRegisterEventHandler(PluginEventCallbackHandler eventHandlerFunction)
{
    PLUGIN_HOST_METHODS.SetEventHandler(eventHandlerFunction);
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_PluginRegisterNotificationHandler(PluginNotificationCallbackHandler notificationHandlerFunction)
{
    PLUGIN_HOST_METHODS.SetNotificationHandler(notificationHandlerFunction);
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_ProbeGetRefIds(uint32_t maxIds, PPI_RefId* refIds, uint32_t* probeCount)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    if (probeCount == nullptr)
    {
        return OpenIPC_Error_Null_Pointer;
    }
    if (refIds == nullptr)
    {
        maxIds = 0;
    }

    const auto probeRefIds = EXAMPLE_PLUGIN_INSTANCE->ProbeGetRefIds();
    *probeCount = static_cast<uint32_t>(probeRefIds.size());
    const uint32_t copyCount = std::min(*probeCount, maxIds);
    std::copy_n(probeRefIds.begin(), copyCount, refIds);
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_ProbeBeginInitialization(PPI_RefId probe_refid, OpenIPC_DeviceId probeID)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    const auto probe = EXAMPLE_PLUGIN_INSTANCE->GetProbeByRefId(probe_refid);
    if (!probe)
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
    return probe->BeginInitialization(probeID);
}

OpenIPC_Error PPI_ProbeFinishInitialization(OpenIPC_DeviceId probeID)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    const auto probe = EXAMPLE_PLUGIN_INSTANCE->GetProbeByDeviceId(probeID);
    if (!probe)
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
    return probe->FinishInitialization();
}

OpenIPC_Error PPI_ProbeDeInitialize(OpenIPC_DeviceId probeID)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    return EXAMPLE_PLUGIN_INSTANCE->RemoveProbeByDeviceId(probeID);
}

OpenIPC_Error PPI_ProbeGetInfo(PPI_RefId refId, PPI_ProbeInfo* info)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    assert(info != nullptr);
    ReferenceProbe* probe = EXAMPLE_PLUGIN_INSTANCE->GetProbeByRefId(refId);
    if (probe == nullptr)
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
    *info = probe->GetProbeInfo();
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_InterfaceGetRefIds(OpenIPC_DeviceId probeID, uint8_t maxIds, PPI_RefId* refIds, uint32_t* interfacecount)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    const ReferenceProbe* probe = EXAMPLE_PLUGIN_INSTANCE->GetProbeByDeviceId(probeID);
    if (probe == nullptr)
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
    if (interfacecount == nullptr)
    {
        return OpenIPC_Error_Null_Pointer;
    }
    if (refIds == nullptr)
    {
        maxIds = 0;
    }

    const auto interfaceRefIds = probe->InterfaceGetRefIds();
    *interfacecount = static_cast<uint32_t>(interfaceRefIds.size());
    const uint32_t copyCount = std::min(*interfacecount, static_cast<uint32_t>(maxIds));
    std::copy_n(interfaceRefIds.begin(), copyCount, refIds);
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_InterfaceGetType(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_EInterfaceType* interface_type)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    assert(interface_type != nullptr);
    ReferenceProbe* probe = EXAMPLE_PLUGIN_INSTANCE->GetProbeByDeviceId(probeID);
    if (probe == nullptr)
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
    auto probeInterface = probe->GetInterfaceByRefId(interface_refid);
    return std::visit([&interface_type](auto& maybeInterface)
                      {
                          if constexpr (is_decay_equ<decltype(maybeInterface), std::monostate>)
                          {
                              return OpenIPC_Error_Invalid_Device_ID;
                          }
                          else
                          {
                              *interface_type = maybeInterface.get().GetInterfaceType();
                              return OpenIPC_Error_No_Error;
                          }
                      }, probeInterface);
}

OpenIPC_Error PPI_InterfaceGetInfoJTAG(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_InterfaceJTAGCapabilities* capabilities)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    assert(capabilities != nullptr);
    ReferenceProbe* probe = EXAMPLE_PLUGIN_INSTANCE->GetProbeByDeviceId(probeID);
    if (probe == nullptr)
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
    auto probeInterface = probe->GetInterfaceByRefId(interface_refid);
    return std::visit([&capabilities](auto& maybeInterface)
                      {
                          // Only the ReferenceJtagInterface supports this
                          if constexpr (is_decay_equ<decltype(maybeInterface), ReferenceJtagInterface>)
                          {
                              *capabilities = maybeInterface.get().GetJtagCapabilities();
                              return OpenIPC_Error_No_Error;
                          }
                          else
                          {
                              return OpenIPC_Error_Invalid_Device_ID;
                          }
                      }, probeInterface);
}

OpenIPC_Error PPI_InterfaceBeginInitialization(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, OpenIPC_DeviceId interfaceID)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    ReferenceProbe* probe = EXAMPLE_PLUGIN_INSTANCE->GetProbeByDeviceId(probeID);
    if (probe == nullptr)
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }

    auto probeInterface = probe->GetInterfaceByRefId(interface_refid);
    return std::visit([interfaceID](auto& maybeInterface)
                      {
                          if constexpr (is_decay_equ<decltype(maybeInterface), std::monostate>)
                          {
                              return OpenIPC_Error_Invalid_Device_ID;
                          }
                          else
                          {
                              return maybeInterface.get().BeginInitialization(interfaceID);
                          }
                      }, probeInterface);
}

OpenIPC_Error PPI_InterfaceFinishInitialization(OpenIPC_DeviceId interfaceID)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(interfaceID);
    return std::visit([](auto& maybeInterface)
                      {
                          if constexpr (is_decay_equ<decltype(maybeInterface), std::monostate>)
                          {
                              return OpenIPC_Error_Invalid_Device_ID;
                          }
                          else
                          {
                              return maybeInterface.get().FinishInitialization();
                          }
                      }, probeInterface);
}

OpenIPC_Error PPI_InterfaceDeInitialize(OpenIPC_DeviceId interfaceID)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(interfaceID);
    return std::visit([](auto& maybeInterface)
                      {
                          if constexpr (is_decay_equ<decltype(maybeInterface), std::monostate>)
                          {
                              return OpenIPC_Error_Invalid_Device_ID;
                          }
                          else
                          {
                              return maybeInterface.get().DeInitialization();
                          }
                      }, probeInterface);
}

PPI_ProbeBundleHandle PPI_Bundle_Allocate()
{
    return new ReferenceBundle();
}

OpenIPC_Error PPI_Bundle_Clear(PPI_ProbeBundleHandle handle)
{
    assert(handle != nullptr);
    auto& bundle = *RetrieveBundle(handle);
    // If the bundle is not an empty bundle, the destructor will handle the clear.
    bundle = std::monostate();

    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_Bundle_Execute(PPI_ProbeBundleHandle handle, OpenIPC_DeviceId deviceInterface, PPI_bool keepLock)
{
    (void)keepLock; // don't care about keepLock since the probe has no sharing considerations.
    // handle could be PPI_PROBE_LOCK_RELEASE or PPI_PROBE_LOCK_HOLD or ReferenceBundle*
    if (handle == PPI_PROBE_LOCK_RELEASE || handle == PPI_PROBE_LOCK_HOLD)
    {
        return OpenIPC_Error_Operation_Not_Supported;
    }
    const auto bundle = RetrieveBundle(handle);

    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(deviceInterface);
    return std::visit([](auto& maybeInterface, auto& maybeBundle)
                      {
                          if constexpr (is_decay_equ<decltype(maybeInterface), ReferenceJtagInterface>
                                        && is_decay_equ<decltype(maybeBundle), ReferenceJtagBundle>)
                          {
                              return maybeInterface.get().ExecuteBundle(maybeBundle);
                          }
                          else if constexpr (is_decay_equ<decltype(maybeInterface), ReferenceStatePortInterface>
                                             && is_decay_equ<decltype(maybeBundle), ReferenceStatePortBundle>)
                          {
                              return maybeInterface.get().ExecuteBundle(maybeBundle);
                          }
                          else if (is_decay_equ<decltype(maybeBundle), std::monostate>)
                          {
                              return OpenIPC_Error_No_Error; // Empty bundle, just say it worked.
                          }
                          else
                          {
                              return OpenIPC_Error_Invalid_Device_ID;
                          }
                      }, probeInterface, *bundle);
}

OpenIPC_Error PPI_Bundle_Free(PPI_ProbeBundleHandle* handle)
{
    assert(handle != nullptr);
    const auto bundle = RetrieveBundle(*handle);
    delete bundle;
    return OpenIPC_Error_No_Error;
}

// Support for STATE based JTAG
OpenIPC_Error PPI_JTAG_GoToState(PPI_ProbeBundleHandle handle, JtagStateEncode gotoState, uint32_t numberOfClocksInState, const PPI_JTAG_StateGotoOptions* const options)
{
    assert(handle != nullptr);
    const bool waitForTrigger = options && options->waitForTrigger;
    const bool errorOnTimeout = options && options->errorOnTimeout;

    const auto bundle = RetrieveBundle(handle);
    if (std::holds_alternative<std::monostate>(*bundle))
    {
        *bundle = ReferenceJtagBundle{}; // Empty bundle can become a Jtag bundle
    }
    if (auto* jtagBundle = std::get_if<ReferenceJtagBundle>(bundle))
    {
        return jtagBundle->AppendJtagGoToState(gotoState, numberOfClocksInState, waitForTrigger, errorOnTimeout);
    }
    else
    {
        return OpenIPC_Error_Probe_Bundle_Invalid;
    }
}

OpenIPC_Error PPI_JTAG_StateIRShift(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_StateShiftOptions* const options)
{
    assert(handle != nullptr);
    if (options)
    {
        if (options->savedSlot != PPI_SLOT_HANDLE_INVALID
            || options->TdiTdoOptions & JtagOption_TDI_Restore_From_Slot
            || options->TdiTdoOptions & JtagOption_TDO_Save_To_Slot)
        {
            return OpenIPC_Error_Operation_Not_Supported; // We are not supporting slots
        }
    }
    std::vector<uint8_t> inData;
    if (inBits == nullptr)
    {
        const bool useAllOnes = options && options->TdiTdoOptions & JtagOption_TDI_All_Ones;
        inData = std::vector((shiftLengthBits + 7) / 8, useAllOnes ? static_cast<uint8_t>(0xFF) : static_cast<uint8_t>(0));
    }
    else
    {
        // Need to copy inBits, since it's lifetime is not guaranteed beyond this method.
        // However outBits which is guaranteed to live until either bundle_clear or bundle_execute (if it is provided)
        inData = std::vector(inBits, inBits + (shiftLengthBits + 7) / 8);
    }
    const auto bundle = RetrieveBundle(handle);
    if (std::holds_alternative<std::monostate>(*bundle))
    {
        *bundle = ReferenceJtagBundle{}; // Empty bundle can become a Jtag bundle
    }
    if (auto* jtagBundle = std::get_if<ReferenceJtagBundle>(bundle))
    {
        return jtagBundle->AppendIrScan(shiftLengthBits, std::move(inData), outBits);
    }
    else
    {
        return OpenIPC_Error_Probe_Bundle_Invalid;
    }
}

OpenIPC_Error PPI_JTAG_StateDRShift(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_StateShiftOptions* const options)
{
    assert(handle != nullptr);
    if (options)
    {
        if (options->savedSlot != PPI_SLOT_HANDLE_INVALID
            || options->TdiTdoOptions & JtagOption_TDI_Restore_From_Slot
            || options->TdiTdoOptions & JtagOption_TDO_Save_To_Slot)
        {
            return OpenIPC_Error_Operation_Not_Supported; // We are not supporting slots
        }
    }
    std::vector<uint8_t> inData;
    if (inBits == nullptr)
    {
        const bool useAllOnes = options && options->TdiTdoOptions & JtagOption_TDI_All_Ones;
        inData = std::vector((shiftLengthBits + 7) / 8, useAllOnes ? static_cast<uint8_t>(0xFF) : static_cast<uint8_t>(0));
    }
    else
    {
        // Need to copy inBits, since it's lifetime is not guaranteed beyond this method.
        // However outBits which is guaranteed to live until either bundle_clear or bundle_execute (if it is provided)
        inData = std::vector(inBits, inBits + (shiftLengthBits + 7) / 8);
    }
    const auto bundle = RetrieveBundle(handle);
    if (std::holds_alternative<std::monostate>(*bundle))
    {
        *bundle = ReferenceJtagBundle{}; // Empty bundle can become a Jtag bundle
    }
    if (auto* jtagBundle = std::get_if<ReferenceJtagBundle>(bundle))
    {
        return jtagBundle->AppendDrScan(shiftLengthBits, std::move(inData), outBits);
    }
    else
    {
        return OpenIPC_Error_Probe_Bundle_Invalid;
    }
}

// This method may become optional in the future. In that case, OpenIPC will assume all interfaces have independent locks
OpenIPC_Error PPI_InterfaceListLockInterfacePeers(OpenIPC_DeviceId interfaceID, uint32_t peerInterfacesLength, OpenIPC_DeviceId* peerInterfaces, uint32_t* numberOfPeerInterfaces)
{
    assert(numberOfPeerInterfaces != nullptr);
    (void)peerInterfaces;
    (void)peerInterfacesLength;
    const auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(interfaceID);
    if (std::holds_alternative<std::monostate>(probeInterface))
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
    *numberOfPeerInterfaces = 0;
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_StatePortGetDefinitions(OpenIPC_DeviceId deviceId, const PPI_InterfaceStatePortDefinition** definitions, uint32_t definitionsSize, uint32_t* numberOfDefinitions)
{
    assert(definitions != nullptr || definitionsSize == 0);
    assert(numberOfDefinitions != nullptr);
    const auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(deviceId);
    if (const auto* statePortInterface = std::get_if<std::reference_wrapper<ReferenceStatePortInterface>>(&probeInterface))
    {
        auto defs = statePortInterface->get().GetDefinitions();
        const auto defSize = static_cast<uint32_t>(defs.size());
        *numberOfDefinitions = defSize;
        if (definitions != nullptr)
        {
            const auto copyCount = std::min(defSize, definitionsSize);
            std::copy_n(defs.data(), copyCount, definitions);
        }
        return OpenIPC_Error_No_Error;
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

OpenIPC_Error PPI_StatePortGetName(const PPI_InterfaceStatePortDefinition* definition, char* buffer, uint32_t bufferSize, uint32_t* realSize)
{
    assert(definition != nullptr);
    assert(buffer != nullptr || bufferSize == 0);
    assert(realSize != nullptr);
    const auto statePortDef = ReferenceStatePortDefinition.FromOpaqueHandle(definition);
    const auto sizeNeeded   = static_cast<uint32_t>(statePortDef->Name.size() + 1);
    *realSize = sizeNeeded;
    if (buffer != nullptr)
    {
        const auto copyCount = std::min(sizeNeeded, bufferSize);
        std::copy_n(statePortDef->Name.c_str(), copyCount, buffer);
        buffer[bufferSize] = '\0';
    }
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_StatePortGetDescription(const PPI_InterfaceStatePortDefinition* definition, char* buffer, uint32_t bufferSize, uint32_t* realSize)
{
    assert(definition != nullptr);
    assert(realSize != nullptr);
    const auto statePortDef = ReferenceStatePortDefinition.FromOpaqueHandle(definition);
    const auto sizeNeeded   = static_cast<uint32_t>(statePortDef->Description.size() + 1);
    *realSize = sizeNeeded;
    if (buffer != nullptr)
    {
        const auto copyCount = std::min(sizeNeeded, bufferSize);
        std::copy_n(statePortDef->Description.c_str(), copyCount, buffer);
        buffer[bufferSize] = '\0';
    }
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_StatePortGetAccessSize(const PPI_InterfaceStatePortDefinition* definition, uint32_t* size)
{
    assert(definition != nullptr);
    assert(size != nullptr);
    const auto statePortDef = ReferenceStatePortDefinition.FromOpaqueHandle(definition);
    *size = statePortDef->AccessSizeInBits;
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_StatePortGetParameters(const PPI_InterfaceStatePortDefinition* definition, const PPI_InterfaceStatePortParameter** parameters, uint32_t length, uint32_t* realLength)
{
    assert(definition != nullptr);
    assert(realLength != nullptr);
    assert(parameters != nullptr || length == 0);
    const auto statePortDef  = ReferenceStatePortDefinition.FromOpaqueHandle(definition);
    auto statePortParameters = statePortDef->GetParameters();
    const auto sizeNeeded    = static_cast<uint32_t>(statePortParameters.size());
    *realLength = sizeNeeded;
    if (parameters != nullptr)
    {
        const auto copyCount = std::min(sizeNeeded, length);
        std::copy_n(statePortParameters.data(), copyCount, parameters);
    }
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_StatePortParameterGetName(const PPI_InterfaceStatePortParameter* parameter, char* buffer, uint32_t bufferSize, uint32_t* realSize)
{
    assert(parameter != nullptr);
    assert(realSize != nullptr);
    assert(buffer != nullptr || bufferSize == 0);
    const auto parameterDef = ReferenceStatePortDefinition.Parameter0.FromOpaqueHandle(parameter);
    const auto sizeNeeded   = static_cast<uint32_t>(parameterDef->Name.size() + 1);
    *realSize = sizeNeeded;
    if (buffer != nullptr)
    {
        const auto copyCount = std::min(sizeNeeded, bufferSize);
        std::copy_n(parameterDef->Name.c_str(), copyCount, buffer);
        buffer[bufferSize] = '\0';
    }
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_StatePortParameterGetDescription(const PPI_InterfaceStatePortParameter* parameter, char* buffer, uint32_t bufferSize, uint32_t* realSize)
{
    assert(parameter != nullptr);
    assert(realSize != nullptr);
    const auto parameterDef = ReferenceStatePortDefinition.Parameter0.FromOpaqueHandle(parameter);
    const auto sizeNeeded   = static_cast<uint32_t>(parameterDef->Description.size() + 1);
    *realSize = sizeNeeded;
    if (buffer != nullptr)
    {
        const auto copyCount = std::min(sizeNeeded, bufferSize);
        std::copy_n(parameterDef->Description.c_str(), copyCount, buffer);
        buffer[bufferSize] = '\0';
    }
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_StatePortParameterGetSize(const PPI_InterfaceStatePortParameter* parameter, uint32_t* size)
{
    assert(parameter != nullptr);
    assert(size != nullptr);
    const auto parameterDef = ReferenceStatePortDefinition.Parameter0.FromOpaqueHandle(parameter);
    *size = parameterDef->SizeInBits;
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_StatePortParameterGetDefaultValue(const PPI_InterfaceStatePortParameter* parameter, uint8_t* buffer, uint32_t size)
{
    assert(parameter != nullptr);
    assert(buffer != nullptr);
    const auto parameterDef = ReferenceStatePortDefinition.Parameter0.FromOpaqueHandle(parameter);
    assert(size >= (parameterDef->SizeInBits + 7 / 8));
    std::optional<std::vector<uint8_t>> defaultValue = parameterDef->DefaultValue;
    if (!defaultValue.has_value())
    {
        return OpenIPC_Error_Not_Supported;
    }
    const auto copyCount = std::min(static_cast<size_t>(size), defaultValue->size());
    std::copy_n(defaultValue->data(), copyCount, buffer);
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_StatePortAllocateOperation(OpenIPC_DeviceId deviceId, const PPI_InterfaceStatePortDefinition* definition, PPI_StatePort_OPERATION_TYPE_ET operationType, PPI_InterfaceStatePortOperation** operation, const PPI_StatePortAllocateOperation_Options* options)
{
    assert(definition != nullptr);
    assert(operation != nullptr);
    (void)options; // Not used by this version of PPI
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(deviceId);
    if (auto* statePortInterface = std::get_if<std::reference_wrapper<ReferenceStatePortInterface>>(&probeInterface))
    {
        (void)statePortInterface; // Not used
        const auto statePortDef = ReferenceStatePortDefinition.FromOpaqueHandle(definition);
        (void)statePortDef;
        if (operationType != StatePort_OPERATION_READ && operationType != StatePort_OPERATION_WRITE)
        {
            return OpenIPC_Error_Not_Supported;
        }
        *operation = reinterpret_cast<PPI_InterfaceStatePortOperation*>(new ReferenceStatePortOperation(operationType == StatePort_OPERATION_READ));
        return OpenIPC_Error_No_Error;
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

OpenIPC_Error PPI_StatePortFreeOperation(OpenIPC_DeviceId deviceId, PPI_InterfaceStatePortOperation** operation)
{
    assert(operation != nullptr);
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(deviceId);
    if (auto* statePortInterface = std::get_if<std::reference_wrapper<ReferenceStatePortInterface>>(&probeInterface))
    {
        (void)statePortInterface; // Not used
        const auto statePortOperation = ReferenceStatePortOperation::FromOpaqueHandle(*operation);
        delete statePortOperation;
        *operation = INVALID_OPERATION;
        return OpenIPC_Error_No_Error;
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

OpenIPC_Error PPI_StatePortOperationSetParameter(PPI_InterfaceStatePortOperation* operation, const PPI_InterfaceStatePortParameter* parameter, const uint8_t* value, uint32_t size)
{
    assert(operation != nullptr);
    assert(parameter != nullptr);
    assert(value != nullptr);

    const auto statePortOperation = ReferenceStatePortOperation::FromOpaqueHandle(operation);
    const auto statePortParameter = ReferenceStatePortDefinition.Parameter0.FromOpaqueHandle(parameter);
    return statePortOperation->SetParameter(statePortParameter, value, size);
}

OpenIPC_Error PPI_StatePortOperationSetWriteValue(PPI_InterfaceStatePortOperation* operation, const uint8_t* buffer, uint32_t size)
{
    assert(operation != nullptr);
    assert(buffer != nullptr);

    const auto statePortOperation = ReferenceStatePortOperation::FromOpaqueHandle(operation);
    return statePortOperation->SetWriteValue(buffer, size);
}

OpenIPC_Error PPI_StatePortOperationSetReadBuffer(PPI_InterfaceStatePortOperation* operation, uint8_t* buffer, uint32_t size)
{
    assert(operation != nullptr);
    assert(buffer != nullptr);

    const auto statePortOperation = ReferenceStatePortOperation::FromOpaqueHandle(operation);
    return statePortOperation->SetReadBuffer(buffer, size);
}

OpenIPC_Error PPI_StatePortOperationSetErrorBuffer(PPI_InterfaceStatePortOperation* operation, ProbeStatePortError* errorBuffer)
{
    assert(operation != nullptr);
    assert(errorBuffer != nullptr);

    const auto statePortOperation = ReferenceStatePortOperation::FromOpaqueHandle(operation);
    return statePortOperation->SetErrorBuffer(errorBuffer);
}

OpenIPC_Error PPI_StatePortAppendOperation(OpenIPC_DeviceId deviceId, PPI_ProbeBundleHandle bundle, PPI_InterfaceStatePortOperation** operationPointer, const PPI_StatePortAppendOperation_Options* options)
{
    assert(operationPointer != nullptr);
    (void)options; // Not used.
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(deviceId);
    if (auto* statePortInterface = std::get_if<std::reference_wrapper<ReferenceStatePortInterface>>(&probeInterface))
    {
        (void)statePortInterface; // Not used
        auto statePortOperation = ReferenceStatePortOperation::FromOpaqueHandle(*operationPointer);

        const auto pluginBundle = RetrieveBundle(bundle);
        if (std::holds_alternative<std::monostate>(*pluginBundle))
        {
            *pluginBundle = ReferenceStatePortBundle{}; // Empty bundle can become a StatePort bundle
        }
        if (auto* statePortBundle = std::get_if<ReferenceStatePortBundle>(pluginBundle))
        {
            const auto error = statePortBundle->AppendOperation(std::unique_ptr<ReferenceStatePortOperation>(statePortOperation));
            if (error == OpenIPC_Error_No_Error)
            {
                *operationPointer = INVALID_OPERATION;
            }
            return error;
        }
        else
        {
            return OpenIPC_Error_Probe_Bundle_Invalid;
        }
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

// ==== Trace ====
OpenIPC_Error PPI_InterfaceTraceOpenWindow(OpenIPC_DeviceId interfaceID, PPI_Trace_PortAccessMode accessMode)
{
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(interfaceID);
    if (auto* traceInterface = std::get_if<std::reference_wrapper<ReferenceTraceInterface>>(&probeInterface))
    {
        return traceInterface->get().OpenWindow(accessMode);
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

OpenIPC_Error PPI_InterfaceTraceCloseWindow(OpenIPC_DeviceId interfaceID)
{
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(interfaceID);
    if (auto* traceInterface = std::get_if<std::reference_wrapper<ReferenceTraceInterface>>(&probeInterface))
    {
        return traceInterface->get().CloseWindow();
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

OpenIPC_Error PPI_InterfaceTraceIsReadDataAvailable(OpenIPC_DeviceId interfaceID, PPI_bool* isDataAvailable)
{
    assert(isDataAvailable != nullptr);
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(interfaceID);
    if (auto* traceInterface = std::get_if<std::reference_wrapper<ReferenceTraceInterface>>(&probeInterface))
    {
        *isDataAvailable = traceInterface->get().IsReadDataAvailable();
        return OpenIPC_Error_No_Error;
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

OpenIPC_Error PPI_InterfaceTraceIsWindowOpen(OpenIPC_DeviceId interfaceID, PPI_bool* isWindowOpen)
{
    assert(isWindowOpen != nullptr);
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(interfaceID);
    if (auto* traceInterface = std::get_if<std::reference_wrapper<ReferenceTraceInterface>>(&probeInterface))
    {
        *isWindowOpen = traceInterface->get().IsWindowOpen();
        return OpenIPC_Error_No_Error;
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

OpenIPC_Error PPI_InterfaceTracePortRead(OpenIPC_DeviceId interfaceID, uint8_t* output, uint32_t maxOutputBytes, uint32_t* outputBytes)
{
    assert(output != nullptr);
    assert(outputBytes != nullptr);
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(interfaceID);
    if (auto* traceInterface = std::get_if<std::reference_wrapper<ReferenceTraceInterface>>(&probeInterface))
    {
        return traceInterface->get().Read(output, maxOutputBytes, *outputBytes);
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

OpenIPC_Error PPI_InterfaceTracePortWrite(OpenIPC_DeviceId interfaceID, const uint8_t* input, uint32_t maxInputBytes, uint32_t* inputBytes)
{
    assert(input != nullptr);
    assert(inputBytes != nullptr);
    auto probeInterface = EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(interfaceID);
    if (auto* traceInterface = std::get_if<std::reference_wrapper<ReferenceTraceInterface>>(&probeInterface))
    {
        return traceInterface->get().Write(input, maxInputBytes, *inputBytes);
    }
    else
    {
        return OpenIPC_Error_Invalid_Device_ID;
    }
}

// ==== Logging ====
OpenIPC_Error PPI_PluginSetStreamLogEventHandler(PluginStreamLogCallbackHandler logCallBackFunction)
{
    PLUGIN_LOGGER.SetLogHandler(logCallBackFunction);
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_PluginUsesLoggerStream(PPI_Stream stream, PPI_bool* usesLoggerStream)
{
    assert(usesLoggerStream != nullptr);
    *usesLoggerStream = PLUGIN_LOGGER.IsStreamUsed(stream);
    return OpenIPC_Error_No_Error;
}

// ==== Device Configs ====

using ReferenceDevice = std::variant<std::monostate, std::reference_wrapper<ReferencePlugin>, std::reference_wrapper<ReferenceProbe>,
                                     std::reference_wrapper<ReferenceJtagInterface>, std::reference_wrapper<ReferenceStatePortInterface>, std::reference_wrapper<ReferenceTraceInterface>>;

ReferenceDevice GetDevice(OpenIPC_DeviceId deviceID)
{
    if (deviceID == OpenIPC_INVALID_DEVICE_ID || deviceID == 0)
    {
        return std::ref(*EXAMPLE_PLUGIN_INSTANCE);
    }
    else if (auto maybeProbe = EXAMPLE_PLUGIN_INSTANCE->GetProbeByDeviceId(deviceID))
    {
        return std::ref(*maybeProbe);
    }
    return std::visit([](auto&& arg) -> ReferenceDevice
                      {
                          return arg;
                      },
                      EXAMPLE_PLUGIN_INSTANCE->GetInterfaceByDeviceId(deviceID));
}

OpenIPC_Error PPI_DeviceListConfigString(OpenIPC_DeviceId deviceID, uint32_t configTypeLength, PPI_char const** configType, uint32_t* numberOfConfigTypes)
{
    assert(numberOfConfigTypes != nullptr);
    auto maybeDevice = GetDevice(deviceID);
    return std::visit([&](auto& maybeDevice)
                      {
                          if constexpr (is_decay_equ<decltype(maybeDevice), std::monostate>)
                          {
                              return OpenIPC_Error_Invalid_Device_ID;
                          }
                          else
                          {
                              std::vector<const char*> configTypePointers = maybeDevice.get().Configs.GetTypes();
                              *numberOfConfigTypes = static_cast<uint32_t>(configTypePointers.size());
                              if (configType != nullptr)
                              {
                                  auto copySize = std::min(configTypeLength, *numberOfConfigTypes);
                                  std::copy_n(configTypePointers.data(), copySize, configType);
                              }
                              return OpenIPC_Error_No_Error;
                          }
                      }, maybeDevice);
}

OpenIPC_Error PPI_DeviceGetConfig(OpenIPC_DeviceId deviceID, const PPI_char* configType, PPI_char value[PPI_MAX_INFO_LEN])
{
    assert(configType != nullptr);
    assert(value != nullptr);
    auto maybeDevice = GetDevice(deviceID);
    return std::visit([&](auto& maybeDevice)
                      {
                          if constexpr (is_decay_equ<decltype(maybeDevice), std::monostate>)
                          {
                              return OpenIPC_Error_Invalid_Device_ID;
                          }
                          else
                          {
                              auto configValue = maybeDevice.get().Configs.TryGet(configType);
                              if (configValue)
                              {
                                  CopyStringToArray(value, PPI_MAX_INFO_LEN, configValue.value());
                                  return OpenIPC_Error_No_Error;
                              }
                              return OpenIPC_Error_Probe_Config_Undefined;

                          }
                      }, maybeDevice);
}

OpenIPC_Error PPI_DeviceSetConfig(OpenIPC_DeviceId deviceID, const PPI_char* configType, const PPI_char value[PPI_MAX_INFO_LEN])
{
    assert(configType != nullptr);
    assert(value != nullptr);
    auto maybeDevice = GetDevice(deviceID);
    return std::visit([&](auto& maybeDevice)
                      {
                          if constexpr (is_decay_equ<decltype(maybeDevice), std::monostate>)
                          {
                              return OpenIPC_Error_Invalid_Device_ID;
                          }
                          else
                          {
                              auto foundConfig = maybeDevice.get().Configs.TrySet(configType, value);
                              if (foundConfig)
                              {
                                  return OpenIPC_Error_No_Error;
                              }
                              return OpenIPC_Error_Probe_Config_Undefined;

                          }
                      }, maybeDevice);
}


// ==== Static Probe Support ====
OpenIPC_Error PPI_PluginGetProbeTypes(uint32_t maxProbeTypes, PPI_char const** probeTypes, uint32_t* probeTypeCount)
{
    if (probeTypeCount == nullptr)
    {
        return OpenIPC_Error_Null_Pointer;
    }
    if (probeTypes == nullptr)
    {
        maxProbeTypes = 0;
    }

    const auto& pluginProbeTypes = ReferencePlugin::PluginGetProbeTypes();
    *probeTypeCount = static_cast<uint32_t>(pluginProbeTypes.size());
    const uint32_t copyCount = std::min(*probeTypeCount, maxProbeTypes);
    for (uint32_t i = 0; i < copyCount; i++)
    {
        probeTypes[i] = pluginProbeTypes[i];
    }
    return OpenIPC_Error_No_Error;
}

OpenIPC_Error PPI_PluginCreateStaticProbe(const PPI_char probeType[PPI_MAX_PROBE_TYPE_LEN], PPI_RefId* refId)
{
    assert(EXAMPLE_PLUGIN_INSTANCE != nullptr);
    assert(probeType != nullptr);
    assert(refId != nullptr);
    
    auto maybeProbe = EXAMPLE_PLUGIN_INSTANCE->CreateProbe(probeType);
    if (maybeProbe)
    {
        *refId = maybeProbe->ProbeRefId;
        return OpenIPC_Error_No_Error;
    }
    return OpenIPC_Error_Probe_Invalid_Parameter;
}
