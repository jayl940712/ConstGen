#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include <string>
#include <vector>
#include "global/type.h"

PROJECT_NAMESPACE_BEGIN

class Instance
{
public:
    explicit Instance() = default;
    explicit Instance(const std::string &name, InstanceType type, IndexType id) 
        : _name(name), _type(type), _id(id) 
    {}

    // Getters
    const std::string &             name() const                        {return _name; }
    InstanceType                    type() const                        {return _type; } 
    IndexType                       id() const                          {return _id; }
    std::vector<IndexType> &        pinIdArray()                        {return _pinIdArray; }

    // Setters
    void                            addPinId(IndexType pinId)           { _pinIdArray.push_back(pinId); }

protected:
    std::string                     _name;
    InstanceType                    _type;
    IndexType                       _id;
    std::vector<IndexType>          _pinIdArray;                        
};

class Mosfet : public Instance
{
public:
    explicit Mosfet(const std::string &name, InstanceType type, IndexType id) 
        : Instance(name, type, id)
    {} 
    // Getters
    IndexType                       sourcePinId() const                 {return _sourcePinId; }
    IndexType                       drainPinId() const                  {return _drainPinId; }
    IndexType                       gatePinId() const                   {return _gatePinId; }
    IndexType                       bulkPinId() const                   {return _bulkPinId; }

    // Setters
    void                            setSourcePinId(IndexType pinId)     { _sourcePinId = pinId; addPinId(pinId); }
    void                            setDrainPinId(IndexType pinId)      { _drainPinId = pinId; addPinId(pinId); }
    void                            setGatePinId(IndexType pinId)       { _gatePinId = pinId; addPinId(pinId); } 
    void                            setBulkPinId(IndexType pinId)       { _bulkPinId = pinId; addPinId(pinId); }

private:
    IndexType                       _sourcePinId;
    IndexType                       _drainPinId;
    IndexType                       _gatePinId;
    IndexType                       _bulkPinId;
};

class Resistor : public Instance
{
public:
    explicit Resistor(const std::string &name, InstanceType type, IndexType id) 
        : Instance(name, type, id)
    {}
    // Getters
    IndexType                       thisPinId() const                   {return _thisPinId; }
    IndexType                       thatPinId() const                   {return _thatPinId; }                   

    // Setters
    void                            setThisPinId(IndexType pinId)       { _thisPinId = pinId; addPinId(pinId); }
    void                            setThatPinId(IndexType pinId)       { _thatPinId = pinId; addPinId(pinId); }

private:
    IndexType                       _thisPinId;
    IndexType                       _thatPinId; 
};

class Capacitor : public Instance
{
public:
    explicit Capacitor(const std::string &name, InstanceType type, IndexType id) 
        : Instance(name, type, id)
    {}
    // Getters
    IndexType                       thisPinId() const                   {return _thisPinId; }
    IndexType                       thatPinId() const                   {return _thatPinId; }                   

    // Setters
    void                            setThisPinId(IndexType pinId)       { _thisPinId = pinId; addPinId(pinId); }
    void                            setThatPinId(IndexType pinId)       { _thatPinId = pinId; addPinId(pinId); }

private:
    IndexType                       _thisPinId;
    IndexType                       _thatPinId; 
};

PROJECT_NAMESPACE_END

#endif
