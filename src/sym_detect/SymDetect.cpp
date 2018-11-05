#include "sym_detect/SymDetect.h"
#include "db/Netlist.h"

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

PROJECT_NAMESPACE_END
