#ifndef __INITNETLIST_H__
#define __INITNETLIST_H__

#include "db/Netlist.h"

PROJECT_NAMESPACE_BEGIN

class InitNetlist
{
public:
    explicit InitNetlist() = default;
    explicit InitNetlist(Netlist &netlist) : _netlistDB(netlist) {}
    
    bool read(const std::string &filename);
     
private:
    Netlist & _netlistDB; 
};


PROJECT_NAMESPACE_END

#endif
