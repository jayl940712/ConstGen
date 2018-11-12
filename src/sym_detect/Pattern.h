#ifndef __PATTERN_H__
#define __PATTERN_H__

#include "db/Netlist.h"
#include "global/type.h"
#include <utility>

PROJECT_NAMESPACE_BEGIN

class Pattern
{
public:
    struct MosPair
    {
        IndexType mosId1, mosId2;
        IndexType inputNetId1, inputNetId2, outputNetId1, outputNetId2, gateNetId1, gateNetId2;
        IndexType inputPinId1, inputPinId2, outputPinId1, outputPinId2;
    };
public:
    explicit Pattern(Netlist & netlist) 
        : _netlist(netlist)
    {}

    MosPattern          pattern(std::pair<IndexType, IndexType> pair);

private:
    Netlist &           _netlist;

    MosPair             constructPair(std::pair<IndexType, IndexType> pair, PinType inputPinType);

    bool                matchedSize(MosPair & pair);
    bool                diffPairInput(MosPair & pair);
    bool                diffPairCascode(MosPair & pair);
    bool                validPairCascode(MosPair & pair);
    bool                validPairLoad(MosPair & pair);
    bool                crossPairCascode(MosPair & pair);
    bool                crossPairLoad(MosPair & pair);
};

PROJECT_NAMESPACE_END

#endif
