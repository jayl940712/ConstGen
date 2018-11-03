#include <string>
#include <iostream>
#include "parser/InitNetlist.h"

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
    return 0; 
}

#endif
