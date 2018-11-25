/*! @file global/type.h
    @brief Type header file.
    @author Mingjie Liu
    @date 11/24/2018
*/
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

/*! @struct MosPair
    @brief A pair of id for Inst.
*/
struct MosPair
{
    IndexType mosId1; /*!< Id1 of Inst. */
    IndexType mosId2; /*!< Id2 of Inst. */
    bool valid = true; /*!< Indicating if valid search pairs. */
/*! Constructor */
    MosPair(IndexType Id1, IndexType Id2) 
        : mosId1(Id1), mosId2(Id2) 
    {}
/*! Equal operator. 

    Two pairs are equal if all Id are equal. 
    Sequence does not matter. 
*/
    inline int operator == (const MosPair & right) const
    {
        if (mosId1 == right.mosId1 && mosId2 == right.mosId2)
            return 1;
        if (mosId1 == right.mosId2 && mosId2 == right.mosId1)
            return 1;
        return 0;
    }
};

/*! @enum InstType
    @brief Type of Inst.
*/
enum class InstType : Byte
{
    RES, /*!< Resistor */
    PMOS, /*!< PMos */
    NMOS, /*!< NMos */
    CAP, /*!< Capacitor */
    OTHER /*!< Other */
};

/*! @enum NetType
    @brief Type of Net.
*/
enum class NetType : Byte
{ 
    POWER, /*!< Power */
    GROUND, /*!< Ground */
    SIGNAL /*!< Signal */
};

/*! @enum PinType
    @brief Type of Pin.
*/
enum class PinType : Byte
{
    SOURCE, /*!< Inst is Mosfet*/
    DRAIN, /*!< Inst is Mosfet*/
    GATE, /*!< Inst is Mosfet*/
    BULK, /*!< Inst is Mosfet*/
    THIS, /*!< Inst is Passive*/
    THAT, /*!< Inst is Passive*/
    OTHER /*!< Other*/
};    

/*! @enum MosType
    @brief Connection type of Mosfet.
    @see Netlist::mosType().
*/
enum class MosType: Byte
{
    DIFF, /*!< D/G/S diff */
    DIODE, /*!< G/D connected */  
    CAP, /*!< G/S connected */
    DUMMY /*!< D/S connected */
};

/*! @enum MosPattern
    @brief Pattern for pair of Mosfet.
    @see Pattern::pattern()
*/
enum class MosPattern: Byte
{
    DIFF_SOURCE, /*!< Source connected diff pair. */    
    DIFF_CASCODE, /*!< Cascode diff pair. */
    CASCODE, /*!< Gate connected cascode pair. */
    LOAD, /*!< Cascode pair with source connected to Power/Ground. */
    CROSS_CASCODE, /*!< Cross coupled cascode pair. */
    CROSS_LOAD, /*!< Cross coupled load. */
    INVALID /*!< No pattern detected. */
};
    
PROJECT_NAMESPACE_END

#endif
