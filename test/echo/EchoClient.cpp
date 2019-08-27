#include <iostream>
#include <string>
#include <algorithm> // for std::find
#include "CppNet.h"
#include "Runnable.h"
#include "Socket.h"
using namespace cppnet;

int index = 0;
std::vector<Handle> handle_vec;
static const char* __buf_spilt = "\r\n";

std::string GetMsg() {
    return "It is a test msg, It is a long test msg. index : " + std::to_string(index) + __buf_spilt;
}

void WriteFunc(const Handle& handle, uint32_t len, uint32_t error) {
    if (error != CEC_SUCCESS) {
        std::cout << "something err while write : " << error << std::endl;
    }
    // do nothing 
}

void ReadFunc(const Handle& handle, base::CBuffer* data, uint32_t len, uint32_t error, bool& continue_read) {
    if (error != CEC_SUCCESS) {
        std::cout << "something err while read : " << error << std::endl;
    } else {
        // print
	    std::cout << *(data) << std::endl;
        data->Clear();
    }
}

void ConnectFunc(const Handle& handle, uint32_t error) {
    if (error != CEC_SUCCESS) {
        std::cout << "something err while connect : " << error << std::endl;
    } else {
        handle_vec.push_back(handle);
    }
}

void DisConnectionFunc(const Handle& handle, uint32_t err) {
    std::cout << "disconnect : " << handle << std::endl;
    auto iter = std::find(handle_vec.begin(), handle_vec.end(), handle);
    if (iter != handle_vec.end()) {
        handle_vec.erase(iter);
    }
}

int main() {

	cppnet::Init(1, true);

    cppnet::SetConnectionCallback(ConnectFunc);
    cppnet::SetWriteCallback(WriteFunc);
    cppnet::SetReadCallback(ReadFunc);
    cppnet::SetDisconnectionCallback(DisConnectionFunc);
    for (size_t i = 0; i < 1; i++) {
#ifndef __linux__
        std::string msg = GetMsg();
        cppnet::SyncConnection("192.168.1.4", 8921, msg.c_str(), msg.length());
#else
        cppnet::SyncConnection("172.21.193.122", 8921);
#endif // !__linux__
    }

    while (1) {
        // sleep 1s;
        base::CRunnable::Sleep(1000);
        for (auto iter = handle_vec.begin(); iter != handle_vec.end(); ++iter) {
            std::string msg = GetMsg();
            cppnet::SyncWrite(*iter, msg.c_str(), msg.length());
        }
    }
    
    cppnet::Join();
}