#include "sym_detect/SymDetect.h"

PROJECT_NAMESPACE_BEGIN

void SymDetect::getDiffPairNetConn(std::vector<MosPair> & diffPair, IndexType netId)
{
    std::vector<IndexType> srcMos;
    _netlist.getInstNetConn(srcMos, netId);  
    _netlist.fltrInstNetConnPinType(srcMos, netId, PinType::SOURCE);
    _netlist.fltrInstMosType(srcMos, MosType::DIFF);
    for (IndexType i = 0; i < srcMos.size(); i++)
        for (IndexType j = i + 1; j < srcMos.size(); j++)
            if (_pattern.pattern(srcMos[i], srcMos[j]) == MosPattern::DIFF_SOURCE)
                diffPair.emplace_back(srcMos[i], srcMos[j]);
}

void SymDetect::getDiffPair(std::vector<MosPair> & diffPair)
{
    diffPair.clear();
    for (IndexType netId = 0; netId < _netlist.numNet(); netId++)
        if (_netlist.isSignal(netId))
            getDiffPairNetConn(diffPair, netId); 
}

//bool SymDetect::endSrch(IndexType mosId, PinType currPinType)
//{
//    IndexType toSrchNetId = _netlist.instNetId(mosId, Pin::nextPinType(currPinType));
//    return !_netlist.isSignal(toSrchNetId);
//}

bool SymDetect::existPair(std::vector<MosPair> & library, IndexType instId1, IndexType instId2)
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

bool SymDetect::existPair(std::vector<srchObj> & library, IndexType instId1, IndexType instId2)
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

MosPattern SymDetect::srchObjPtrn(srchObj & obj)
{
    return _pattern.pattern(obj.pair.mosId1, obj.pair.mosId2);
}

bool SymDetect::endSrch(srchObj & currObj)
{
    if (srchObjPtrn(currObj) == MosPattern::DIFF_SOURCE &&
        currObj.srchPinType == PinType::DRAIN)
        return true;  
    if (srchObjPtrn(currObj) == MosPattern::LOAD ||
        srchObjPtrn(currObj) == MosPattern::CROSS_LOAD)
        return true;
    if (currObj.srchPinType != PinType::SOURCE && currObj.srchPinType != PinType::DRAIN)
        return true;
    return false;
}

bool SymDetect::validSrchObj(IndexType instId1, IndexType instId2, IndexType srchPinId1, IndexType srchPinId2)
{
    if (_netlist.getPinTypeInstPinConn(instId1, srchPinId1) != _netlist.getPinTypeInstPinConn(instId2, srchPinId2))
        return false;
    MosPattern currPtrn = _pattern.pattern(instId1, instId2);
//    if (currPtrn == MosPattern::DIFF_SOURCE ||
//        currPtrn == MosPattern::DIFF_CASCODE)
//        return false;
    if (currPtrn != MosPattern::INVALID)
        return true;
    return false;
}  

void SymDetect::inVldDiffPairSrch(std::vector<MosPair> & diffPairSrch, MosPair & currPair)
{
    for (MosPair & pair : diffPairSrch)
        if (pair == currPair)
            pair.valid = false;
}

void SymDetect::pushNextSrchObj(std::vector<MosPair> & dfsVstPair, std::vector<srchObj> & dfsStack, srchObj & currObj, std::vector<MosPair> & diffPairSrc)
{
    if (endSrch(currObj))
        return;
    IndexType srchPinId1 = _netlist.instPinId(currObj.pair.mosId1, Pin::nextPinType(currObj.srchPinType));
    IndexType srchPinId2 = _netlist.instPinId(currObj.pair.mosId2, Pin::nextPinType(currObj.srchPinType));
    std::vector<IndexType> Mos1, Mos2;
    _netlist.getInstPinConn(Mos1, srchPinId1);
    _netlist.getInstPinConn(Mos2, srchPinId2);
    for (IndexType instId1: Mos1)
    {
        for (IndexType instId2 : Mos2)
        {
            if (validSrchObj(instId1, instId2, srchPinId1, srchPinId2) &&
                !existPair(dfsVstPair, instId1, instId2) &&
                !existPair(dfsStack, instId1, instId2)) 
            {
                MosPair currPair(instId1, instId2);
                dfsStack.emplace_back(currPair, _netlist.getPinTypeInstPinConn(instId1, srchPinId1));
                inVldDiffPairSrch(diffPairSrc, currPair);
            }
       }
    }
}

void SymDetect::dfsDiffPair(std::vector<MosPair> & dfsVstPair, MosPair & diffPair, std::vector<MosPair> & diffPairSrc)
{
    std::vector<srchObj> dfsStack;  //use vector to implement stack.
    dfsStack.emplace_back(diffPair, PinType::SOURCE);
    while (!dfsStack.empty())
    {
        srchObj currObj = dfsStack.back();
        dfsStack.pop_back();
        dfsVstPair.push_back(currObj.pair);
        pushNextSrchObj(dfsVstPair, dfsStack, currObj, diffPairSrc);
    } 
}

void SymDetect::hiSymDetect(std::vector<std::vector<MosPair>> & symGroup)
{
    std::vector<MosPair> dfsVstPair;
    std::vector<MosPair> diffPairSrc;
    getDiffPair(diffPairSrc);
    for (MosPair & pair : diffPairSrc)
    {
        if (pair.valid)
        {
            dfsVstPair.clear();
            dfsDiffPair(dfsVstPair, pair, diffPairSrc);
            symGroup.push_back(dfsVstPair);
        }
    }
}

PROJECT_NAMESPACE_END
