#include "db/Net.h"

PROJECT_NAMESPACE_BEGIN

std::set<std::string> POWER_NET_NAMES = {"vdd", "VDD", "Vdd", "VDDA", "vdda", "Vdda"};
std::set<std::string> GROUND_NET_NAMES = {"vss", "VSS", "Vss", "VSSA", "vssa", "Vssa", "gnd", "Gnd", "GND"};

NetType Net::netType()
{
    if ( POWER_NET_NAMES.count(_name) )
        return NetType::POWER;
    if ( GROUND_NET_NAMES.count(_name) ) 
        return NetType::GROUND; 
    return NetType::SIGNAL;
}

PROJECT_NAMESPACE_END
