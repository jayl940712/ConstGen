#ifndef __PATTERN_H__
#define __PATTERN_H__

#include "db/Netlist.h"
#include "global/type.h"

PROJECT_NAMESPACE_BEGIN

class Pattern
{
public:
    explicit Pattern(const Netlist & netlist) 
        : _netlist(netlist)
    {}

    MosPattern          pattern(IndexType mosId1, IndexType mosId2) const;

private:
    const Netlist &     _netlist;

private:
    bool                matchedType(IndexType mosId1, IndexType mosId2) const;
    bool                matchedSize(IndexType mosId1, IndexType mosId2) const;
    bool                diffPairInput(IndexType mosId1, IndexType mosId2) const;
    bool                diffPairCascode(IndexType mosId1, IndexType mosId2) const;
    bool                validPairCascode(IndexType mosId1, IndexType mosId2) const;
    bool                validPairLoad(IndexType mosId1, IndexType mosId2) const;
    bool                crossPairCascode(IndexType mosId1, IndexType mosId2) const;
    bool                crossPairLoad(IndexType mosId1, IndexType mosId2) const;
};

PROJECT_NAMESPACE_END

#endif
