#include "sym_detect/SymDetect.h"
#include "db/Netlist.h"
#include "db/Pin.h"
#include <iostream>
#include <stack>

PROJECT_NAMESPACE_BEGIN

std::vector<std::pair<IndexType, IndexType>> SymDetect::diffPairSearch() 
{
    std::vector<std::pair<IndexType, IndexType>> results;
    for (IndexType netId = 0; netId < _netlist.numNets(); netId++)
        if (_netlist.net(netId).netType() == NetType::SIGNAL)
        {
            std::vector<IndexType> source;
            source = _netlist.netMosfetId(netId, PinType::SOURCE, MosType::DIFF);
            std::cout << "Returned source devices." << std::endl;
            if (source.size() < 2)
                continue;
            for (IndexType i = 0; i < source.size() - 1; i++)
                for (IndexType j = i + 1; j < source.size(); j++)
                    if (_pattern.pattern(std::pair<IndexType, IndexType> (source[i], source[j])) == MosPattern::DIFF_SOURCE)
                        results.push_back(std::pair<IndexType, IndexType> (source[i], source[j]));
        }
    return results;
}

bool SymDetect::endSearch(IndexType mosId, PinType currentPinType)
{
    PinType toSearchPinType = Pin::otherPinType(currentPinType);
    IndexType toSearchNetId = _netlist.instanceNetId(mosId, toSearchPinType);
    if (_netlist.net(toSearchNetId).netType() == NetType::SIGNAL)
        return false;
    return true;
}

std::vector<std::pair<IndexType, IndexType>> SymDetect::symGroup(std::pair<IndexType, IndexType> diffPair)
{
    bool begin = true;
    std::stack<std::pair<IndexType, IndexType>> DFS_pair;
    std::stack<PinType> DFS_pinType;
    std::vector<std::pair<IndexType, IndexType>> symResult;
    DFS_pair.push(diffPair);
    DFS_pinType.push(PinType::SOURCE);
    std::vector<PinType> possiblePinType = {PinType::SOURCE, PinType::DRAIN};
    std::vector<InstanceType> possibleMosType = {InstanceType::NMOS, InstanceType::PMOS};
    while (!DFS_pair.empty())
    {
        std::pair<IndexType, IndexType> currentPair = DFS_pair.top();
        PinType currentPinType = DFS_pinType.top();
        MosPattern currentPattern = _pattern.pattern(currentPair);
        DFS_pair.pop();
        DFS_pinType.pop();
        symResult.push_back(currentPair);
        std::cout << _netlist.instance(currentPair.first).name() << _netlist.instance(currentPair.second).name() << std::endl;
        if (!begin && currentPattern == MosPattern::DIFF_SOURCE)
            continue;
        if (currentPattern == MosPattern::LOAD || currentPattern == MosPattern::CROSS_LOAD)
            continue;
        begin = false;
        if (!endSearch(currentPair.first, currentPinType) && !endSearch(currentPair.second, currentPinType))
            for (InstanceType instanceSearch : possibleMosType)
                for (PinType pinSearch : possiblePinType)
                {
                    IndexType searchPinId1 = _netlist.pinId(currentPair.first, Pin::otherPinType(currentPinType));
                    IndexType searchPinId2 = _netlist.pinId(currentPair.second, Pin::otherPinType(currentPinType));
                    std::vector<IndexType> MOS1 = _netlist.pinInstanceId(searchPinId1, instanceSearch, pinSearch);
                    std::vector<IndexType> MOS2 = _netlist.pinInstanceId(searchPinId2, instanceSearch, pinSearch);
                    for (IndexType mosId1: MOS1)
                        for (IndexType mosId2 : MOS2)
                        {
                            MosPattern patternSearch = _pattern.pattern(std::pair<IndexType, IndexType> (mosId1, mosId2));
                            if (patternSearch != MosPattern::INVALID && patternSearch != MosPattern::DIFF_CASCODE && patternSearch != MosPattern::DIFF_SOURCE)
                            {
                                DFS_pair.push(std::pair<IndexType, IndexType>(mosId1, mosId2));
                                DFS_pinType.push(pinSearch);
                            }
                        }
                }
    }
    return symResult;
}

PROJECT_NAMESPACE_END
