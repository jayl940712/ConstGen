#include "db/Net.h"

PROJECT_NAMESPACE_BEGIN

static const std::set<std::string> POWER_NET_NAMES = {"vdd", "VDD", "Vdd", "VDDA", "vdda", "Vdda"};
static const std::set<std::string> GROUND_NET_NAMES = {"vss", "VSS", "Vss", "VSSA", "vssa", "Vssa", "gnd", "Gnd", "GND"};

NetType Net::netType() const
{
    if ( POWER_NET_NAMES.count(_name) )
        return NetType::POWER;
    if ( GROUND_NET_NAMES.count(_name) ) 
        return NetType::GROUND; 
    return NetType::SIGNAL;
}

PROJECT_NAMESPACE_END
