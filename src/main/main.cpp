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
    netlist.print_all();
    std::cout << "Start diff pair recognition." << netlist.numNet() << std::endl;
    std::vector<MosPair> diffPair;
    SymDetect symDetect(netlist);
    symDetect.getDiffPair(diffPair);
    std::cout << "Diff pair recognized." << std::endl;
    for (MosPair & pair : diffPair)
    {
        std::vector<MosPair> symGroup;
        symDetect.dfsDiffPair(symGroup, pair);
        std::cout << "BEGIN GROUP!!!" << std::endl;
        for (MosPair & matchedPair : symGroup)
        {
            std::cout << netlist.inst(matchedPair.mosId1).name() << " " << netlist.inst(matchedPair.mosId2).name() << std::endl;
        }
        std::cout << "END GROUP!!!" << std::endl;
    }
    return 0; 
}

#endif
