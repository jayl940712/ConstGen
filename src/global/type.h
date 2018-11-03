#ifndef __TYPE_H__
#define __TYPE_H__

#include <cstdint>
#include <set>
#include <string>
#include "global/global.h"

PROJECT_NAMESPACE_BEGIN

using IndexType     = std::uint32_t;
using IntType       = std::int32_t;
using RealType      = double;
using Byte          = std::uint8_t;

constexpr IndexType     INDEX_TYPE_MAX          = 1000000000;
constexpr IntType       INT_TYPE_MAX            = 1000000000;
constexpr IntType       INT_TYPE_MIN            = -1000000000;
constexpr RealType      REAL_TYPE_MAX           = 1e100;
constexpr RealType      REAL_TYPE_MIN           = -1e100;
constexpr RealType      REAL_TYPE_TOL           = 1e-6; 

enum class InstanceType : Byte
{
    RES,
    PMOS,
    NMOS,
    CAP,
    OTHER
};

enum class NetType : Byte
{ 
    POWER,
    GROUND,
    SIGNAL 
};

enum class PinType : Byte
{
    SOURCE,
    DRAIN,
    GATE,
    BULK,
    THIS,
    THAT,
    OTHER
};    

PROJECT_NAMESPACE_END

#endif
