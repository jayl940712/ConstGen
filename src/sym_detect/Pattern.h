#ifndef __PATTERN_H__
#define __PATTERN_H__

#include "db/Netlist.h"
#include "global/type.h"

PROJECT_NAMESPACE_BEGIN

class Pattern
{
public:
    explicit Pattern(Netlist & netlist) 
        : _netlist(netlist)
    {}

    MosPattern          pattern(IndexType mosId1, IndexType mosId2);

private:
    Netlist &           _netlist;

private:
    bool                matchedType(IndexType mosId1, IndexType mosId2);
    bool                matchedSize(IndexType mosId1, IndexType mosId2);
    bool                diffPairInput(IndexType mosId1, IndexType mosId2);
    bool                diffPairCascode(IndexType mosId1, IndexType mosId2);
    bool                validPairCascode(IndexType mosId1, IndexType mosId2);
    bool                validPairLoad(IndexType mosId1, IndexType mosId2);
    bool                crossPairCascode(IndexType mosId1, IndexType mosId2);
    bool                crossPairLoad(IndexType mosId1, IndexType mosId2);
};

PROJECT_NAMESPACE_END

#endif
