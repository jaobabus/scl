#pragma once

#include <stdbool.h>
#include <iso646.h>


#ifdef SCL_SINGLE_HEADER
#define SCL_EXPORT_API_BEFORE_TYPE inline
#define SCL_EXPORT_API_AFTER_TYPE
#define SCL_STATIC_API_BEFORE_TYPE inline
#define SCL_CPP_ANON_NAMESPACE_START namespace {
#define SCL_CPP_ANON_NAMESPACE_END }
#endif

#ifndef SCL_EXPORT_API_BEFORE_TYPE
#define SCL_EXPORT_API_BEFORE_TYPE inline
#define SCL_EXPORT_API_AFTER_TYPE
#define SCL_STATIC_API_BEFORE_TYPE static
#define SCL_CPP_ANON_NAMESPACE_START
#define SCL_CPP_ANON_NAMESPACE_END
#endif
