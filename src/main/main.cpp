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
    SymDetect symDetect(netlist);
    std::cout << "Start diff pair recognition." << std::endl;
    std::vector<std::pair<IndexType, IndexType>>  diffPair = symDetect.diffPairSearch();
    std::cout << "Diff pair recognized." << std::endl;
    for (std::pair<IndexType, IndexType> pair : diffPair)
    {
        std::cout << netlist.instance(pair.first).name() << " " << netlist.instance(pair.second).name() << std::endl;
        std::vector<std::pair<IndexType, IndexType>> symGroup = symDetect.symGroup(pair);
        std::cout << "BEGIN GROUP!!!" << std::endl;
        for (std::pair<IndexType, IndexType> matchedPair : symGroup)
        {
            std::cout << netlist.instance(matchedPair.first).name() << " " << netlist.instance(matchedPair.second).name() << std::endl;
        }
        std::cout << "END GROUP!!!" << std::endl;
    }
    return 0; 
}

#endif
