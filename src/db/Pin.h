#ifndef __PIN_H__
#define __PIN_H__

#include <string>
#include "global/type.h"

PROJECT_NAMESPACE_BEGIN

class Pin
{
public:
    explicit Pin() = default;
    explicit Pin(IndexType id, IndexType instId, IndexType netId, PinType type)  
        : _id(id), _instId(instId), _netId(netId), _type(type)
    {}

    // Getters
    IndexType                       id() const          { return _id; }
    IndexType                       instId() const      { return _instId; }
    IndexType                       netId() const       { return _netId; }
    PinType                         type() const        { return _type; }

    // Setters
    
    static PinType                  nextPinType(PinType type);

private:
    IndexType                       _id;
    IndexType                       _instId;
    IndexType                       _netId;          
    PinType                         _type;
};

PROJECT_NAMESPACE_END

#endif

