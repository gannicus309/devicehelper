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

FillDeviceDetailsErrorCodes LibUsbWrapper::fillDeviceDetails(DeviceInfo &deviceInfo)
{
    if(initDevice(deviceInfo.vendorID , deviceInfo.productID))
    {
        deviceInfo.usbDeviceSubClass = getUsbDeviceSubClass();
        libusb_close(mDeviceHandle);
            mDeviceHandle = nullptr;
        return FillDeviceDetailsErrorCodes::FILL_DEVICE_SUCCESSFUL;
    }
    else
    {
        return FillDeviceDetailsErrorCodes::DEVICE_OPEN_FAILED;
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

uint8_t LibUsbWrapper::getUsbDeviceSubClass() {
  if (!mUsbContext) {
    #ifdef LOGLIBUSBHELPER
    DBG_MSG_LIBUSBHELPER << "Failed to initialize libusb\n";
    #endif
    return 1;
  }
  int32_t ubsDeviceSubClass = LIBUSB_CLASS_HID;
  if (mDeviceHandle) {
    libusb_device *device = libusb_get_device(mDeviceHandle);
    if (libusb_open(device, &mDeviceHandle) == LIBUSB_SUCCESS) {
      libusb_device_descriptor deviceDesc;
      int ret = libusb_get_device_descriptor(device, &deviceDesc);
      if (!ret) {
        #ifdef LOGLIBUSBHELPER
        DBG_MSG_LIBUSBHELPER << "deviceDesc.bDeviceSubClass "
                             << deviceDesc.bDeviceSubClass << "\n";
        #endif
        ubsDeviceSubClass = deviceDesc.bDeviceSubClass;
      } else {
        #ifdef LOGLIBUSBHELPER
        DBG_MSG_LIBUSBHELPER << "deviceDesc.bDeviceClass Failed\n";
        #endif
      }
    } else {
      #ifdef LOGLIBUSBHELPER
      DBG_MSG_LIBUSBHELPER << "Not able to get libusb_device\n";
      #endif
    }
  } else {
    #ifdef LOGLIBUSBHELPER
    DBG_MSG_LIBUSBHELPER << "mDeviceHandle is NULL\n";
    #endif
  }
  return ubsDeviceSubClass;
}

static int parseInterfaces(libusb_device *dev, uint8_t *ifnum,
                           uint8_t *readEndpoint, uint8_t *writeEndpoint) {
  #ifdef LOGLIBUSBHELPER
  DBG_MSG_LIBUSBHELPER << "Parse Interfaces \n";
  #endif
  int ret = LIBUSB_ERROR_OTHER;
  libusb_config_descriptor *cdesc;
  libusb_get_active_config_descriptor(dev, &cdesc);

  // Each device has multiple interfaces, loop through them.
  for (int j = 0; j < cdesc->bNumInterfaces; ++j) {
    const libusb_interface *intf = &cdesc->interface[j];

    // Each interface has a bunch of alternate settings.
    for (int k = 0; k < intf->num_altsetting; ++k) {
      const libusb_interface_descriptor *idesc = &intf->altsetting[k];

      // Each alternate setting has endpoints.
      for (int l = 0; l < idesc->bNumEndpoints; ++l) {
        const libusb_endpoint_descriptor *edesc = &idesc->endpoint[l];

        #ifdef LOGLIBUSBHELPER
        DBG_MSG_LIBUSBHELPER << "idesc->bInterfaceClass :"<<idesc->bInterfaceClass <<"\n";
        DBG_MSG_LIBUSBHELPER << "idesc->bInterfaceSubClass :"<<idesc->bInterfaceSubClass <<"\n";
        DBG_MSG_LIBUSBHELPER << "idesc->bInterfaceProtocol :"<<idesc->bInterfaceProtocol <<"\n";
        #endif
        // We have a match for the AOA protocol.
        if (idesc->bInterfaceClass == AOA_CLASS &&
            idesc->bInterfaceSubClass == AOA_SUBCLASS &&
            idesc->bInterfaceProtocol == AOA_PROTOCOL) {
          #ifdef LOGLIBUSBHELPER
          DBG_MSG_LIBUSBHELPER << "Device is Android auto class \n";
          #endif
          ret = LIBUSB_SUCCESS;
          *ifnum = j;
          if ((LIBUSB_ENDPOINT_IN & edesc->bEndpointAddress) != 0) {
            *readEndpoint = edesc->bEndpointAddress;
          } else {
            *writeEndpoint = edesc->bEndpointAddress;
          }
        }
      }
    }
  }

  libusb_free_config_descriptor(cdesc);
  return ret;
}

static int sendString(libusb_device_handle *handle, const char *str,
                      int index) {
  return libusb_control_transfer(handle, DEVICE_TO_HOST_TYPE,
                                 ACCESSORY_SEND_STRING, 0, index,
                                 (uint8_t *)str, strlen(str) + 1, 0);
}

/* Set the device to accessory mode */
int LibUsbWrapper::startAccessoryMode() {
  #ifdef LOGLIBUSBHELPER
  DBG_MSG_LIBUSBHELPER << "StartAccesoryMode\n";
  #endif
  uint8_t buffer[2];
    if(!mDeviceHandle){
    #ifdef LOGLIBUSBHELPER
    DBG_MSG_LIBUSBHELPER << "mDeviceHandle empty\n";
    #endif
    return 0; 
  }
  int ret = libusb_control_transfer(mDeviceHandle, HOST_TO_DEVICE_TYPE,
                                    ACCESSORY_GET_PROTOCOL, 0, 0,
                                    (uint8_t *)buffer, 2, 0);
  ret = sendString(mDeviceHandle, ACCESSORY_MANUFACTURER_NAME,
                   ACCESSORY_STRING_MANUFACTURER);
  ret = sendString(mDeviceHandle, ACCESSORY_MODEL_NAME, ACCESSORY_STRING_MODEL);
  ret = sendString(mDeviceHandle, ACCESSORY_DESCRIPTION,
                   ACCESSORY_STRING_DESCRIPTION);
  ret = sendString(mDeviceHandle, ACCESSORY_VERSION, ACCESSORY_STRING_VERSION);
  ret = sendString(mDeviceHandle, ACCESSORY_URI, ACCESSORY_STRING_URI);
  ret = sendString(mDeviceHandle, ACCESSORY_SERIAL_NUMBER,
                   ACCESSORY_STRING_SERIAL);
  ret = libusb_control_transfer(mDeviceHandle, DEVICE_TO_HOST_TYPE,
                                ACCESSORY_START, 0, 0, NULL, 0, 0);
  return ret;
}

/* Checks to see if the given device is valid (a Nexus phone), and switches it
   to accessory mode if needed. Then, sets the configuration and claims the
   interface. Returns true if the accessory if the device is valid, false if it
   is not */
ChangeProfileErrorCodes LibUsbWrapper::changeDeviceProfile(int32_t vendorID, int32_t productID) {

  libusb_device_descriptor desc;
  if(!initDevice(vendorID , productID))
      return ChangeProfileErrorCodes::DEVICE_OPEN_FAILED;
  if (!mDeviceHandle) {
      #ifdef LOGLIBUSBHELPER
      DBG_MSG_LIBUSBHELPER << "Not able to open the device\n";
      #endif
      return ChangeProfileErrorCodes::NOT_IN_ACCESSORY;
    } else {
      #ifdef LOGLIBUSBHELPER
      DBG_MSG_LIBUSBHELPER << "libusb_open_device is successful\n";
      #endif
    }
    #ifdef LOGLIBUSBHELPER
    DBG_MSG_LIBUSBHELPER << "Successfully opened device.\n";
    #endif

     libusb_device *device = libusb_get_device(mDeviceHandle);
     if(!device)
      return ChangeProfileErrorCodes::NOT_IN_ACCESSORY;  

    libusb_get_device_descriptor(device, &desc);

    if (!(desc.idProduct >= AOAP_PRODUCT_MIN && desc.idProduct <= AOAP_PRODUCT_MAX)) {
      #ifdef LOGLIBUSBHELPER
      DBG_MSG_LIBUSBHELPER
          << "Found Google device not in accessory mode. Trying to turn on."
          << '\n';
      #endif
      startAccessoryMode();
      libusb_reset_device(mDeviceHandle);
      return ChangeProfileErrorCodes::NOT_IN_ACCESSORY;
    }
    #ifdef LOGLIBUSBHELPER
    DBG_MSG_LIBUSBHELPER << "Valid device found.\n";
    #endif

    // find the endpoints and interface we need
    if (parseInterfaces(device, &mInterface, &mReadEndpoint, &mWriteEndpoint) !=
        LIBUSB_SUCCESS) {
      #ifdef LOGLIBUSBHELPER
      DBG_MSG_LIBUSBHELPER << "Failed to parse interface." << '\n';
      #endif
      return ChangeProfileErrorCodes::ACCESSORY_PARSING_FAILED;
    }
#ifndef __APPLE__
    if (libusb_set_auto_detach_kernel_driver(mDeviceHandle, 1) !=
                  LIBUSB_SUCCESS) {
      #ifdef LOGLIBUSBHELPER
      DBG_MSG_LIBUSBHELPER << "Failed to detach kernel driver, error: "
                           << libusb_error_name(err) << '\n';
      #endif
      return ChangeProfileErrorCodes::KERNEL_DRIVER_DETACH_FAILED;
    }
    #ifdef LOGLIBUSBHELPER
    DBG_MSG_LIBUSBHELPER << "Kernel driver detached successfully.\n";
    #endif
#endif
    if (libusb_set_configuration(mDeviceHandle, 1) != LIBUSB_SUCCESS) {
      #ifdef LOGLIBUSBHELPER
      DBG_MSG_LIBUSBHELPER << "Failed to set configuration, error: "
                           << libusb_error_name(err) << '\n';
      #endif
      return ChangeProfileErrorCodes::SET_CONFIGURATION_FAILED;
    }
    #ifdef LOGLIBUSBHELPER
    DBG_MSG_LIBUSBHELPER << "Set configuration successfully.\n";
    #endif
    if (libusb_claim_interface(mDeviceHandle, mInterface) !=
                  LIBUSB_SUCCESS) {
      #ifdef LOGLIBUSBHELPER
      DBG_MSG_LIBUSBHELPER << "Failed to claim interface, error: "
                           << libusb_error_name(err) << '\n';
      #endif
      return ChangeProfileErrorCodes::CLAIM_INTERFACE_FAILED;
    }
    #ifdef LOGLIBUSBHELPER
    DBG_MSG_LIBUSBHELPER << "Claimed interface successfully.\n";
    #endif

    // Reset for good measure.
    libusb_reset_device(mDeviceHandle);
    mDeviceHandle = nullptr;
    
    return ChangeProfileErrorCodes::CHANGE_PROFILE_SUCCESS;
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
