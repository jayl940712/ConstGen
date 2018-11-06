#include "sym_detect/SymDetect.h"
#include "db/Netlist.h"
#include "db/Pin.h"
#include <iostream>
#include <stack>

PROJECT_NAMESPACE_BEGIN

std::vector<SymDetect::diffPair> SymDetect::diffPairSearch() 
{
    std::vector<SymDetect::diffPair> results;
    for (IndexType netId = 0; netId < _netlist.numNets(); netId++)
        if (_netlist.net(netId).netType() == NetType::SIGNAL)
        {
            std::vector<IndexType> source, drain;
            source = _netlist.netMosfetId(netId, PinType::SOURCE, MosType::DIFF);
            drain = _netlist.netMosfetId(netId, PinType::DRAIN, MosType::DIFF);
            if (drain.size() > 0 && source.size() == 2 && _netlist.instanceNetId(source[0], PinType::GATE) != _netlist.instanceNetId(source[1], PinType::GATE))
            {
                SymDetect::diffPair pair;
                pair.bias = drain;
                pair.diff = std::pair<IndexType, IndexType>(source[0],source[1]); 
                results.push_back(pair);
            }
        }
    return results;
}

bool SymDetect::validPair(IndexType mosId1, IndexType mosId2)
{
    if (mosId1 == mosId2)
        return false;
    if (_netlist.instance(mosId1).type() != _netlist.instance(mosId2).type())
        return false;
    if (_netlist.mosType(mosId1) != MosType::DIFF && _netlist.mosType(mosId1) != MosType::DIODE)
        return false;
    if (_netlist.mosType(mosId2) != MosType::DIFF && _netlist.mosType(mosId2) != MosType::DIODE)
        return false;
    if (_netlist.instanceNetId(mosId1, PinType::GATE) == _netlist.instanceNetId(mosId2, PinType::GATE))
        return true;
    if (_netlist.instanceNetId(mosId1, PinType::GATE) == _netlist.instanceNetId(mosId2, PinType::DRAIN) && _netlist.instanceNetId(mosId1, PinType::DRAIN) == _netlist.instanceNetId(mosId2, PinType::GATE))
        return true;
    return false;
}

bool SymDetect::endSearch(IndexType mosId, PinType currentPinType)
{
    PinType toSearchPinType = Pin::otherPinType(currentPinType);
    IndexType toSearchNetId = _netlist.instanceNetId(mosId, toSearchPinType);
    if (_netlist.net(toSearchNetId).netType() == NetType::SIGNAL)
        return false;
    return true;
}

IndexType SymDetect::pinId(IndexType mosId, PinType pinType)
{
    std::vector<IndexType> pinArray = _netlist.instance(mosId).pinIdArray();
    for (IndexType pinId : pinArray)
        if (_netlist.pin(pinId).type() == pinType)
            return pinId;
    return INDEX_TYPE_MAX;
}

std::vector<std::pair<IndexType, IndexType>> SymDetect::symGroup(std::pair<IndexType, IndexType> diffPair)
{
    std::stack<std::pair<IndexType, IndexType>> DFS_pair;
    std::stack<PinType> DFS_pinType;
    std::vector<std::pair<IndexType, IndexType>> symResult;
    DFS_pair.push(diffPair);
    DFS_pinType.push(PinType::SOURCE);
    while (!DFS_pair.empty())
    {
        std::pair<IndexType, IndexType> currentPair = DFS_pair.top();
        PinType currentPinType = DFS_pinType.top();
        DFS_pair.pop();
        DFS_pinType.pop();
        symResult.push_back(currentPair);
        if (!endSearch(currentPair.first, currentPinType) && !endSearch(currentPair.second, currentPinType))
        {
            std::cout << "Next Searches" << std::endl;
            std::vector<IndexType> first = _netlist.pinMosfetId(pinId(currentPair.first, Pin::otherPinType(currentPinType)));
            std::vector<IndexType> second = _netlist.pinMosfetId(pinId(currentPair.second, Pin::otherPinType(currentPinType)));
            for (IndexType instId: first)
                std::cout << _netlist.instance(instId).name() << " ";
            std::cout << std::endl;
            for (IndexType instId: second)
                std::cout << _netlist.instance(instId).name() << " ";
            std::cout << std::endl << "END" << std::endl;
        }
    }
    return symResult;
}

PROJECT_NAMESPACE_END
