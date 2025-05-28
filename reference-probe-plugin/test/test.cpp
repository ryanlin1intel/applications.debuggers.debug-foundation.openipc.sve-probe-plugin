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

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <string_view>
#include <exception>
#include <memory>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace // helpers
{

    bool EndsWith(std::string_view str, std::string_view suffix)
    {
        return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
    }

    bool StartsWith(std::string_view str, std::string_view prefix)
    {
        return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
    }

    #if defined(_WIN32)
        void* GetProcedureFromDll(HMODULE libraryHandle, LPCSTR procName)
        {
            void* pProc = NULL;
            if (libraryHandle != NULL && procName != NULL)
            {
                pProc = reinterpret_cast<void*>(::GetProcAddress(libraryHandle, procName));
            }
            return pProc;
        }

        using DllHandle = std::unique_ptr<std::remove_pointer_t<HMODULE>, decltype(& ::FreeLibrary)>;
        DllHandle LoadDll(const std::string& dllName)
        {
            std::cout << "Trying to load " << std::quoted(dllName) << "\n";
            // Tell Windows to search the DLL's path for dependencies the DLL
            // might have (by default, Windows will not search the same folder
            // as the DLL itself for dependencies).
            ::SetDllDirectoryA(".");
            HMODULE handle = ::LoadLibraryA(dllName.c_str());
            return DllHandle(handle, &::FreeLibrary);
        }

    #else
        void* GetProcedureFromDll(void* libraryHandle, const char* procName)
        {
            void* pProc = NULL;

            if (libraryHandle != NULL && procName != NULL)
            {
                pProc = dlsym(libraryHandle, procName);
            }
            return pProc;
        }

        using DllHandle = std::unique_ptr<void, void (*)(void*)>;
        DllHandle LoadDll(const std::string& dllNameIn)
        {
            std::string dllName = "./" + dllNameIn; // load it from the current directory
            std::cout << "Trying to load " << std::quoted(dllName) << "\n";
            auto result = DllHandle(dlopen(dllName.c_str(), RTLD_LAZY | RTLD_LOCAL), [](void* handle) { dlclose(handle); });
            if (!result)
            {
                std::cerr << "dlerror = " << dlerror() << "\n";
            }
            return result;
        }

    #endif

    template<typename T>
    void RequireEqual(const T& actual, const T& expected, std::string_view message)
    {
        if (actual != expected)
        {
            std::cerr << message << " Got " << actual << " but expected " << expected << ".\n";
            throw std::runtime_error("");
        }
    }

    template<typename T>
    void RequireNotEqual(const T& actual, const T& expected, std::string_view message)
    {
        if (actual == expected)
        {
            std::cerr << message << " Got " << actual << " but expected to NOT get " << expected << ".\n";
            throw std::runtime_error("");
        }
    }

}

int TestPluginName(const std::string&pluginName)
{
    // Collect the dlls/sos in the specified paths
    #if defined(_WIN32)
        const std::string PluginSearchStringPrefix  = "ProbePlugin";
        const std::string PluginSearchStringPostfix = ".dll";
    #elif defined(__linux__)
        const std::string PluginSearchStringPrefix  = "libProbePlugin";
        const std::string PluginSearchStringPostfix = ".so";
    #elif defined(__APPLE__)
        const std::string PluginSearchStringPrefix  = "libProbePlugin";
        const std::string PluginSearchStringPostfix = ".dylib";
    #endif

    if (!StartsWith(pluginName, PluginSearchStringPrefix))
    {
        std::cerr << "Plugin name " << std::quoted(pluginName) << " does not start with " << std::quoted(PluginSearchStringPrefix) << "\n";
        return 1;
    }
    if (!EndsWith(pluginName, PluginSearchStringPostfix))
    {
        std::cerr << "Plugin name " << std::quoted(pluginName) << " does not end with " << std::quoted(PluginSearchStringPostfix) << "\n";
        return 1;
    }

    return 0;
}

int _checkRequiredMethods(const DllHandle& dllHandle, const std::vector<const char*> &methods)
{
    bool hasAllRequiredMethods = true;
    for (auto& methodName : methods)
    {
        auto methodPointer = GetProcedureFromDll(dllHandle.get(), methodName);
        if (methodPointer == nullptr)
        {
            std::cerr << "Failed to find required PPI function " << std::quoted(methodName) << ".\n";
            hasAllRequiredMethods = false;
        }
    }
    if (!hasAllRequiredMethods)
    {
        return 1;
    }

    return 0;
}

int _checkRequiredMethods(const std::string& pluginName, const std::vector<const char*> &methods)
{
    const auto dllHandle = LoadDll(pluginName);
    if (!dllHandle)
    {
        std::cerr << "Failed to load dll " << std::quoted(pluginName) << ".\n";
        return 1;
    }
    return _checkRequiredMethods(dllHandle, methods);
}

int TestLoad(const std::string& pluginName)
{
    const auto dllHandle = LoadDll(pluginName);
    if (!dllHandle)
    {
        std::cerr << "Failed to load dll " << std::quoted(pluginName) << ".\n";
        return 1;
    }
    const auto pluginGetInfo = reinterpret_cast<PPI_PluginGetInfo_TYPE>(GetProcedureFromDll(dllHandle.get(), "PPI_PluginGetInfo"));
    if (pluginGetInfo == nullptr)
    {
        std::cerr << "Failed to find required PPI function " << std::quoted("PPI_PluginGetInfo") << ".\n";
        return 1;
    }
    PPI_PluginInfo info {};
    const OpenIPC_Error result = pluginGetInfo(PPI_PLUGIN_API_VERSION, &info);
    if (result != OpenIPC_Error_No_Error)
    {
        std::cerr << "PPI_PluginGetInfo returned an error: " << result << ".\n";
        return 1;
    }
    RequireEqual(info.clientInterfaceSupported, static_cast<PPI_bool>(1), "PPI_PluginGetInfo returned unexpected clientInterfaceSupported.");
    RequireEqual(info.pluginInterfaceVersion,   PPI_PLUGIN_API_VERSION,   "PPI_PluginGetInfo returned unexpected pluginInterfaceVersion.");
    RequireNotEqual(info.pluginName[0], '\0', "PPI_PluginGetInfo returned an empty pluginName.");
    RequireEqual(info.pluginName[PPI_MAX_INFO_LEN - 1], '\0', "PPI_PluginGetInfo returned a pluginName that does not end with a null.");
    // Value of info.requiresLock is implementation specific

    return _checkRequiredMethods(dllHandle, {
        "PPI_PluginDeinitialize",
        "PPI_PluginGetInfo",
        "PPI_PluginInitialize",
        "PPI_PluginRegisterEventHandler",
        "PPI_PluginRegisterNotificationHandler",
        "PPI_ProbeGetRefIds",
        "PPI_ProbeDeInitialize",
        "PPI_ProbeGetInfo",
        "PPI_ProbeBeginInitialization", "PPI_ProbeFinishInitialization", // Or "PPI_ProbeInitialize",
        "PPI_InterfaceGetRefIds",
    });
}

int TestJtagBundleSupport(const std::string& pluginName)
{
    return _checkRequiredMethods(pluginName, {
        // Required for any Interface
        "PPI_InterfaceGetType",
        "PPI_InterfaceBeginInitialization",
        "PPI_InterfaceFinishInitialization",
        "PPI_InterfaceDeInitialize",
        "PPI_InterfaceListLockInterfacePeers",
        // Required for jtag interfaces
        "PPI_InterfaceGetInfoJTAG",
        // Required for an Interface that uses Bundles
        "PPI_Bundle_Allocate",
        "PPI_Bundle_Clear",
        "PPI_Bundle_Execute",
        "PPI_Bundle_Free",
        // Required for State based JTAG
        "PPI_JTAG_GoToState",
        "PPI_JTAG_StateIRShift",
        "PPI_JTAG_StateDRShift",
    });
}


int TestStatePortSupport(const std::string& pluginName)
{
    return _checkRequiredMethods(pluginName, {
        // Required for any Interface
        "PPI_InterfaceGetType",
        "PPI_InterfaceBeginInitialization",
        "PPI_InterfaceFinishInitialization",
        "PPI_InterfaceDeInitialize",
        "PPI_InterfaceListLockInterfacePeers",
        // Required for an Interface that uses Bundles
        "PPI_Bundle_Allocate",
        "PPI_Bundle_Clear",
        "PPI_Bundle_Execute",
        "PPI_Bundle_Free",
        // Required for StatePort
        "PPI_StatePortGetDefinitions",
        "PPI_StatePortGetName",
        "PPI_StatePortGetDescription",
        "PPI_StatePortGetAccessSize",
        "PPI_StatePortGetParameters",
        "PPI_StatePortParameterGetName",
        "PPI_StatePortParameterGetDescription",
        "PPI_StatePortParameterGetSize",
        "PPI_StatePortParameterGetDefaultValue",
        "PPI_StatePortAllocateOperation",
        "PPI_StatePortFreeOperation",
        "PPI_StatePortOperationSetParameter",
        "PPI_StatePortOperationSetWriteValue",
        "PPI_StatePortOperationSetReadBuffer",
        "PPI_StatePortOperationSetErrorBuffer",
        "PPI_StatePortAppendOperation"
    });
}


int TestTraceSupport(const std::string& pluginName)
{
    return _checkRequiredMethods(pluginName, {
        // Required for any Interface
        "PPI_InterfaceGetType",
        "PPI_InterfaceBeginInitialization",
        "PPI_InterfaceFinishInitialization",
        "PPI_InterfaceDeInitialize",
        "PPI_InterfaceListLockInterfacePeers",
        // Required for Trace
        "PPI_InterfaceTraceOpenWindow",
        "PPI_InterfaceTraceCloseWindow",
        "PPI_InterfaceTraceIsReadDataAvailable",
        "PPI_InterfaceTraceIsWindowOpen",
        "PPI_InterfaceTracePortRead",
        "PPI_InterfaceTracePortWrite"
    });
}

int TestDeviceConfigSupport(const std::string& pluginName)
{
    return _checkRequiredMethods(pluginName, {
        "PPI_DeviceListConfigString",
        "PPI_DeviceGetConfig",
        "PPI_DeviceSetConfig"
    });
}

int TestStaticProbeSupport(const std::string& pluginName)
{
    return _checkRequiredMethods(pluginName, {
        "PPI_PluginGetProbeTypes",
        "PPI_PluginCreateStaticProbe"
    });
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Probe Plugin Name must be passed as the first argument.\n";
        return 1;
    }

    try
    {
        if (auto result = TestPluginName(argv[1]))
        {
            std::cout << "TestPluginName failed.\n";
            return result;
        }
        if (auto result = TestLoad(argv[1]))
        {
            std::cout << "TestLoad failed.\n";
            return result;
        }
        if (auto result = TestJtagBundleSupport(argv[1]))
        {
            std::cout << "TestJtagBundleSupport failed.\n";
            return result;
        }
        if (auto result = TestStatePortSupport(argv[1]))
        {
            std::cout << "TestStatePortSupport failed.\n";
            return result;
        }
        if (auto result = TestTraceSupport(argv[1]))
        {
            std::cout << "TestTraceSupport failed.\n";
            return result;
        }
        if (auto result = TestDeviceConfigSupport(argv[1]))
        {
            std::cout << "TestDeviceConfigSupport failed.\n";
            return result;
        }
        if (auto result = TestStaticProbeSupport(argv[1]))
        {
            std::cout << "TestStaticProbeSupport failed.\n";
            return result;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0; // PASS
}
