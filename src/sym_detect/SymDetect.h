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
    };
public:
    explicit SymDetect(Netlist & netlist)
        : _netlist(netlist)
    {}
    
    std::vector<diffPair> diffPairSearch();

private:
    Netlist & _netlist;
}; 

PROJECT_NAMESPACE_END

#endif
