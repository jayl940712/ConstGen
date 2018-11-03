#include "db/Netlist.h"
#include <cstdio>

PROJECT_NAMESPACE_BEGIN

void Netlist::init(InitDataObject &obj)
{
    for (Netlist::InitNet & net : obj.netArray )
        _netArray.push_back( Net(net.name, net.id) );
    for (InitInstance instance : obj.instanceArray )
    {
        if (instance.type == InstanceType::PMOS)
        {
            Mosfet inst(instance.name, instance.type, _instanceArray.size());
            inst.setSourcePinId(_pinArray.size());
            _netArray[instance.netIdArray.at(0)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(0), PinType::SOURCE));    
            inst.setGatePinId(_pinArray.size());
            _netArray[instance.netIdArray.at(1)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(1), PinType::GATE));    
            inst.setDrainPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(2)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(2), PinType::DRAIN));    
            inst.setBulkPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(3)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(3), PinType::BULK));    
            _instanceArray.push_back(inst);
        }
        else if (instance.type == InstanceType::NMOS)
        {
            Mosfet inst(instance.name, instance.type, _instanceArray.size());
            inst.setSourcePinId(_pinArray.size());
            _netArray[instance.netIdArray.at(0)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(0), PinType::SOURCE));    
            inst.setGatePinId(_pinArray.size());
            _netArray[instance.netIdArray.at(1)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(1), PinType::GATE));    
            inst.setDrainPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(2)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(2), PinType::DRAIN));    
            inst.setBulkPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(3)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(3), PinType::BULK));    
            _instanceArray.push_back(inst);
        } 
        else if (instance.type == InstanceType::RES)
        {
            Resistor inst(instance.name, instance.type, _instanceArray.size());
            inst.setThisPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(0)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(0), PinType::THIS));
            inst.setThatPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(1)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(1), PinType::THAT));
            inst.addPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(2)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(2), PinType::OTHER));
            _instanceArray.push_back(inst);
        }
        else if (instance.type == InstanceType::CAP)
        {
            Capacitor inst(instance.name, instance.type, _instanceArray.size());
            inst.setThisPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(0)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(0), PinType::THIS));
            inst.setThatPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(1)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(1), PinType::THAT));
            inst.addPinId(_pinArray.size());
            _netArray[instance.netIdArray.at(2)].addPinId(_pinArray.size());
            _pinArray.push_back(Pin(_pinArray.size(), inst.id(), instance.netIdArray.at(2), PinType::OTHER));
            _instanceArray.push_back(inst);
        }
        else
        {
            Instance inst(instance.name, instance.type, _instanceArray.size());
            for (IndexType netId : instance.netIdArray)
            {
                inst.addPinId(_pinArray.size());
                _netArray[netId].addPinId(_pinArray.size());
                _pinArray.push_back(Pin(_pinArray.size(), inst.id(), netId, PinType::OTHER));                
            }    
            _instanceArray.push_back(inst);
        } 
    }
}

void Netlist::print_all()
{
    for (Net & net : _netArray)
    {
        std::printf("Net %d, %s \n", net.id(), net.name().c_str());
    }
    for (Instance & inst : _instanceArray)
    {
        std::printf("Instance %d, %s \n", inst.id(), inst.name().c_str()); 
        for (IndexType pinId : inst.pinIdArray())
            std::printf("Pin %d, from  Instance %s to net %d:%s \n", pinId, _instanceArray[_pinArray[pinId].instanceId()].name().c_str(), _pinArray[pinId].netId(), _netArray[_pinArray[pinId].netId()].name().c_str()); 
    }   
}

PROJECT_NAMESPACE_END


