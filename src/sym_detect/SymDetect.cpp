/*! @file sym_detect/SymDetect.cpp
    @brief Detect symmetric patterns
    @author Mingjie Liu
    @date 11/24/2018
*/
#include "sym_detect/SymDetect.h"

PROJECT_NAMESPACE_BEGIN

void SymDetect::getPatrnNetConn(std::vector<MosPair> & diffPair, IndexType netId,
                                    MosPattern srchPatrn) const
{
    std::vector<IndexType> srcMos;
    _netlist.getInstNetConn(srcMos, netId);  
    _netlist.fltrInstNetConnPinType(srcMos, netId, PinType::SOURCE); //filter connect by source
    _netlist.fltrInstMosType(srcMos, MosType::DIFF); //filter by MosType::DIFF
    for (IndexType i = 0; i < srcMos.size(); i++)
    {
        for (IndexType j = i + 1; j < srcMos.size(); j++)
        {
            MosPattern pattern = _pattern.pattern(srcMos[i], srcMos[j]);
            if (pattern == srchPatrn)
            {
                diffPair.emplace_back(srcMos[i], srcMos[j], pattern); //add only if matched MosPattern
            }
        }
    }
}

void SymDetect::getDiffPair(std::vector<MosPair> & diffPair) const
{
    diffPair.clear();
    for (IndexType netId = 0; netId < _netlist.numNet(); netId++)
        if (_netlist.isSignal(netId)) //Iterate for all signal nets for DIFF_SOURCE
            getPatrnNetConn(diffPair, netId, MosPattern::DIFF_SOURCE); 
    for (IndexType netId = 0; netId < _netlist.numNet(); netId++)
        if (!_netlist.isSignal(netId)) //Iterate for non signal nets for CROSS_LOAD
            getPatrnNetConn(diffPair, netId, MosPattern::CROSS_LOAD); 
}

bool SymDetect::existPair(std::vector<MosPair> & library, IndexType instId1, IndexType instId2) const
{
    for (MosPair & currPair : library)
    {
        if (currPair.mosId1() == instId1 && currPair.mosId2() == instId2)
            return true;
        if (currPair.mosId2() == instId1 && currPair.mosId1() == instId2)
            return true;
    }
    return false;
}

bool SymDetect::existPair(std::vector<MosPair> & library, IndexType instId) const
{
    for (MosPair & currPair : library)
    {
        if (currPair.mosId1() == instId)
            return true;
        if (currPair.mosId2() == instId)
            return true;
    }
    return false;
}

bool SymDetect::endSrch(MosPair & currObj) const
{
    if (currObj.pattern() == MosPattern::PASSIVE)
        return _netlist.instNetId(currObj.mosId1(), currObj.nextPinType1()) == 
               _netlist.instNetId(currObj.mosId2(), currObj.nextPinType2()); // If reached connected passive device.
    if (currObj.pattern() == MosPattern::DIFF_SOURCE &&
        currObj.srchPinType1() == PinType::DRAIN)
        return true; // If reached new DIFF_SOURCE through drain. 
    if (currObj.pattern() == MosPattern::DIFF_SOURCE &&
        currObj.srchPinType1() == PinType::GATE &&
        currObj.srchPinType2() == PinType::GATE)
        return false; // If reached new DIFF_SOURCE through gate. Comment to exclude gate connections. 
    if (currObj.pattern() == MosPattern::CROSS_LOAD &&
        currObj.srchPinType1() == PinType::DRAIN)
        return true; // If reached new CROSS_LOAD through drain. 
   if (currObj.pattern() == MosPattern::LOAD)
        return true; // If LOAD pattern.
    if (currObj.srchPinType1() != PinType::SOURCE && 
        currObj.srchPinType1() != PinType::DRAIN)
        return true; // If reached by gate.
    return false;
}

bool SymDetect::validSrchObj(IndexType instId1, IndexType instId2,
                             IndexType srchPinId1, IndexType srchPinId2) const
{
    MosPattern currPtrn = _pattern.pattern(instId1, instId2);
    if (Pin::isPasvDev(_netlist.pin(srchPinId1).type()) &&
        Pin::isPasvDev(_netlist.pin(srchPinId2).type()))
        return currPtrn == MosPattern::PASSIVE; // Pass valid for all passive pairs.
    if (currPtrn == MosPattern::PASSIVE)
        return true;
// This needs update and more considerations.
    if (_netlist.getPinTypeInstPinConn(instId1, srchPinId1) != 
        _netlist.getPinTypeInstPinConn(instId2, srchPinId2))
        return false; //false if reached through different PinType
    if (_netlist.getPinTypeInstPinConn(instId1, srchPinId1) == PinType::GATE)
        return false; //block gate connected patterns
    if (_netlist.getPinTypeInstPinConn(instId2, srchPinId2) == PinType::GATE)
        return false;
//  Below is removed, DIFF_SOURCE removal updated.
//    if (currPtrn == MosPattern::DIFF_SOURCE ||
//        currPtrn == MosPattern::DIFF_CASCODE)
//        return false;
    if (currPtrn != MosPattern::INVALID)
        return true;
    return false;
}  

bool SymDetect::validDiffPair(IndexType instId1, IndexType instId2,
                            IndexType srchPinId1, IndexType srchPinId2) const
{
    if (_netlist.getPinTypeInstPinConn(instId1, srchPinId1) != PinType::GATE ||
        _netlist.getPinTypeInstPinConn(instId2, srchPinId2) != PinType::GATE)
        return false;
    if (_pattern.pattern(instId1, instId2) == MosPattern::DIFF_SOURCE)
        return true;
    return false;
}

void SymDetect::inVldDiffPairSrch(std::vector<MosPair> & diffPairSrch, MosPair & currPair) const
{
    for (MosPair & pair : diffPairSrch)
        if (pair.isEqual(currPair))
            pair.inVld();
}

void SymDetect::pushNextSrchObj(std::vector<MosPair> & dfsVstPair, std::vector<MosPair> & dfsStack,
                                 MosPair & currObj, std::vector<MosPair> & diffPairSrc) const
{
    if (endSrch(currObj))
        return; //return if endSrch
    IndexType srchPinId1 = _netlist.instPinId(currObj.mosId1(), currObj.nextPinType1());
    IndexType srchPinId2 = _netlist.instPinId(currObj.mosId2(), currObj.nextPinType2());
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
                MosPair currPair(instId1, instId2, _pattern.pattern(instId1, instId2));
                currPair.setSrchPinType1(_netlist.getPinTypeInstPinConn(instId1, srchPinId1));
                currPair.setSrchPinType2(_netlist.getPinTypeInstPinConn(instId2, srchPinId2));
                dfsStack.push_back(currPair);
                inVldDiffPairSrch(diffPairSrc, currPair); //invalidate DFS sources
            }
            else if (validDiffPair(instId1, instId2, srchPinId1, srchPinId2) &&
                    !existPair(dfsVstPair, instId1, instId2) && 
                    !existPair(dfsStack, instId1, instId2) &&
                    !existPair(diffPairSrc, instId1, instId2))
            {
                MosPair currPair(instId1, instId2, MosPattern::DIFF_SOURCE);
                dfsStack.push_back(currPair);
            }
       }
    }
}

void SymDetect::dfsDiffPair(std::vector<MosPair> & dfsVstPair, MosPair & diffPair, 
                            std::vector<MosPair> & diffPairSrc) const
{
    std::vector<MosPair> dfsStack;  //use vector to implement stack.
    dfsStack.push_back(diffPair);
    while (!dfsStack.empty()) //DFS 
    {
        MosPair currObj = dfsStack.back();
        dfsStack.pop_back();
        dfsVstPair.push_back(currObj); //pop current visit from stack and add to visited
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
        if (pair.valid())
        {
            dfsVstPair.clear();
            dfsDiffPair(dfsVstPair, pair, diffPairSrc); //search
            addSelfSym(dfsVstPair); //add self symmetry pairs.
            symGroup.push_back(dfsVstPair); //add results to new group
        }
    }
}

void SymDetect::getVldDrainMos(std::vector<IndexType> & vldMos, IndexType netId) const
{
    _netlist.getInstNetConn(vldMos, netId);  
    _netlist.fltrInstNetConnPinType(vldMos, netId, PinType::DRAIN); //filter connect by drain
    _netlist.fltrInstMosType(vldMos, MosType::DIFF); //filter by MosType::DIFF
}
    

void SymDetect::selfSymSrch(std::vector<MosPair> & dfsVstPair, MosPair & diffPair) const
{
    if (diffPair.pattern() != MosPattern::DIFF_SOURCE)
        return; //return if not of DIFF_SOURCE pattern.
    if (!_netlist.isSignal(_netlist.srcNetId(diffPair.mosId1())))
        return; //return if already reached ground.
    std::vector<IndexType> dfsStack; //used vector as stack implementation.
    getVldDrainMos(dfsStack, _netlist.srcNetId(diffPair.mosId1())); //push all valid drain connection to stack.
    while (!dfsStack.empty()) //dfs search
    {
        IndexType currMosId = dfsStack.back();
        dfsStack.pop_back();
        if (!existPair(dfsVstPair, currMosId)) //if not already as a pattern.
            dfsVstPair.emplace_back(currMosId, currMosId, MosPattern::SELF);
        IndexType netId = _netlist.srcNetId(currMosId);
        if (_netlist.isSignal(netId)) //search if not reached ground.
        {
            std::vector<IndexType> nextVst;
            getVldDrainMos(nextVst, _netlist.srcNetId(currMosId));
            for (IndexType id : nextVst)
                dfsStack.push_back(id); //add valid Inst to search.
        }
    }
}

void SymDetect::addSelfSym(std::vector<MosPair> & dfsVstPair) const
{
    for (MosPair & pair : dfsVstPair)
        selfSymSrch(dfsVstPair, pair);
}

PROJECT_NAMESPACE_END
