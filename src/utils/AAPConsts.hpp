/*********************************************************************
 *  @file         AAPConsts.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once

extern const int AOA_CLASS;
extern const int AOA_SUBCLASS;
extern const int AOA_PROTOCOL;

extern const int AOAP_PRODUCT_MIN;
extern const int AOAP_PRODUCT_MAX;

#ifndef GOOG_VENDOR
#define GOOG_VENDOR 0x18d1
#endif
#ifndef AOAP_PRODUCT_1
#define AOAP_PRODUCT_1 0x2d00
#endif
#ifndef AOAP_PRODUCT_2
#define AOAP_PRODUCT_2 0x2d01
#endif

extern const int ACCESSORY_STRING_MANUFACTURER ;
extern const int ACCESSORY_STRING_MODEL;
extern const int ACCESSORY_STRING_DESCRIPTION;
extern const int ACCESSORY_STRING_VERSION;
extern const int ACCESSORY_STRING_URI;
extern const int ACCESSORY_STRING_SERIAL;
extern const int ACCESSORY_GET_PROTOCOL;
extern const int ACCESSORY_SEND_STRING;
extern const int ACCESSORY_START;

extern const char *ACCESSORY_MANUFACTURER_NAME;
extern const char *ACCESSORY_MODEL_NAME;
extern const char *ACCESSORY_DESCRIPTION;
extern const char *ACCESSORY_VERSION;
extern const char *ACCESSORY_URI;
extern const char *ACCESSORY_SERIAL_NUMBER;

extern const int HOST_TO_DEVICE_TYPE;
extern const int DEVICE_TO_HOST_TYPE;