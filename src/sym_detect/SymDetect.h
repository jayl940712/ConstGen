#ifndef __SYMDETECT_H__
#define __SYMDETECT_H__

#include "db/Netlist.h"
#include "sym_detect/Pattern.h"
#include <vector>

PROJECT_NAMESPACE_BEGIN

class SymDetect
{
public:
    explicit SymDetect(const Netlist & netlist)
        : _netlist(netlist), _pattern(Pattern(netlist))
    {}

    void                        hiSymDetect(std::vector<std::vector<MosPair>> & symGroup) const;

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
    const Netlist & _netlist;
    Pattern   _pattern;

    MosPattern                  srchObjPtrn(srchObj & obj) const;
    bool                        endSrch(IndexType mosId, PinType pinType) const;
    bool                        existPair(std::vector<MosPair> & library, IndexType instId1, IndexType instId2) const;
    bool                        existPair(std::vector<srchObj> & library, IndexType instId1, IndexType instId2) const;
    bool                        endSrch(srchObj & obj) const;
    bool                        validSrchObj(IndexType instId1, IndexType instId2, 
                                             IndexType srchPinId1, IndexType srchPinId2) const;
    void                        pushNextSrchObj(std::vector<MosPair> & dfsVstPair, std::vector<srchObj> & dfsStack, 
                                                srchObj & currObj, std::vector<MosPair> & diffPairSrc) const;
    void                        getDiffPairNetConn(std::vector<MosPair> & diffPair, IndexType netId) const;
    void                        getDiffPair(std::vector<MosPair> & diffPair) const;
    void                        dfsDiffPair(std::vector<MosPair> & dfsVstPair, MosPair & diffPair, 
                                             std::vector<MosPair> & diffPairSrch) const;
    void                        inVldDiffPairSrch(std::vector<MosPair> & diffPairSrch, MosPair & currPair) const;
}; 

PROJECT_NAMESPACE_END

#endif
