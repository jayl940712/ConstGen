#ifndef __SYMDETECT_H__
#define __SYMDETECT_H__

#include "db/Netlist.h"
#include "sym_detect/Pattern.h"
#include <vector>
#include <utility>
#include <stack>

PROJECT_NAMESPACE_BEGIN

class SymDetect
{
public:
    explicit SymDetect(Netlist & netlist)
        : _netlist(netlist), _pattern(Pattern(netlist))
    {}

    std::vector<std::pair<IndexType, IndexType>>    diffPairSearch();
    std::vector<std::pair<IndexType, IndexType>>    symGroup(std::pair<IndexType, IndexType> diffPair);

private:
    Netlist & _netlist;
    Pattern   _pattern;

    bool                                        endSearch(IndexType mosId, PinType pinType);
    bool                                        existPair(std::vector<std::pair<IndexType, IndexType>> &library, std::pair<IndexType, IndexType> query);
    bool                                        existPair(std::stack<std::pair<IndexType, IndexType>> library, std::pair<IndexType, IndexType> query);
}; 

PROJECT_NAMESPACE_END

#endif
