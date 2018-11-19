#ifndef __INST_H__
#define __INST_H__

#include <string>
#include <vector>
#include "global/type.h"

PROJECT_NAMESPACE_BEGIN

class Inst
{
public:
    explicit Inst() = default;
    explicit Inst(const std::string &name, InstType type, IndexType id) 
        : _name(name), _type(type), _id(id) 
    {}
    explicit Inst(const std::string &name, InstType type, IndexType id, RealType wid, RealType len) 
        : _name(name), _type(type), _id(id), _wid(wid), _len(len) 
    {}

    // Getters
    const std::string &             name() const                        { return _name; }
    InstType                        type() const                        { return _type; } 
    IndexType                       id() const                          { return _id; }
    std::vector<IndexType> &        pinIdArray()                        { return _pinIdArray; }
    RealType                        wid() const                         { return _wid; }
    RealType                        len() const                         { return _len; }

    // Setters
    void                            addPinId(IndexType pinId)           { _pinIdArray.push_back(pinId); }
    void                            setWid(RealType wid)                { _wid = wid; }
    void                            setLen(RealType len)                { _len = len; }

private:
    std::string                     _name;
    InstType                        _type;
    IndexType                       _id;
    std::vector<IndexType>          _pinIdArray;                        
    RealType                        _wid;
    RealType                        _len;
};


PROJECT_NAMESPACE_END

#endif
