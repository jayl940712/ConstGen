/*! @file sym_detect/SymDetect.h
    @brief Detect symmetric patterns.
    @author Mingjie Liu
    @date 11/24/2018
*/
#ifndef __SYMDETECT_H__
#define __SYMDETECT_H__

#include "db/Netlist.h"
#include "db/MosPair.h"
#include "sym_detect/Pattern.h"
#include <vector>

PROJECT_NAMESPACE_BEGIN
/*! @class SymDetect
    @brief SymDetect class
*/
class SymDetect
{
public:
/*! @brief Constructor
    Only needs netlist as input. 
    Pattern class inherently constructed.
    @param netlist Netlist class.
 */
    explicit SymDetect(const Netlist & netlist)
        : _netlist(netlist), _pattern(Pattern(netlist))
    {}

/*! @brief Hierarchy symmetry detection. 
    
    Output would contain 2 levels of hierarchy. symGroup
    is a vector of std::vector<MosPair> oneGroup. Where 
    oneGroup is a group of MosPair in the same symmetry 
    group. Each MosPair should follow a MosPattern, or 
    it should be of self symmetry. This funtion has been 
    also updated to contain basic passive pair symmetry. 

    @param symGroup Detected symmetry groups of netlist.
    @see MosPattern
    @see MosPair
*/
    void                        hiSymDetect(std::vector<std::vector<MosPair>> & symGroup) const;

private:
    const Netlist & _netlist;
    Pattern   _pattern;

/*! @brief Return pattern of MosPair. */
    MosPattern                  MosPairPtrn(MosPair & obj) const;
///    bool                        endSrch(IndexType mosId, PinType pinType) const;
/*! @brief Check if pair already reached. */
    bool                        existPair(std::vector<MosPair> & library, IndexType instId1, IndexType instId2) const;
/*! @breif Check if self symmetry pair already reached. */
    bool                        existPair(std::vector<MosPair> & library, IndexType instId) const;
/*! @brief Check if pair already reached. */
//    bool                        existPair(std::vector<MosPair> & library, IndexType instId1, IndexType instId2) const;
/*! @brief Return true if end of search path. 

    Current end search terminations:
    (1) Connected PASSIVE
    (2) DIFF_SOURCE reached through DRAIN
    (3) LOAD, CROSS_LOAD 
    (4) gate connected pairs 
*/
    bool                        endSrch(MosPair & obj) const;
/*! @brief Return true if a valid pair. 

    Valid pairs have following attributes:
    (1) Any mosfet pairs not reached by PASSIVE
    (2) Reached through same PinType
    (3) Not reached through gate
    (4) Valid MosPattern

    @param instId1 Reached pair instId1
    @param instId2 Reached pair instId2
    @param srchPinId1 instId1 reached by srchPinId1.
    @param srchPinId2 instId2 reached by srchPinId2.
*/
    bool                        validSrchObj(IndexType instId1, IndexType instId2, 
                                             IndexType srchPinId1, IndexType srchPinId2) const;
/*! @brief Push next valid MosPair to dfsStack.

    This function push valid pairs that could be reached 
    from currObj to dfsStack. It also removes reached DIFF_SOURCE
    MosPair from diffPairSrc.

    @see inVldDiffPairSrch.
    @param dfsVstPair All current visited MosPair
    @param dfsStack Stack to store to visit MosPair
    @param currObj Current MosPair under visit
    @param diffPairSrc All DFS sources
*/
    void                        pushNextSrchObj(std::vector<MosPair> & dfsVstPair, std::vector<MosPair> & dfsStack, 
                                                MosPair & currObj, std::vector<MosPair> & diffPairSrc) const;
/*! @brief Get srchPatrn MosPair connected to netId.

    Find MosPair that follow srchPatrn. These
    MosPair are appended to diffPair. Used to 
    get valid DFS source. srchPatrn inputs commonly
    are DIFF_SOURCE and CROSS_LOAD.
    Currently pairs should follow:
    (1) Have MosPattern srchPatrn
    (2) source connected to netId
    (3) MosType::DIFF

    @param netId Source should be connected to netId.
    @param diffPair Stored output vector.
*/
    void                        getPatrnNetConn(std::vector<MosPair> & diffPair, IndexType netId,
                                                MosPattern srchPatrn) const;
/*! @brief Get valid DFS source of netlist.

    Iterate all signal nets for getPatrnNetConn.
    Commonly srchPatrn are DIFF_SOURCE and 
    CROSS_LOAD.
    This would return all DFS sources.

    @see getDiffPairNetConn
    @param diffPair Store the output vector
*/
    void                        getDiffPair(std::vector<MosPair> & diffPair) const;
/*! @brief DFS search with given source. Visited MosPair are stored.

    Search for symmetry patterns in DFS manner with search source as diffPair.
    Store visited valid MosPair at dfsVstPair. diffPairSrch are needed as input
    to invalidate reached sources. dfsVstPair would be in the same hierarchy 
    symmetry group.

    @see pushNextSrchObj
    @param[out] dfsVstPair Vector to store all visited MosPair
    @param[in] diffPair DFS search source
    @param[in] diffPairSrch Vector of all stored DFS search source
*/
    void                        dfsDiffPair(std::vector<MosPair> & dfsVstPair, MosPair & diffPair, 
                                             std::vector<MosPair> & diffPairSrch) const;
/*! @brief Invalidate visited pairs from sources.

    If a MosPair have already been visited and is 
    a DFS source, it should be invalidated
    as a DFS search source to avoid revisiting.

    @param diffPairSrch Vector of all DFS sources.
    @param currPair MosPair to invalidate.
*/
    void                        inVldDiffPairSrch(std::vector<MosPair> & diffPairSrch, MosPair & currPair) const; 
/*! @brief Get valid drain connected mosfet to netId.

    Valid Mosfets must be connected to netId through PinType::DRAIN,
    it should also have MosType::DIFF. This is used to search self
    symmetric pairs connected to MosPattern::DIFF_SOURCE.

    @param vldMos Vector to store valid Mosfet.
    @param netId Id of connected net.
*/
    void                        getVldDrainMos(std::vector<IndexType> & vldMos, IndexType netId) const;
/*! @brief Iteratively search for self symmetry given diffPair.

    diffPair should be of MosPattern::DIFF_SOURCE. Valid self
    symmetric instances are added to dfsVstPair. Redundancy is 
    also removed from dfsVstPair.

    @param dfsVstPair Self symmetric pairs will be added to this vector.
    @param diffPair MosPattern::DIFF_SOURCE pair to begin self symmetry search.

    @see getVldDrainMos
*/
    void                        selfSymSrch(std::vector<MosPair> & dfsVstPair, MosPair & diffPair) const;
/*! @brief Top function to call to add self symmetry to already searched symmetry group.

    Iteratively searches for self symmetry instances for MosPattern::DIFF_SOURCE pairs in dfsVstPair.
    Valid self symmetry instances will be appended. This function is called at the end of every
    DFS search for symmetry pairs.

    @param dfsVstPair Symmetry group.

    @see selfSymSrch
    @see hiSymDetect
*/
    void                        addSelfSym(std::vector<MosPair> & dfsVstPair) const;

};

PROJECT_NAMESPACE_END

#endif
