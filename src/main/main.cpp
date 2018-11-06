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
    std::vector<SymDetect::diffPair> diffPair;
    diffPair =  symDetect.diffPairSearch();
    for (SymDetect::diffPair pair : diffPair)
    {
        std::cout << netlist.instance(pair.diff.first).name() << " " << netlist.instance(pair.diff.second).name() << std::endl;
        std::vector<std::pair<IndexType, IndexType>> symGroup = symDetect.symGroup(pair.diff);
    }
    return 0; 
}

#endif
