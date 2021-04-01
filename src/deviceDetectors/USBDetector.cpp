/*********************************************************************
 *  @file         USBDetector.cpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include "USBDetector.hpp"
#include "log.hpp"

#define SUBSYSTEM "usb"
const char *UDEV_VENDORID_TAG = "idVendor";
const char *UDEV_PRODUCTID_TAG = "idProduct";
const char *UDEV_SERIAL_NUMBER_TAG = "serial";
const char *UDEV_DEVICE_ACTION_ADDED = "add";
const char *UDEV_DEVICE_ACTION_REMOVED = "remove";
const char *UDEV_DEVICE_NUMBER = "devnum";
const char *UDEV_BUS_NUMBER = "busnum";
const char *UDEV_DEVICE_CLASS = "bDeviceClass";
const char *UDEV_DEVICE_SUBCLASS = "bDeviceSubClass";
const char *UDEV_DEVICE_NAME = "manufacturer";
// TODO
// std::string descriptorInformation;

namespace devicehelper
{
namespace detector
{
//#undef LOGUSBMONITOR
#define LOGUSBMONITOR ON

int USBDetector::udev_initialize()
{
#ifdef LOGUSBMONITOR
    LOG_USBMONITOR << "udev Initializing\n";
#endif

    struct udev *udev = udev_new();
    if (!udev)
    {
        #ifdef LOGUSBMONITOR
        LOG_USBMONITOR << stderr << "udev_new() failed\n";
        #endif
        return 1;
    }

    enumerate_devices(udev);
    monitor_devices(udev);

    udev_unref(udev);
    return 0;
}

void USBDetector::print_device(const deviceList::DeviceInfo &deviceInfo)
{
#ifdef LOGUSBMONITOR
    LOG_USBMONITOR << deviceInfo.serialNumber << "\t" << std::hex
                       << deviceInfo.vendorID << "\t" << std::hex
                       << deviceInfo.productID << "\t" << deviceInfo.devicePath
                       << '\n';
#endif

    deviceList::DeviceInfo threadMsg = deviceInfo;
}

void USBDetector::process_device(struct udev_device *dev)
{
    if (dev)
    {
        deviceList::DeviceInfo deviceInfo;

        const char *vendor = udev_device_get_sysattr_value(dev, UDEV_VENDORID_TAG);
        if (vendor)
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "vendor " << vendor << "\n";
            #endif
            std::stringstream strstream;
            strstream << std::hex << vendor;
            strstream >> deviceInfo.vendorID;
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "vendorid empty\n";
            #endif
        }

        const char *product =
            udev_device_get_sysattr_value(dev, UDEV_PRODUCTID_TAG);
        if (product)
        {
            std::stringstream strstream;
            strstream << std::hex << product;
            strstream >> deviceInfo.productID;
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "productid is  empty\n";
            #endif
        }

        const char *serialNumber =
            udev_device_get_sysattr_value(dev, UDEV_SERIAL_NUMBER_TAG);
        if (serialNumber)
        {
            deviceInfo.serialNumber = serialNumber;
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "serial number is empty\n";
            #endif
        }

        const char *action = udev_device_get_action(dev);
        if (action)
        {

            if (0 == strcmp(action, UDEV_DEVICE_ACTION_ADDED))
            {
                #ifdef LOGUSBMONITOR
                LOG_USBMONITOR << "Device detected\n";
                #endif
                deviceInfo.devicestate = deviceList::DeviceState::DETECTION;
            }
            else if (0 == strcmp(action, UDEV_DEVICE_ACTION_REMOVED))
            {
                #ifdef LOGUSBMONITOR
                LOG_USBMONITOR << "Device removed\n";
                #endif
                deviceInfo.devicestate = deviceList::DeviceState::EMPTY;
            }
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "udev_device_get_action returned empty, assuming "
                                  "the device connection\n";
            #endif
            deviceInfo.devicestate = deviceList::DeviceState::DETECTION;
        }

        const char *syspath = udev_device_get_syspath(dev);
        if (syspath)
        {
            deviceInfo.devicePath = syspath;
        }

        const char *deviceNo =
            udev_device_get_sysattr_value(dev, UDEV_DEVICE_NUMBER);
        if (deviceNo)
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "Device number " << deviceNo << "\n";
            #endif
            std::stringstream strstream;
            strstream << deviceNo;
            strstream >> deviceInfo.deviceNo;
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "Device number is empty\n";
            #endif
        }

        const char *busNo =
            udev_device_get_sysattr_value(dev, UDEV_BUS_NUMBER);
        if (busNo)
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "Bus number " << busNo << "\n";
            #endif
            std::stringstream strstream;
            strstream << busNo;
            strstream >> deviceInfo.busNo;
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "Bus number is empty\n";
            #endif
        }

        const char *deviceClass =
            udev_device_get_sysattr_value(dev, UDEV_DEVICE_CLASS);
        if (deviceClass)
        {
            //TODO Add switch case to map enums
            //deviceInfo.usbDeviceClass = deviceClass;
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "Deviceclass "<< deviceClass<<"\n";
            #endif

            using USBDeviceClassCode = devicehelper::deviceList::USBDeviceClassCode;
            std::stringstream strstream;
            strstream << std::hex << deviceClass;
            int32_t usbDeviceClass;
            strstream>>usbDeviceClass;

            deviceInfo.usbDeviceClass = (USBDeviceClassCode) usbDeviceClass;

            // Blacklist the Hubs from being displayed
            if(deviceInfo.usbDeviceClass == USBDeviceClassCode::HUB)
                return;
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "Deviceclass is empty\n";
            #endif
        }


        const char *deviceSubClass =
            udev_device_get_sysattr_value(dev, UDEV_DEVICE_SUBCLASS);
        if (deviceSubClass)
        {
            //TODO Add switch case to map enums
            //deviceInfo.usbDeviceSubClass = deviceSubClass;
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "DeviceSubClass "<< deviceSubClass<<"\n";
            #endif
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "DeviceSubClass is empty\n";
            #endif
        }

        const char *devType = udev_device_get_devtype(dev);
        if (devType)
        {
            //deviceInfo.deviceType = (int)devType;
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "DevType "<< devType<<"\n";
            #endif
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "DeviceType is empty\n";
            #endif
        }

        const char *devName = udev_device_get_sysattr_value(dev, UDEV_DEVICE_NAME);
        if (devName)
        {
            deviceInfo.mediumName = devName;
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "Device Name "<< devName<<"\n";
            #endif
        }
        else
        {
            #ifdef LOGUSBMONITOR
            LOG_USBMONITOR << "Device Name is empty\n";
            #endif
        }
        print_device(deviceInfo);

        udev_device_unref(dev);
    }
}

void USBDetector::enumerate_devices(struct udev *udev)
{
    struct udev_enumerate *enumerate = udev_enumerate_new(udev);

    udev_enumerate_add_match_subsystem(enumerate, SUBSYSTEM);
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    udev_list_entry_foreach(entry, devices)
    {
        const char *path = udev_list_entry_get_name(entry);
        struct udev_device *dev = udev_device_new_from_syspath(udev, path);
        process_device(dev);
    }

    udev_enumerate_unref(enumerate);
    #ifdef LOGUSBMONITOR
    LOG_USBMONITOR << "enumerated\n";
    #endif
}

void USBDetector::monitor_devices(struct udev *udev)
{
    struct udev_monitor *mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, SUBSYSTEM, NULL);
    udev_monitor_enable_receiving(mon);
    int fd = udev_monitor_get_fd(mon);

    while (1)
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        int ret = select(fd + 1, &fds, NULL, NULL, NULL);
        if (ret <= 0)
            break;

        if (FD_ISSET(fd, &fds))
        {
            struct udev_device *dev = udev_monitor_receive_device(mon);
            process_device(dev);
        }
    }
}

} // namespace detector
} // namespace devicemanager