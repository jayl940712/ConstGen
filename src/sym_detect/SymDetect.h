#ifndef __SYMDETECT_H__
#define __SYMDETECT_H__

#include "db/Netlist.h"
#include <vector>
#include <utility>

PROJECT_NAMESPACE_BEGIN

class SymDetect
{
public:
    struct diffPair
    {
        std::pair <IndexType, IndexType> diff;
        std::vector<IndexType> bias;
        InstanceType type;
    };
public:
    explicit SymDetect(Netlist & netlist)
        : _netlist(netlist)
    {}
    
    std::vector<diffPair>                           diffPairSearch();
    std::vector<std::pair<IndexType, IndexType>>    symGroup(std::pair<IndexType, IndexType> diffPair);

private:
    Netlist & _netlist;
    
    bool                                            validPair(IndexType mosId1, IndexType mosId2);
    bool                                            endSearch(IndexType mosId, PinType pinType);
    IndexType                                       pinId(IndexType mosId, PinType pinType);

}; 

PROJECT_NAMESPACE_END

#endif
