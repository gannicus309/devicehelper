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

namespace devicehelper
{

void run()
{
    LOG_GENERAL << "Starting devicehelper\n";
    auto detector = std::make_shared<detector::USBDetector>();
    detector->udev_initialize();
    LOG_GENERAL << "Waiting for requests...\n";
    auto mainLoopPtr = std::unique_ptr<GMainLoop, decltype(&g_main_loop_unref)>{
        g_main_loop_new(nullptr, FALSE), g_main_loop_unref};

    g_main_loop_run(mainLoopPtr.get());
}

} // namespace devicehelper