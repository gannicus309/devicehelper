/*********************************************************************
 *  @file         run.cpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#include <iostream>
#include <memory>
#include <thread>
#include "log.hpp"
#include "run.hpp"
#include "usbdetector.hpp"

namespace devicehelper
{

void run()
{
    LOG_GENERAL << "Starting devicehelper\n";
    auto detector = std::make_shared<detector::USBDetector>();
}

} // namespace devicehelper