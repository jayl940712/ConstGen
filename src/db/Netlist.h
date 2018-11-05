#ifndef __NETLIST_H__
#define __NETLIST_H__

#include <vector>
#include <string>
#include "db/Net.h"
#include "db/Pin.h"
#include "db/Instance.h"

PROJECT_NAMESPACE_BEGIN

class Netlist
{
public:
    struct InitNet
    {
        std::string                 name;
        IndexType                   id;
    };
    struct InitInstance
    {
        InstanceType                type;
        std::vector<IndexType>      netIdArray;
        std::string                 name; 
    };
    struct InitDataObject
    {
        std::vector<InitNet>        netArray;
        std::vector<InitInstance>   instanceArray;
    };

public:
    explicit Netlist() = default;
    
    void                            init(InitDataObject &obj);
    void                            print_all();

    std::vector<IndexType>          pinInstanceId(IndexType pinId);
    std::vector<IndexType>          pinInstanceId(IndexType pinId, InstanceType type);
    std::vector<IndexType>          pinMosfetId(IndexType pinId);
    std::vector<IndexType>          netMosfetId(IndexType netId, PinType pinType, MosType mosType);
    IndexType                       instanceNetId(IndexType instanceId, PinType pinType);
    MosType                         mosType(IndexType mosId);
    
    // Getters
    Pin &                           pin(IndexType id)                   {return _pinArray.at(id); }
    Net &                           net(IndexType id)                   {return _netArray.at(id); }
    Instance &                      instance(IndexType id)              {return _instanceArray.at(id); }
    IndexType                       numPins() const                     {return _pinArray.size(); }
    IndexType                       numNets() const                     {return _netArray.size(); }
    IndexType                       numInstance() const                 {return _instanceArray.size(); }

    // Setters
    void                            addPin(Pin & pin)             { _pinArray.push_back(pin); }
    void                            addNet(Net & net)             { _netArray.push_back(net); }
    void                            addInstance(Instance & inst)  { _instanceArray.push_back(inst); }

private:    
    std::vector<Net>                _netArray;
    std::vector<Pin>                _pinArray;
    std::vector<Instance>           _instanceArray;
};

PROJECT_NAMESPACE_END

#endif
