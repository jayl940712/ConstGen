/**
 * @file GlobalAPI.cpp
 * @brief The Python interface for the classes defined in global.h
 * @author Mingjie Liu
 * @date 03/18/2020
 */

#include <pybind11/pybind11.h>
#include "global/global.h"

namespace py = pybind11;

void initGlobalAPI(py::module &m)
{
    // See https://stackoverflow.com/questions/47893832/pybind11-global-level-enum
    py::enum_<PROJECT_NAMESPACE::InstType>(m, "InstType")
        .value("Res", PROJECT_NAMESPACE::InstType::RES)
        .value("Pch", PROJECT_NAMESPACE::OriType::PMOS)
        .value("Nch", PROJECT_NAMESPACE::OriType::NMOS)
        .value("Cap", PROJECT_NAMESPACE::OriType::CAP)
        .value("OTHER", PROJECT_NAMESPACE::OriType::OTHER)
        .export_values();

    py::enum_<PROJECT_NAMESPACE::PinType>(m, "PinType")
        .value("D", PROJECT_NAMESPACE::PinType::DRAIN)
        .value("G", PROJECT_NAMESPACE::PinType::GATE)
        .value("S", PROJECT_NAMESPACE::PinType::SOURCE)
        .value("B", PROJECT_NAMESPACE::PinType::BULK)
        .value("THIS", PROJECT_NAMESPACE::PinType::THIS)
        .value("THAT", PROJECT_NAMESPACE::PinType::THAT)
        .value("OTHER", PROJECT_NAMESPACE::PinType::OTHER)
        .export_values();
}
