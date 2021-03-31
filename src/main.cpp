/*********************************************************************
 *  @file         main.cpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#include <iostream>
#include <exception>
#include "run.hpp"
#include <memory>
#include "log.hpp"

int main()
{
    try {
        LOG_GENERAL << "DeviceHelper starting.\n";
        devicehelper::run();
    } catch (std::exception& e) {
        LOG_GENERAL << "DeviceHelper: unexpected exception: " << e.what() <<"\n";
        return 1;
    }
    LOG_GENERAL << "DeviceHelper: Exiting application.\n";
    return 0;
}
