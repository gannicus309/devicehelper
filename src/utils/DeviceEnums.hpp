/*********************************************************************
 *  @file         DeviceEnums.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once

namespace devicehelper {
namespace deviceList {

enum class DeviceType
{
    /**
     * Unknown device type or device type not yet recognized
    */
    UNKNOWN = 0,
    /**
     * Mass storage device type
        */
    MASS_STORAGE = 1,
    /**
     * MTP type
        */
    MTP_DEVICE = 2,
    /**
     * iAP device type. Used for apple devices.
        */
    IAP_DEVICE = 3,
    /**
     * network type
        */
    NETWORK_DEVICE = 4,
    /**
     * Mirror Link device type.
        */
    MLINK_DEVICE = 5
};


/**
 * DeviceState enumeration contains all supported devices states.
*/
enum class DeviceState
{
    /**
     * Unknown device state
        */
    UNKNOWN = 0,
    /**
     * Device is empty. No device connected
        */
    EMPTY = 1,
    /**
     * New device detection and recognition is in progress
        */
    DETECTION = 2,
    /**
     * Connected device is not supported
        */
    NOT_SUPPORTED = 3,
    /**
     * Connected device is ready
        */
    READY = 4,
    /**
     * Error occured
        */
    DEVICE_ERROR = 5,
    /**
     * Profile change is ongoinig
        */
    PROFILE_CHANGE = 6
};

enum class ProfileType
{
    /**
     * Interface profile
    */
    PER_INTERFACE = 0,
    /**
     * Audio profile
        */
    AUDIO = 1,
    /**
     * Comm profile
        */
    COMM = 2,
    /**
     * Hid profile
        */
    HID = 3,
    /**
     * Ptp profile
        */
    PTP = 6,
    /**
     * Printer profile
        */
    PRINTER = 7,
    /**
     * Mass storage profile
        */
    MASS_STORAGE = 8,
    /**
     * Hub profile
        */
    HUB = 9,
    /**
     * Data profile
        */
    DATA = 10,
    /**
     * Android Auto RemoteUI protocol.
        */
    AOAP = 11,
    /**
     * Apple Car Play RemoteUI protocol.
        */
    CARPLAY = 12,
    /**
     * Mirrorlink RemoteUI protocol.
        */
    MIRRORLINK = 13,
    /**
     * CarLife RemoteUI protocol.
        */
    CARLIFE = 14,
    /**
     * Vendor spec profile
        */
    VENDOR_SPEC = 255
};


/**
 * USB device class enumeration
    */
enum class USBDeviceClassCode
{
    /**
     * Unknown device class
        */
    UNKNOWN = -1,
    /**
     * Class code defined in interfaces
        */
    FROM_INTERFACE = 0,
    /**
     * Communication and CDC Control device class
        */
    COMMUNICATION = 2,
    /**
     * HUB device class
        */
    HUB = 9,
    /**
     * Billboard device class
        */
    BILLBOARD = 17,
    /**
     * Diagnostic device class
        */
    DIAGNOSTIC = 220,
    /**
     * Miscellaneous device class
        */
    MISCELLANEOUS = 239,
    /**
     * Vendor specific device class
        */
    VENDOR_SPECIFIC = 255
};

/**
 * Device enumeration contains all supported devices. Can be used to identify a specific device.
    */
enum class Device
{
    UNKNOWN = 0,
    SD = 1,
    USB1 = 2,
    USB2 = 3
};
}
}