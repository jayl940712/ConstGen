/*! @file main/main.cpp
    @brief main.cpp
    @author Mingjie LIu
    @date 11/25/2018

    Takes 1 argument input. Parse the file
    into Netlist. Detect hierarchy symmetry
    groups and print to command line.
    Input file should be of certain format.
    See parser/InitNetlist.h for details.
*/
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
    parser.read(inFile); //initialize netlist with parser
    std::cout << "Done!" << std::endl;
//    netlist.print_all(); //print netlist
    SymDetect symDetect(netlist); //declare SymDetect
    std::vector<std::vector<MosPair>> symGroup;
    symDetect.hiSymDetect(symGroup); //call hiSymDetect
    for (std::vector<MosPair> & diffPair : symGroup) //print hiSym Groups
    {
        std::cout << "BEGIN GROUP" << std::endl;
        for (MosPair & pair : diffPair)
        {
            std::cout << netlist.inst(pair.mosId1()).name() << " " 
                << netlist.inst(pair.mosId2()).name() << std::endl;
        }
        std::cout << "END GROUP" << std::endl;
    }
    return 0; 
}

#endif
