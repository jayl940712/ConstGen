#include <cstdio>
#include <fstream>
#include "InitNetlist.h"

PROJECT_NAMESPACE_BEGIN

bool InitNetlist::read(const std::string &fileName)
{
    Netlist::InitDataObject obj; 
    std::ifstream inFile(fileName.c_str());
    if (!inFile.is_open())
    {
        std::printf("Cannot open file: %s \n", fileName.c_str());
        return false;
    }
    std::string inLine;
    while (inFile >> inLine)
    {
        if (inLine == "Inst")
        {
            IndexType id;
            inFile >> id;
            inFile >> inLine;
            if (inLine == "PMOS" || inLine == "NMOS" || inLine == "CAP" || inLine == "RES" || inLine == "OTHER")
            {
                Netlist::InitInstance inst;
                InstanceType instType;
                if (inLine == "PMOS")
                    instType = InstanceType::PMOS;
                else if (inLine == "NMOS")
                    instType = InstanceType::NMOS;
                else if (inLine == "CAP")
                    instType = InstanceType::CAP;
                else if (inLine == "RES")
                    instType = InstanceType::RES;
                else
                    instType = InstanceType::OTHER;
                inst.type = instType;
                inFile >> inst.name; 
                if (inst.type == InstanceType::CAP)
                {
                    inFile >> inst.width;
                    inst.length = 0.0;
                }
                else if (inst.type != InstanceType::OTHER)
                    inFile >> inst.width >> inst.length;
                IndexType netId;
                while (inFile >> netId)
                    inst.netIdArray.push_back(netId);
                obj.instanceArray.push_back(inst);
            }
            else
            {
                std::printf("Invalid Syntex in file: %s \n", fileName.c_str());
            }
        }
        else if (inLine == "NET")
        {
            Netlist::InitNet net;
            inFile >> net.id; 
            inFile >> net.name;
            obj.netArray.push_back(net);
        }
        else 
            std::printf("Invalid Syntex in file: %s \n", fileName.c_str()); 
        inFile.clear();
    }
    _netlistDB.init(obj);
    return true;
}

PROJECT_NAMESPACE_END
