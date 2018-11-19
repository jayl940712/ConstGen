#ifndef __NETLIST_H__
#define __NETLIST_H__

#include <vector>
#include <string>
#include "global/type.h"
#include "db/Net.h"
#include "db/Pin.h"
#include "db/Inst.h"

PROJECT_NAMESPACE_BEGIN

class Netlist
{
public:
    struct InitNet
    {
        std::string                 name;
        IndexType                   id = INDEX_TYPE_MAX;
    };
    struct InitInst
    {
        InstType                    type = InstType::OTHER;
        std::vector<IndexType>      netIdArray;
        std::string                 name; 
        RealType                    wid = 0, len = 0;
    };
    struct InitDataObj
    {
        std::vector<InitNet>        netArray;
        std::vector<InitInst>       instArray;
    };
public:
    explicit Netlist() = default;
    
    void                            init(InitDataObj &obj);
    void                            print_all();

    bool                            isMos(InstType instType);
    bool                            isPasvDev(InstType instType);

    bool                            isSignal(IndexType netId)           { return _netArray[netId].netType() == NetType::SIGNAL; }

    MosType                         mosType(IndexType mosId);
    IndexType                       instNetId(IndexType instId, PinType pinType);
    IndexType                       instPinId(IndexType instId, PinType pinType);

    IndexType                       srcNetId(IndexType mosId)           { return instNetId(mosId, PinType::SOURCE); }
    IndexType                       drainNetId(IndexType mosId)         { return instNetId(mosId, PinType::DRAIN); }
    IndexType                       gateNetId(IndexType mosId)          { return instNetId(mosId, PinType::GATE); }

    PinType                         getPinTypeInstNetConn(IndexType instId, IndexType pinId);
    PinType                         getPinTypeInstPinConn(IndexType instId, IndexType pinId);

    void                            getInstNetConn(std::vector<IndexType> & instArray, IndexType netId);
    void                            getInstPinConn(std::vector<IndexType> & instArray, IndexType pinId);

    void                            rmvInstHasPin(std::vector<IndexType> & instArray, IndexType pinId);

    void                            fltrInstPinConnPinType(std::vector<IndexType> & instArray, IndexType pinId, PinType connPinType);
    void                            fltrInstNetConnPinType(std::vector<IndexType> & instArray, IndexType netId, PinType connPinType);
    void                            fltrInstMosType(std::vector<IndexType> & instArray, MosType mosType);
   
    // Getters
    Pin &                           pin(IndexType id)                   { return _pinArray.at(id); }
    Net &                           net(IndexType id)                   { return _netArray.at(id); }
    Inst &                          inst(IndexType id)                  { return _instArray.at(id); }
    IndexType                       numPin() const                      { return _pinArray.size(); }
    IndexType                       numNet() const                      { return _netArray.size(); }
    IndexType                       numInst() const                     { return _instArray.size(); }

    // Setters
    void                            addPin(Pin & pin)                   { _pinArray.push_back(pin); }
    void                            addNet(Net & net)                   { _netArray.push_back(net); }
    void                            addInst(Inst & inst)                { _instArray.push_back(inst); }

private:    
    std::vector<Net>                _netArray;
    std::vector<Pin>                _pinArray;
    std::vector<Inst>               _instArray;
};

PROJECT_NAMESPACE_END

#endif
