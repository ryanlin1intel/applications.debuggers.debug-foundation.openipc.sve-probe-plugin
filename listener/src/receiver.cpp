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
#include "shift.h"
#include "utils.h"

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
