#include <string>
#include <iostream>
#include <vector>
#include "parser/InitNetlist.h"
#include "sym_detect/SymDetect.h"

#ifndef __SFA_TEST__
#define __SFA_TEST__

int main(int argc, char* argv[])
{
    using namespace PROJECT_NAMESPACE;
    Netlist netlist;
    InitNetlist parser = InitNetlist(netlist);
    std::string inFile(argv[1]);
    parser.read(inFile);
    std::cout << "Done!" << std::endl;
//    netlist.print_all();
    SymDetect symDetect(netlist);
    std::vector<std::vector<MosPair>> symGroup;
    symDetect.hiSymDetect(symGroup);
    for (std::vector<MosPair> & diffPair : symGroup)
    {
        std::cout << "BEGIN GROUP" << std::endl;
        for (MosPair & pair : diffPair)
        {
            std::cout << netlist.inst(pair.mosId1).name() << " " << netlist.inst(pair.mosId2).name() << std::endl;
        }
        std::cout << "END GROUP" << std::endl;
    }
    return 0; 
}

#endif
