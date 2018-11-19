#ifndef __SYMDETECT_H__
#define __SYMDETECT_H__

#include "db/Netlist.h"
#include "sym_detect/Pattern.h"
#include <vector>

PROJECT_NAMESPACE_BEGIN

class SymDetect
{
public:
    explicit SymDetect(Netlist & netlist)
        : _netlist(netlist), _pattern(Pattern(netlist))
    {}

    void                                getDiffPairNetConn(std::vector<MosPair> & diffPair, IndexType netId);
    void                                getDiffPair(std::vector<MosPair> & diffPair);
    void                                dfsDiffPair(std::vector<MosPair> & dfsVstPair, MosPair & diffPair);

private:
    struct srchObj
    {
        MosPair pair;
        PinType srchPinType;
        srchObj(MosPair & diffPair, PinType pinType) 
            : pair(diffPair), srchPinType(pinType) 
        {}
    };

private:
    Netlist & _netlist;
    Pattern   _pattern;

    bool                                endSrch(IndexType mosId, PinType pinType);
    bool                                existPair(std::vector<MosPair> & library, IndexType instId1, IndexType instId2);
    bool                                existPair(std::vector<srchObj> & library, IndexType instId1, IndexType instId2);

    MosPattern                          srchObjPtrn(srchObj & obj);
    bool                                endSrch(srchObj & obj);
    bool                                validSrchObj(IndexType instId1, IndexType instId2, IndexType srchPinId1, IndexType srchPinId2);
    void                                pushNextSrchObj(std::vector<MosPair> & dfsVstPair, std::vector<srchObj> & dfsStack, srchObj & currObj);
}; 

PROJECT_NAMESPACE_END

#endif
