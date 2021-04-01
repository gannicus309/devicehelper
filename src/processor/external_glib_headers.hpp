/*********************************************************************
 *  @file         external_glib_headers.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once

#ifdef __GNUC__
#ifndef __clang__ // clang also defines __GNUC__

// Remember flags state
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wuseless-cast"

#endif // !__clang__
#endif // __GNUC__

#include <glib.h>

#ifdef __GNUC__
#ifndef __clang__ // clang also defines __GNUC__

// Restore flags state
#pragma GCC diagnostic pop

#endif // !__clang__
#endif // __GNUC__
