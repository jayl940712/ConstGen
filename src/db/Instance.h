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
    explicit Instance(const std::string &name, InstanceType type, IndexType id, RealType width, RealType length) 
        : _name(name), _type(type), _id(id), _width(width), _length(length) 
    {}

    // Getters
    const std::string &             name() const                        {return _name; }
    InstanceType                    type() const                        {return _type; } 
    IndexType                       id() const                          {return _id; }
    std::vector<IndexType> &        pinIdArray()                        {return _pinIdArray; }
    RealType                        width() const                       {return _width; }
    RealType                        length() const                      {return _length; }

    // Setters
    void                            addPinId(IndexType pinId)           { _pinIdArray.push_back(pinId); }
    void                            setWidth(RealType width)            { _width = width; }
    void                            setLength(RealType length)          { _length = length; }

private:
    std::string                     _name;
    InstanceType                    _type;
    IndexType                       _id;
    std::vector<IndexType>          _pinIdArray;                        
    RealType                        _width;
    RealType                        _length;
};


PROJECT_NAMESPACE_END

#endif
