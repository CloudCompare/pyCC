//##########################################################################
//#                                                                        #
//#                                boost.Python                            #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Library General Public License as       #
//#  published by the Free Software Foundation; version 2 or later of the  #
//#  License.                                                              #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          Copyright 2021 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "converters.hpp"
#include "ScalarFieldPy.hpp"
#include "ccPointCloudPy.hpp"
#include "ccMeshPy.hpp"
#include "ccPrimitivesPy.hpp"
#include "ccPolylinePy.hpp"
#include "distanceComputationToolsPy.hpp"

#include "initCC.h"
#include "pyCC.h"
#include "PyScalarType.h"
#include <ScalarField.h>
#include <QString>
#include <vector>

#include "pyccTrace.h"
#include "cloudComPy_DocStrings.hpp"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

char const* greet()
{
   return "hello, world, this is CloudCompare Python Interface: 'CloudComPy'";
}

void initCC_py()
{
    PyObject *cc_module;
    cc_module = PyImport_ImportModule("cloudComPy");
    const char* modulePath = PyUnicode_AsUTF8(PyModule_GetFilenameObject(cc_module));
    CCTRACE("modulePath: " <<  modulePath);
    initCC::init(modulePath);
}

const char* getScalarType()
{
    //Get the scalar type used in cloudCompare under the form defined in Numpy: 'float32' or 'float64'
    return CC_NPY_FLOAT_STRING;
}


BOOST_PYTHON_FUNCTION_OVERLOADS(loadPointCloud_overloads, loadPointCloud, 1, 6);
BOOST_PYTHON_FUNCTION_OVERLOADS(loadPolyline_overloads, loadPolyline, 1, 6);
BOOST_PYTHON_FUNCTION_OVERLOADS(GetPointCloudRadius_overloads, GetPointCloudRadius, 1, 2);

BOOST_PYTHON_MODULE(cloudComPy)
{
    using namespace boost::python;

    bnp::initialize();
    initializeConverters();

    export_ScalarField();
    export_ccPolyline();
    export_ccPointCloud();
    export_ccMesh();
    export_ccPrimitives();
    export_distanceComputationTools();

    // TODO: function load entities ("file.bin")
    // TODO: more methods on distanceComputationTools
    // TODO: methods from ccEntityAction.h to transpose without dialogs
    // TODO: compute octree
    // TODO: mesh Delaunay
    // TODO: explore menus edit, tools, plugins

    scope().attr("__doc__") = cloudComPy_doc;

    def("greet", greet);

    enum_<CC_SHIFT_MODE>("CC_SHIFT_MODE")
        .value("AUTO", AUTO)
        .value("XYZ", XYZ)
        ;

    enum_<CC_FILE_ERROR>("CC_FILE_ERROR")
        .value("CC_FERR_NO_ERROR", CC_FERR_NO_ERROR)
        .value("CC_FERR_BAD_ARGUMENT", CC_FERR_BAD_ARGUMENT)
        .value("CC_FERR_UNKNOWN_FILE", CC_FERR_UNKNOWN_FILE)
        .value("CC_FERR_WRONG_FILE_TYPE", CC_FERR_WRONG_FILE_TYPE)
        .value("CC_FERR_WRITING", CC_FERR_WRITING)
        .value("CC_FERR_READING", CC_FERR_READING)
        .value("CC_FERR_NO_SAVE", CC_FERR_NO_SAVE)
        .value("CC_FERR_NO_LOAD", CC_FERR_NO_LOAD)
        .value("CC_FERR_BAD_ENTITY_TYPE", CC_FERR_BAD_ENTITY_TYPE)
        .value("CC_FERR_CANCELED_BY_USER", CC_FERR_CANCELED_BY_USER)
        .value("CC_FERR_NOT_ENOUGH_MEMORY", CC_FERR_NOT_ENOUGH_MEMORY)
        .value("CC_FERR_MALFORMED_FILE", CC_FERR_MALFORMED_FILE)
        .value("CC_FERR_CONSOLE_ERROR", CC_FERR_CONSOLE_ERROR)
        .value("CC_FERR_BROKEN_DEPENDENCY_ERROR", CC_FERR_BROKEN_DEPENDENCY_ERROR)
        .value("CC_FERR_FILE_WAS_WRITTEN_BY_UNKNOWN_PLUGIN", CC_FERR_FILE_WAS_WRITTEN_BY_UNKNOWN_PLUGIN)
        .value("CC_FERR_THIRD_PARTY_LIB_FAILURE", CC_FERR_THIRD_PARTY_LIB_FAILURE)
        .value("CC_FERR_THIRD_PARTY_LIB_EXCEPTION", CC_FERR_THIRD_PARTY_LIB_EXCEPTION)
        .value("CC_FERR_NOT_IMPLEMENTED", CC_FERR_NOT_IMPLEMENTED)
        ;

    enum_<CurvatureType>("CurvatureType")
        .value("GAUSSIAN_CURV", GAUSSIAN_CURV)
        .value("MEAN_CURV", MEAN_CURV)
        .value("NORMAL_CHANGE_RATE", NORMAL_CHANGE_RATE)
        ;

    def("loadPointCloud", loadPointCloud,
        loadPointCloud_overloads(args("mode", "skip", "x", "y", "z", "filename"),
                                 cloudComPy_loadPointCloud_doc)[return_value_policy<reference_existing_object>()]);

    def("loadPolyline", loadPolyline,
        loadPolyline_overloads(args("mode", "skip", "x", "y", "z", "filename"),
                               cloudComPy_loadPolyline_doc)
        [return_value_policy<reference_existing_object>()]);

    def("SavePointCloud", SavePointCloud, cloudComPy_SavePointCloud_doc);

    def("SaveEntities", SaveEntities, cloudComPy_SaveEntities_doc);

    def("initCC", &initCC_py, cloudComPy_initCC_doc);

    def("computeCurvature", computeCurvature, cloudComPy_computeCurvature_doc);

    def("filterBySFValue", filterBySFValue, return_value_policy<reference_existing_object>(), cloudComPy_filterBySFValue_doc);

    def("GetPointCloudRadius", GetPointCloudRadius, GetPointCloudRadius_overloads(args("knn", "clouds"), cloudComPy_GetPointCloudRadius_doc));

    def("getScalarType", getScalarType, cloudComPy_getScalarType_doc);
}
