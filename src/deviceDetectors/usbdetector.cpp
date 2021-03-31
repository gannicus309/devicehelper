/*********************************************************************
 *  * ----------------------------------------------------------------------
 *  @file         usbdetector.cpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include "usbdetector.hpp"
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
#undef LOGUSBMONITOR

int USBDetector::udev_initialize()
{
#ifdef LOGUSBMONITOR
    DBG_MSG_USBMONITOR << "udev Initializing\n";
#endif

    struct udev *udev = udev_new();
    if (!udev)
    {
        #ifdef LOGUSBMONITOR
        DBG_MSG_USBMONITOR << stderr << "udev_new() failed\n";
        #endif
        return 1;
    }

    enumerate_devices(udev);
    monitor_devices(udev);

    udev_unref(udev);
    return 0;
}

void USBDetector::print_device()
{
}

void USBDetector::process_device(struct udev_device *dev)
{

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
    DBG_MSG_USBMONITOR << "enumerated\n";
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