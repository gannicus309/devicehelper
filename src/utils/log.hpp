/*********************************************************************
 *  @file         log.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

/* Add DLT logging here */
#include <iostream>
#define LOG_GENERAL std::cout<<"[GENERAL] "<<__FUNCTION__ << "[" << __LINE__ << "] "
#define DBG_MSG_USBMONITOR std::cout<<"[USBMONITOR] "<<__FUNCTION__ << "[" << __LINE__ << "] "