/*********************************************************************
 *  @file         run.cpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#include <iostream>
#include <memory>
#include <thread>
#include "external_glib_headers.hpp"
#include "log.hpp"
#include "run.hpp"
#include "USBDetector.hpp"
#include "MessageQueue.hpp"
#include "DeviceEventLoop.hpp"
namespace devicehelper
{

void run()
{
    LOG_GENERAL << "Starting devicehelper\n";
    auto detector = std::make_shared<detector::USBDetector>();
    auto messagequeue = std::make_shared<MessageQueue>();
    auto deviceEventloop = std::make_shared<DeviceEventLoop>();
    DevicehelperServer deviceHelperServer(detector, messagequeue, deviceEventloop);

    LOG_GENERAL << "Starting USB monitoring\n";
    deviceHelperServer.startUSBMonitor();

    LOG_GENERAL << "Waiting for requests...\n";
    auto mainLoopPtr = std::unique_ptr<GMainLoop, decltype(&g_main_loop_unref)>{
        g_main_loop_new(nullptr, FALSE), g_main_loop_unref};

    g_main_loop_run(mainLoopPtr.get());
}

} // namespace devicehelper