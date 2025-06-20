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
//build cmd: cl transmitter.cpp /Fe:transmitter.exe /EHsc ws2_32.lib
//test transmitter app for openipc remote plugin development. 



#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

std::string getCurrentEpochMillisUTC() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return std::to_string(ms);
}

std::string buildXMLRequest(const std::string& request_id, const std::string& payload) {
    std::ostringstream oss;
    oss << "<request>"
        << "<request_id>" << request_id << "</request_id>"
        << "<payload>" << payload << "</payload>"
        << "</request>";
    return oss.str();
}

int main() {
    std::string payload = "test-message";
    std::string request_id = getCurrentEpochMillisUTC();
    std::string xml = buildXMLRequest(request_id, payload);

    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in serverAddr{};
    char buffer[2048];

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to receiver IP

    if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    send(sock, xml.c_str(), xml.size(), 0);

    int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Receiver response:\n" << buffer << "\n";
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
