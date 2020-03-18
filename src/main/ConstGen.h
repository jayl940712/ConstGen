/*! @file main/ConstGen.cpp
    @brief ConstGen.cpp
    @author Mingjie LIu
    @date 03/18/2020

    Reimplementing interface for python.
*/
#ifndef __CONST_GEN__
#define __CONST_GEN__

#include <string>

PROJECT_NAMESPACE_BEGIN

/*! @class ConstGen
    @brief ConstGen class
*/
class ConstGen
{
public:
/*! @brief Default Constructor */
    explicit ConstGen() = default;

/*! @brief Read netlist file throught parser initNetlist */
    void    readNetlistFile(const std::string & fileName) { _parser.read(fileName); }
    
/*! @brief Add a net to netlist. */
    IndexType   addNet(const std::string name, IndexType netIdx) { _netlistDB.addNet(name, netIdx); }

/*! @brief Add a instance to netlist. */
    IndexType   addInst(const std::string name, InstType type, RealType wid, RealType len, RealType nf) { _netlistDB.addInst(name, type, wid, len, nf); }

/*! @brief Add a pin to inst 
    @param Index of inst in _instArray
    @param Index of net connected to pin.
    @param PinType of added pin.
*/
    void        addInstPin(IndexType instIdx, IndexType netIdx, PinType pinType) { _netlistDB.addInstPin(instIdx, netIdx, pinType); }

 /*! @brief Dump result to file */
    void    dumpResult(const std::string & cirName) const
    {
        _symDetect.setNetlist(_netlistDB);
        _symDetect.print();
        _symDetect.dumpSym(cirName + ".sym");
        // Symmetry net constraint no longer needed in flow.
        // _symDetect.dumpNet(file + ".symnet"); 
    }
    
private:
    Netlist     _netlistDB; 
    InitNetlist _parser(_netlistDB);
    SymDetect   _symDetect;
};

PROJECT_NAMESPACE_END

#endif
