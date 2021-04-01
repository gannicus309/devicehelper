/*********************************************************************
 *  @file         log.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

/* Add DLT logging here */
#include <iostream>
#define LOG_GENERAL std::cout<<"[GENERAL] "<<__FUNCTION__ << "[" << __LINE__ << "] "
#define LOG_USBMONITOR std::cout<<"[USBMONITOR] "<<__FUNCTION__ << "[" << __LINE__ << "] "
#define LOG_LIBUSBHELPER std::cout<<"[LIBUSBHELPER] "<<__FUNCTION__ << "[" << __LINE__ << "] "
#define LOG_DEVICELIST std::cout<<"[DEVICELIST] "<<__FUNCTION__ << "[" << __LINE__ << "] "