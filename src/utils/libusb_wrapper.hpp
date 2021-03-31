/*********************************************************************
 *  @file         libusb_wrapper.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once

#include <libusb.h>
#include <deviceinfo.hpp>

namespace devicehelper {
namespace deviceList
{

class LibUsbWrapper 
{    
public:
    LibUsbWrapper();

    LibUsbWrapper(LibUsbWrapper &&) = delete;
    LibUsbWrapper(const LibUsbWrapper &) = default;
    LibUsbWrapper &
    operator=(const LibUsbWrapper &) = delete;
    LibUsbWrapper &operator=(LibUsbWrapper &&) = delete;

    ~LibUsbWrapper();
private:
    libusb_context* mUsbContext = nullptr;
    libusb_device_handle* mDeviceHandle = nullptr;
    uint8_t mInterface;
    uint8_t mReadEndpoint;
    uint8_t mWriteEndpoint;
    bool is_Intialized;
    bool initDevice(int32_t vendorID , int32_t productID);
};
} // namespace devicelist
}// namespace devicehelper

