/*! @file db/MosPair.h
    @brief A pair of Mosfet with MosPattern
    @author Mingjie Liu
    @date 11/27/2018
*/ 
#ifndef __MOSPAIR_H__
#define __MOSPAIR_H__

#include "global/type.h"
#include "sym_detect/Pattern.h"

PROJECT_NAMESPACE_BEGIN
/*! @class MosPair
    @brief A pair of Mosfet with MosPattern

    This class stores a pair of Mosfet Id 
    and also assists DFS in SymDetect.h.
    This class has no reference to netlist,
    pattern needs to be set at construction.
*/
class MosPair
{
public:
/*! @brief Default Constructor. */
    explicit MosPair() = default;
/*! @brief Constructor for MosPair

    Sequence of Ids does not matter.
    pattern is set according to input.

    @param mosId1 Id for Mos1
    @param mosId2 Id for Mos2
*/
    explicit MosPair(IndexType mosId1, IndexType mosId2, MosPattern pattern)
        :_mosId1(mosId1), _mosId2(mosId2), _pattern(pattern)
    {
        _valid = true; /*!< valid is set true as default. */
        _srchPinType = PinType::SOURCE; /*!< reached Pin set as SOURCE default. */
    }

    // Getters
/*! @brief Get mosId1. */
    IndexType               mosId1() const                  { return _mosId1; }
/*! @brief Get mosId2. */
    IndexType               mosId2() const                  { return _mosId2; }
/*! @brief Return if valid search pair. 
    @see SymDetect::inVldDiffPairSrch
*/
    bool                    valid() const                   { return _valid; }
/*! @brief Get pattern */
    MosPattern              pattern() const                 { return _pattern; }
/*! @brief Get PinType on how DFS reached the pair. */
    PinType                 srchPinType() const             { return _srchPinType; }

    // Setters
/*! @brief Invalidate pair. */
    void                    inVld()                         { _valid = false; }
/*! @brief set reached PinType.
    
    This is how the pair is reached through DFS search.
*/
    void                    setSrchPinType(PinType type)    { _srchPinType = type; }

/*! @brief Return next PinType to search. */
    PinType                 nextPinType()                   { return Pin::nextPinType(_srchPinType); }
/*! @brief Equal operator 
    
    Two pairs are equal if Id are equal.
    Sequence of Id does not matter.
*/
    bool                    isEqual(const MosPair & right) const;

private:
    IndexType               _mosId1, _mosId2;
    MosPattern              _pattern;
    bool                    _valid;
    PinType                 _srchPinType;
};

PROJECT_NAMESPACE_END

#endif
