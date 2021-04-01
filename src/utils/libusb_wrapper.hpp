/*********************************************************************
 *  @file         libusb_wrapper.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once

#include <libusb.h>
#include <DeviceInfo.hpp>
#include "AAPConsts.hpp"
namespace devicehelper {
namespace deviceList
{
    
enum class ChangeProfileErrorCodes
{
    CHANGE_PROFILE_SUCCESS = 0,
    DEVICE_OPEN_FAILED,
    NOT_IN_ACCESSORY,    
    ACCESSORY_PARSING_FAILED,
    KERNEL_DRIVER_DETACH_FAILED,
    SET_CONFIGURATION_FAILED,
    CLAIM_INTERFACE_FAILED
};

enum class FillDeviceDetailsErrorCodes
{
    FILL_DEVICE_SUCCESSFUL,
    DEVICE_OPEN_FAILED
};

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

    FillDeviceDetailsErrorCodes fillDeviceDetails(DeviceInfo &deviceInfo);
    ChangeProfileErrorCodes changeDeviceProfile(int32_t vendorID, int32_t productID);
private:
    libusb_context* mUsbContext = nullptr;
    libusb_device_handle* mDeviceHandle = nullptr;
    uint8_t mInterface;
    uint8_t mReadEndpoint;
    uint8_t mWriteEndpoint;
    bool is_Intialized;
    bool initDevice(int32_t vendorID , int32_t productID);
    int startAccessoryMode();
    uint8_t getUsbDeviceSubClass();
    uint8_t getUsbBusNumber() const;
};
} // namespace devicelist
}// namespace devicehelper

