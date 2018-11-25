/*! @file sym_detect/SymDetect.cpp
    @brief Detect symmetric patterns
    @author Mingjie LIu
    @date 11/24/2018
*/
#include "sym_detect/SymDetect.h"

PROJECT_NAMESPACE_BEGIN

void SymDetect::getDiffPairNetConn(std::vector<MosPair> & diffPair, IndexType netId) const
{
    std::vector<IndexType> srcMos;
    _netlist.getInstNetConn(srcMos, netId);  
    _netlist.fltrInstNetConnPinType(srcMos, netId, PinType::SOURCE); //filter connect by source
    _netlist.fltrInstMosType(srcMos, MosType::DIFF); //filter by MosType::DIFF
    for (IndexType i = 0; i < srcMos.size(); i++)
        for (IndexType j = i + 1; j < srcMos.size(); j++)
            if (_pattern.pattern(srcMos[i], srcMos[j]) == MosPattern::DIFF_SOURCE)
                diffPair.emplace_back(srcMos[i], srcMos[j]); //add only if MosPattern::DIFF_SOURCE
}

void SymDetect::getDiffPair(std::vector<MosPair> & diffPair) const
{
    diffPair.clear();
    for (IndexType netId = 0; netId < _netlist.numNet(); netId++)
        if (_netlist.isSignal(netId)) //Iterate for all signal nets
            getDiffPairNetConn(diffPair, netId); 
}

//bool SymDetect::endSrch(IndexType mosId, PinType currPinType)
//{
//    IndexType toSrchNetId = _netlist.instNetId(mosId, Pin::nextPinType(currPinType));
//    return !_netlist.isSignal(toSrchNetId);
//}

bool SymDetect::existPair(std::vector<MosPair> & library, IndexType instId1, IndexType instId2) const
{
    for (MosPair & currPair : library)
    {
        if (currPair.mosId1 == instId1 && currPair.mosId2 == instId2)
            return true;
        if (currPair.mosId2 == instId1 && currPair.mosId1 == instId1)
            return true;
    }
    return false;
}

bool SymDetect::existPair(std::vector<srchObj> & library, IndexType instId1, IndexType instId2) const
{
    for (srchObj & currObj : library)
    {
        if (currObj.pair.mosId1 == instId1 && currObj.pair.mosId2 == instId2)
            return true;
        if (currObj.pair.mosId2 == instId1 && currObj.pair.mosId1 == instId2)
            return true;
    }
    return false;
}

MosPattern SymDetect::srchObjPtrn(srchObj & obj) const
{
    return _pattern.pattern(obj.pair.mosId1, obj.pair.mosId2);
}

bool SymDetect::endSrch(srchObj & currObj) const
{
    if (srchObjPtrn(currObj) == MosPattern::DIFF_SOURCE &&
        currObj.srchPinType == PinType::DRAIN)
        return true; // If reached new DIFF_SOURCE through drain. 
    if (srchObjPtrn(currObj) == MosPattern::LOAD ||
        srchObjPtrn(currObj) == MosPattern::CROSS_LOAD)
        return true; // If LOAD pattern.
    if (currObj.srchPinType != PinType::SOURCE && currObj.srchPinType != PinType::DRAIN)
        return true; // If reached by gate.
    return false;
}

bool SymDetect::validSrchObj(IndexType instId1, IndexType instId2,
                             IndexType srchPinId1, IndexType srchPinId2) const
{
// This needs update and more considerations.
    if (_netlist.getPinTypeInstPinConn(instId1, srchPinId1) != _netlist.getPinTypeInstPinConn(instId2, srchPinId2))
        return false; //false if reached through different PinType
    if (_netlist.getPinTypeInstPinConn(instId1, srchPinId1) == PinType::GATE)
        return false; //block gate connected patterns
    if (_netlist.getPinTypeInstPinConn(instId2, srchPinId2) == PinType::GATE)
        return false;
    MosPattern currPtrn = _pattern.pattern(instId1, instId2);
//  Below is removed, DIFF_SOURCE removal updated.
//    if (currPtrn == MosPattern::DIFF_SOURCE ||
//        currPtrn == MosPattern::DIFF_CASCODE)
//        return false;
    if (currPtrn != MosPattern::INVALID)
        return true;
    return false;
}  

void SymDetect::inVldDiffPairSrch(std::vector<MosPair> & diffPairSrch, MosPair & currPair) const
{
    for (MosPair & pair : diffPairSrch)
        if (pair == currPair)
            pair.valid = false;
}

void SymDetect::pushNextSrchObj(std::vector<MosPair> & dfsVstPair, std::vector<srchObj> & dfsStack,
                                 srchObj & currObj, std::vector<MosPair> & diffPairSrc) const
{
    if (endSrch(currObj))
        return; //return if endSrch
    IndexType srchPinId1 = _netlist.instPinId(currObj.pair.mosId1, Pin::nextPinType(currObj.srchPinType));
    IndexType srchPinId2 = _netlist.instPinId(currObj.pair.mosId2, Pin::nextPinType(currObj.srchPinType));
    std::vector<IndexType> Mos1, Mos2;
    _netlist.getInstPinConn(Mos1, srchPinId1); //Connected Inst through nextPinType
    _netlist.getInstPinConn(Mos2, srchPinId2);
    for (IndexType instId1: Mos1)
    {
        for (IndexType instId2 : Mos2)
        {
            if (validSrchObj(instId1, instId2, srchPinId1, srchPinId2) && // valid pattern
                !existPair(dfsVstPair, instId1, instId2) && //not visited before
                !existPair(dfsStack, instId1, instId2)) 
            {
                MosPair currPair(instId1, instId2);
                dfsStack.emplace_back(currPair, _netlist.getPinTypeInstPinConn(instId1, srchPinId1));
                inVldDiffPairSrch(diffPairSrc, currPair); //invalidate DIFF_SOURCE
            }
       }
    }
}

void SymDetect::dfsDiffPair(std::vector<MosPair> & dfsVstPair, MosPair & diffPair, 
                            std::vector<MosPair> & diffPairSrc) const
{
    std::vector<srchObj> dfsStack;  //use vector to implement stack.
    dfsStack.emplace_back(diffPair, PinType::SOURCE);
    while (!dfsStack.empty()) //DFS 
    {
        srchObj currObj = dfsStack.back();
        dfsStack.pop_back();
        dfsVstPair.push_back(currObj.pair);
        pushNextSrchObj(dfsVstPair, dfsStack, currObj, diffPairSrc);
    } 
}

void SymDetect::hiSymDetect(std::vector<std::vector<MosPair>> & symGroup) const
{
    std::vector<MosPair> dfsVstPair;
    std::vector<MosPair> diffPairSrc;
    getDiffPair(diffPairSrc); //get all DFS source
    for (MosPair & pair : diffPairSrc) //for each source
    {
        if (pair.valid)
        {
            dfsVstPair.clear();
            dfsDiffPair(dfsVstPair, pair, diffPairSrc); //search
            symGroup.push_back(dfsVstPair); //add results to new group
        }
    }
}

PROJECT_NAMESPACE_END
