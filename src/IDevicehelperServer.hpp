#pragma once

#include <vector>
#include "DeviceInfo.hpp"
namespace devicehelper {

class IDevicehelperServer
{
public:
    virtual ~IDevicehelperServer() = default;

    virtual std::vector<deviceList::DeviceInfo> getDeviceInfoList() = 0;
    virtual void DevicelistChanged() = 0;
    virtual void changeDeviceProfile(const int32_t deviceID,const deviceList::ProfileType profile) = 0;
protected:
    IDevicehelperServer() = default;
    IDevicehelperServer(IDevicehelperServer&&) = default;
    IDevicehelperServer(const IDevicehelperServer&) = default;
    IDevicehelperServer& operator=(const IDevicehelperServer&) = default;
    IDevicehelperServer& operator=(IDevicehelperServer&&) = default;

};

} // namespace devicehelper
