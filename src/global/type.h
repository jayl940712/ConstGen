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

struct MosPair
{
    IndexType mosId1, mosId2;
    bool valid = true;
    MosPair(IndexType Id1, IndexType Id2) 
        : mosId1(Id1), mosId2(Id2) 
    {}
    inline int operator == (const MosPair & right) const
    {
        if (mosId1 == right.mosId1 && mosId2 == right.mosId2)
            return 1;
        if (mosId1 == right.mosId2 && mosId2 == right.mosId1)
            return 1;
        return 0;
    }
};

enum class InstType : Byte
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

enum class MosType: Byte
{
    DIFF,
    DIODE,
    CAP,
    DUMMY
};

enum class MosPattern: Byte
{
    DIFF_SOURCE,
    DIFF_CASCODE,
    CASCODE,
    LOAD,
    CROSS_CASCODE,
    CROSS_LOAD,
    INVALID
};
    
PROJECT_NAMESPACE_END

#endif
