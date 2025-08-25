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

//by martin.monroy@intel.com
//build with x64 VSS tools
//build cmd: cl receiver.cpp /Fe:receiver.exe /EHsc ws2_32.lib msxml6.lib
//replaced by cmake command.
//the msxml6.lib library is not native on the vs tools framework so it needs to manually be added on the build command since it is still available to the build environment. 
//test receiver app for openipc remote plugin development. 

#include "receiver.h"

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

std::vector<uint8_t> Shift(const std::vector<uint8_t>& input, size_t bitSize)
{
    std::cout << "input.size() = " << input.size() << ", bitSize = " << bitSize << '\n';
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


std::string bytes_to_hex_list(const std::vector<std::uint8_t>& bytes) {
    if (bytes.empty()) return std::string{};
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        if (i) oss << ", ";
        // cast to unsigned so it prints as a number, not a char
        oss << "0x" << std::setw(2) << static_cast<unsigned>(bytes[i]);
    }
    return oss.str();
}


// Trim ASCII whitespace in-place (C++14)
static inline void trim_inplace(std::string& s) {
    auto not_space = [](unsigned char ch){ return std::isspace(ch) == 0; };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
}

// Parse comma-separated byte literals (e.g. "0x1A, 255, FF") into 'out'.
// Accepts: 0x.. / 0X.. (hex), plain hex like "FF", or decimal like "255".
// Returns true on success; on failure returns false and sets 'error'.
bool parse_byte_vector(const std::string& input,std::vector<std::uint8_t>& out,std::string& error)
{
    out.clear();
    std::stringstream ss(input);
    std::string token;
    std::size_t index = 0;

    while (std::getline(ss, token, ',')) {
        ++index;
        trim_inplace(token);

        if (token.empty()) {
            error = "Empty element at position " + std::to_string(index);
            return false;
        }

        // Disallow signs for byte values
        if (token.front() == '+' || token.front() == '-') {
            error = "Sign not allowed for byte value at position " + std::to_string(index);
            return false;
        }

        // Choose base: autodetect if "0x"/"0X", else hex if it has hex letters, else decimal.
        int base = 10;
        if (token.size() >= 2 && token[0] == '0' && (token[1] == 'x' || token[1] == 'X')) {
            base = 0; // std::stoul autodetects 0x / 0 prefix
        } else if (token.find_first_of("abcdefABCDEF") != std::string::npos) {
            base = 16;
        } // else base = 10

        // Parse using stoul with position to detect leftovers
        std::size_t pos = 0;
        unsigned long value = 0;
        try {
            value = std::stoul(token, &pos, base);
        } catch (const std::invalid_argument&) {
            error = "Invalid number at position " + std::to_string(index) + " (token: '" + token + "')";
            return false;
        } catch (const std::out_of_range&) {
            error = "Value out of range at position " + std::to_string(index) + " (token: '" + token + "')";
            return false;
        }

        if (pos != token.size()) {
            error = "Unexpected characters in token at position " + std::to_string(index) +
                    " (near: '" + token.substr(pos) + "')";
            return false;
        }
        if (value > 0xFFul) {
            error = "Value exceeds byte range at position " + std::to_string(index) +
                    " (got " + std::to_string(value) + ")";
            return false;
        }

        out.push_back(static_cast<std::uint8_t>(value));
    }

    // Treat empty input as success with empty vector; change if your policy differs.
    return true;
}


bool to_size_t_stoul(const std::string& s, size_t& out) {
    try {
        unsigned long long v = std::stoull(s);          // handles leading/trailing spaces, base prefixes, +/-
        if (v > (std::numeric_limits<size_t>::max)())   // make sure it fits
            return false;
        out = static_cast<size_t>(v);
        return true;
    } catch (...) {
        return false;
    }
}

std::string extractXMLElement(IXMLDOMDocument* doc, const std::wstring& tag) {
    IXMLDOMNode* node = nullptr;
    BSTR tagName = SysAllocString(tag.c_str());
    doc->selectSingleNode(tagName, &node);
    SysFreeString(tagName);

    if (!node) return "";

    BSTR value;
    node->get_text(&value);
    std::string result = _bstr_t(value);
    SysFreeString(value);
    node->Release();
    return result;
}

std::string buildXMLResponseInit(const std::string& request_id) {
    std::ostringstream oss;
    oss << "<response>"
        << "<request_id>" << request_id << "</request_id>"
        << "<status>OK</status>"
        << "</response>";
    return oss.str();
}

std::string buildXMLResponseShift(const std::string& request_id, const std::vector<std::uint8_t>& shiftOutput) {
    std::ostringstream oss;
    std::string outputVector = bytes_to_hex_list(shiftOutput); //convert to string
    
    oss << "<response>"
        << "<request_id>" << request_id << "</request_id>"
        << "<status>OK</status>"
        << "<output>" << outputVector << "</output>"
        << "</response>";
    return oss.str();
}            
                
int main() {
    CoInitialize(nullptr);

    WSADATA wsaData;
    SOCKET listenSocket, clientSocket;
    sockaddr_in serverAddr{}, clientAddr{};
    char buffer[2048];

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);
    bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, SOMAXCONN);

    std::ofstream logfile("receiver_log.txt", std::ios::app);
    std::cout << "Receiver listening on port 12345...\n";

    while (true) {
        int clientSize = sizeof(clientAddr);
        clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &clientSize);
        if (clientSocket == INVALID_SOCKET) continue;

        //WAIT for a request
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        //If request received process it
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string xmlStr(buffer);  //string buffer to be parsed from xml request

            logfile << "Received XML:\n" << xmlStr << "\n";

            //variable prep to process incoming request
            IXMLDOMDocument* doc = nullptr; //XML document to parse and extract request data
            HRESULT hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&doc));
            VARIANT_BOOL success = VARIANT_FALSE;

            if (SUCCEEDED(hr)) { //just checking we did generate this object correctly
                BSTR xmlBstr = _com_util::ConvertStringToBSTR(xmlStr.c_str()); //load the string onto the xml object
                doc->loadXML(xmlBstr, &success); //load xml into a parsable xml object and verify the load on variable success
                SysFreeString(xmlBstr); //release memory allocated to BSTR xmlBstr for processing request since it is now on the doc variable. 
            }

            if (success == VARIANT_TRUE) {//load check and subsequent variable load
                std::string request_id = extractXMLElement(doc, L"/request/request_id");
               
                std::string initialize = extractXMLElement(doc, L"/request/initialize");  //True or False if true ten value/size must be included
                if(initialize=="False"){
                    //std::string payload = extractXMLElement(doc, L"/request/payload");
                    std::string bitSizeS = extractXMLElement(doc, L"/request/bitSize"); //bitsize is a size_t on the trans side
                    std::string inputS = extractXMLElement(doc, L"/request/input");  //input vector<uint8_t> on the trans
                    
                    size_t bitSizeT;
                    to_size_t_stoul(bitSizeS,bitSizeT); //convert string into size_t for function manipulation in receiver
                    
                    //Vector example: 
                    /*
                        std::vector<std::uint8_t> myVec = {
                                0x00, 0x11, 0x22, 0x33, 0x44,
                                0x55, 0x66, 0x77, 0x88, 0x99
                            };
                    Assume I have this in the xml tag: 0x00, 0x11, 0x22, 0x33, 0x44,0x55, 0x66, 0x77, 0x88, 0x99
                    
                    */
                    std::vector<std::uint8_t> inputV;
                    std::string error;
                    if (parse_byte_vector(inputS, inputV, error)) {
                        std::cout << "Parsed " << inputV.size() << " bytes:\n";
                        for (std::size_t i = 0; i < inputV.size(); ++i) {
                            std::cout << "0x" << std::hex << std::uppercase
                                      << static_cast<int>(inputV[i])
                                      << (i + 1 == inputV.size() ? "\n" : ", ");
                        }
                    } else {
                        std::cerr << "Parse failed: " << error << "\n";
                    }
                    
                    std::vector<std::uint8_t> shiftOutput;
                    shiftOutput = Shift(inputV, bitSizeT); //shiftOutput is the return vector
                    
                    ////////////////////////////RESPONSE//////////////////////////////////////
                    std::string xmlResponse = buildXMLResponseShift(request_id, shiftOutput);
                    send(clientSocket, xmlResponse.c_str(), xmlResponse.size(), 0);
                    logfile << "Responded:\n" << xmlResponse << "\n\n";
                }
                else{ //initialize size and value
                    std::string initSize =  extractXMLElement(doc, L"/request/size");
                    std::string initValue = extractXMLElement(doc, L"/request/value");
                    size_t sizeToSet;
                    to_size_t_stoul(initSize,sizeToSet); //convert string into size_t for function manipulation in receiver
                    set_size(sizeToSet);
                    
                    //set _value
                    std::vector<std::uint8_t> initialInputVector;
                    std::string error;                    
                    
                    if (parse_byte_vector(initValue, initialInputVector, error)){
                        std::cerr << "Initial value converted from vector: \n";
                    }else{
                        std::cerr << "vector conversion failed: " << error << "\n";
                    }
                    set_value(initialInputVector);
                    ////////////////////////////RESPONSE//////////////////////////////////////
                    std::string xmlResponse = buildXMLResponseInit(request_id);
                    send(clientSocket, xmlResponse.c_str(), xmlResponse.size(), 0);
                    logfile << "Responded:\n" << xmlResponse << "\n\n";
                }
                logfile.flush();
            } else {
                std::cerr << "Failed to parse XML.\n";
            }

            if (doc) doc->Release();
        }

        closesocket(clientSocket);
    }

    logfile.close();
    closesocket(listenSocket);
    WSACleanup();
    CoUninitialize();
    return 0;
}
