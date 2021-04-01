/*********************************************************************
 *  @file         AAPConsts.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

 #pragma once

const int AOA_CLASS = 255;
const int AOA_SUBCLASS = 255;
const int AOA_PROTOCOL = 0;

const int AOAP_PRODUCT_MIN = 0x2d00;
const int AOAP_PRODUCT_MAX = 0x2d05;

#define GOOG_VENDOR 0x18d1
#define AOAP_PRODUCT_1 0x2d00
#define AOAP_PRODUCT_2 0x2d01

const int ACCESSORY_STRING_MANUFACTURER = 0;
const int ACCESSORY_STRING_MODEL = 1;
const int ACCESSORY_STRING_DESCRIPTION = 2;
const int ACCESSORY_STRING_VERSION = 3;
const int ACCESSORY_STRING_URI = 4;
const int ACCESSORY_STRING_SERIAL = 5;
const int ACCESSORY_GET_PROTOCOL = 51;
const int ACCESSORY_SEND_STRING = 52;
const int ACCESSORY_START = 53;

const char *ACCESSORY_MANUFACTURER_NAME = "Android";
const char *ACCESSORY_MODEL_NAME = "Android Open Automotive Protocol";
const char *ACCESSORY_DESCRIPTION = "Android Open Automotive Protocol";
const char *ACCESSORY_VERSION = "1.0";
const char *ACCESSORY_URI = "http://www.android.com/";
const char *ACCESSORY_SERIAL_NUMBER = "0000000012345678";

const int HOST_TO_DEVICE_TYPE = 0xc0;
const int DEVICE_TO_HOST_TYPE = 0x40;