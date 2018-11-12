#include "sym_detect/Pattern.h"

PROJECT_NAMESPACE_BEGIN

Pattern::MosPair Pattern::constructPair(std::pair<IndexType, IndexType> pair, PinType pinType)
{
    MosPair mosfetPair;
    mosfetPair.mosId1 = pair.first;
    mosfetPair.mosId2 = pair.second;
    mosfetPair.inputPinId1 = _netlist.pinId(pair.first, pinType);
    mosfetPair.inputPinId2 = _netlist.pinId(pair.second, pinType);
    mosfetPair.outputPinId1 = _netlist.pinId(pair.first, Pin::otherPinType(pinType));
    mosfetPair.outputPinId2 = _netlist.pinId(pair.second, Pin::otherPinType(pinType));
    mosfetPair.inputNetId1 = _netlist.pin(mosfetPair.inputPinId1).netId();
    mosfetPair.inputNetId2 = _netlist.pin(mosfetPair.inputPinId2).netId();
    mosfetPair.outputNetId1 = _netlist.pin(mosfetPair.outputPinId1).netId();
    mosfetPair.outputNetId2 = _netlist.pin(mosfetPair.outputPinId2).netId();
    mosfetPair.gateNetId1 = _netlist.pin(_netlist.pinId(pair.first, PinType::GATE)).netId();
    mosfetPair.gateNetId2 = _netlist.pin(_netlist.pinId(pair.second, PinType::GATE)).netId();
    return mosfetPair;
}

bool Pattern::matchedSize(MosPair & pair)
{
    if(_netlist.instance(pair.mosId1).width() != _netlist.instance(pair.mosId2).width())
        return false;
    if(_netlist.instance(pair.mosId1).length() != _netlist.instance(pair.mosId2).length())
        return false;
    return true;
}

bool Pattern::diffPairInput(MosPair & pair)
{
    if(_netlist.instance(pair.mosId1).type() != _netlist.instance(pair.mosId2).type())
        return false;
    if(pair.inputNetId1 == pair.inputNetId2 && pair.gateNetId1 != pair.gateNetId2 && pair.outputNetId1 != pair.outputNetId2 && _netlist.mosType(pair.mosId1) == MosType::DIFF && _netlist.mosType(pair.mosId2) == MosType::DIFF)
        return true;
    return false;
}

bool Pattern::diffPairCascode(MosPair & pair)
{
    if(_netlist.instance(pair.mosId1).type() != _netlist.instance(pair.mosId2).type())
        return false;
    if(pair.inputNetId1 != pair.inputNetId2 && pair.gateNetId1 != pair.gateNetId2 && pair.outputNetId1 != pair.outputNetId2 && _netlist.mosType(pair.mosId1) == MosType::DIFF && _netlist.mosType(pair.mosId2) == MosType::DIFF)
        return true;
    return false;
}

bool Pattern::validPairCascode(MosPair & pair)
{
    if(_netlist.instance(pair.mosId1).type() != _netlist.instance(pair.mosId2).type())
        return false;
    if(_netlist.mosType(pair.mosId1) != MosType::DIFF && _netlist.mosType(pair.mosId1) != MosType::DIODE )
        return false;
    if(_netlist.mosType(pair.mosId2) != MosType::DIFF && _netlist.mosType(pair.mosId2) != MosType::DIODE )
        return false;
    if(pair.inputNetId1 != pair.inputNetId2 && pair.gateNetId1 == pair.gateNetId2 && pair.outputNetId1 != pair.outputNetId2)
        return true;
    return false;
}

bool Pattern::validPairLoad(MosPair & pair)
{
    if(_netlist.instance(pair.mosId1).type() != _netlist.instance(pair.mosId2).type())
        return false;
    if(_netlist.mosType(pair.mosId1) != MosType::DIFF && _netlist.mosType(pair.mosId1) != MosType::DIODE )
        return false;
    if(_netlist.mosType(pair.mosId2) != MosType::DIFF && _netlist.mosType(pair.mosId2) != MosType::DIODE )
        return false;
    if(pair.inputNetId1 == pair.inputNetId2 && pair.gateNetId1 == pair.gateNetId2 && pair.outputNetId1 != pair.outputNetId2)
        return true;
    return false;
}  

bool Pattern::crossPairCascode(MosPair & pair)
{
    if(_netlist.instance(pair.mosId1).type() != _netlist.instance(pair.mosId2).type())
        return false;
    if(pair.inputNetId1 != pair.inputNetId2 && pair.gateNetId1 == pair.outputNetId2 && pair.outputNetId1 == pair.gateNetId2 && _netlist.mosType(pair.mosId1) == MosType::DIFF && _netlist.mosType(pair.mosId2) == MosType::DIFF)
        return true;
    return false;
}

bool Pattern::crossPairLoad(MosPair & pair)
{
    if(_netlist.instance(pair.mosId1).type() != _netlist.instance(pair.mosId2).type())
        return false;
    if(pair.inputNetId1 == pair.inputNetId2 && pair.gateNetId1 == pair.outputNetId2 && pair.outputNetId1 == pair.gateNetId2 && _netlist.mosType(pair.mosId1) == MosType::DIFF && _netlist.mosType(pair.mosId2) == MosType::DIFF)
        return true;
    return false;
}

MosPattern Pattern::pattern(std::pair<IndexType, IndexType> pair)
{
    MosPair mosPair = constructPair(pair, PinType::SOURCE);
    if (!matchedSize(mosPair))
        return MosPattern::INVALID;
    if (crossPairCascode(mosPair))
        return MosPattern::CROSS_CASCODE;
    if (crossPairLoad(mosPair))
        return MosPattern::CROSS_LOAD;
    if (validPairCascode(mosPair))
        return MosPattern::CASCODE;
    if (validPairLoad(mosPair))
        return MosPattern::LOAD;
    if (diffPairInput(mosPair))
        return MosPattern::DIFF_SOURCE;
    if (diffPairCascode(mosPair))
        return MosPattern::DIFF_CASCODE;
    return MosPattern::INVALID; 
}

PROJECT_NAMESPACE_END
