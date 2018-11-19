#include "db/Pin.h"

PROJECT_NAMESPACE_BEGIN

PinType Pin::nextPinType(PinType type)
{
    if (type == PinType::SOURCE)
        return PinType::DRAIN;
    if (type == PinType::DRAIN)
        return PinType::SOURCE;
    if (type == PinType::THIS)
        return PinType::THAT;
    if (type == PinType::THAT)
        return PinType::THIS;
    return PinType::OTHER;
}


PROJECT_NAMESPACE_END

