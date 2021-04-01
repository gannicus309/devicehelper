/*********************************************************************
 *  @file         libusb_wrapper.cpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#include "log.hpp"
#include <cstring>
#include <iostream>
#include <libusb_wrapper.hpp>


namespace devicehelper {
namespace deviceList {

#define DEBUG 1
#undef LOGLIBUSBHELPER

LibUsbWrapper::LibUsbWrapper() {
  int ret = libusb_init(&mUsbContext);
  if (ret != LIBUSB_SUCCESS) {
    #ifdef LOGLIBUSBHELPER
    LOG_LIBUSBHELPER << "Failed to initialize libusb\n";
    #endif
#ifdef DEBUG
    libusb_set_option(mUsbContext, LIBUSB_OPTION_LOG_LEVEL);
#endif
  } else {
    #ifdef LOGLIBUSBHELPER
    LOG_LIBUSBHELPER << "libusb_init successful\n";
    #endif
  }
}

bool LibUsbWrapper::initDevice(int32_t vendorID, int32_t productID) {

  is_Intialized = false;
  if (!mUsbContext) {
    #ifdef LOGLIBUSBHELPER
    LOG_LIBUSBHELPER << "Failed to initialize libusb\n";
    #endif
    return is_Intialized;
  }
  if (mDeviceHandle) {
    libusb_close(mDeviceHandle);
    mDeviceHandle = nullptr;
  }
  if (!mUsbContext) {
    #ifdef LOGLIBUSBHELPER
    LOG_LIBUSBHELPER << "Failed to initialize libusb\n";
    #endif
    return is_Intialized;
  }

 // discover devices
  libusb_device **list;
  libusb_device *found = NULL;
  ssize_t cnt = libusb_get_device_list(NULL, &list);
  ssize_t i = 0;
  int err = 0;
  if (cnt < 0)
  {
    #ifdef LOGLIBUSBHELPER
    LOG_LIBUSBHELPER << "No Device found\n";
    #endif
    return false;
  }
      
  for (i = 0; i < cnt; i++) {
      libusb_device *device = list[i];
      libusb_device_descriptor desc;
      libusb_get_device_descriptor(device, &desc);  
      if ((desc.idVendor == vendorID)&&(desc.idProduct == productID)) {
          #ifdef LOGLIBUSBHELPER
          LOG_LIBUSBHELPER << "Device found\n";
          #endif
          found = device;
          break;
      }
  }
  if (found) {
      err = libusb_open(found, &mDeviceHandle);
      if (err || !mDeviceHandle) {
        #ifdef LOGLIBUSBHELPER
        LOG_LIBUSBHELPER << "Not able to open the device, error:"<< libusb_error_name(err) <<"\n";
        #endif
      }
      else {
      is_Intialized = true;
      #ifdef LOGLIBUSBHELPER
      LOG_LIBUSBHELPER << "libusb_open_device is successful\n";
      #endif
      }
    }
    else
    {
      #ifdef LOGLIBUSBHELPER
      LOG_LIBUSBHELPER << "no device found\n";
      #endif
    }
          
  libusb_free_device_list(list, 1);
  return is_Intialized;
}

LibUsbWrapper::~LibUsbWrapper() {
  #ifdef LOGLIBUSBHELPER
  LOG_LIBUSBHELPER << "Destructor Called\n";
  #endif
  if (mDeviceHandle) {
    libusb_close(mDeviceHandle);
    mDeviceHandle = nullptr;
  }
  if (mUsbContext) {
    libusb_exit(mUsbContext);
    mUsbContext = nullptr;
  }
}


} // namespace deviceList
} // namespace devicehelper
