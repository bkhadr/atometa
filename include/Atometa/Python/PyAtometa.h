#pragma once

#include "Atometa/Core/Core.h"
#include "Atometa/Chemistry/Molecule.h"
#include "Atometa/Chemistry/Atom.h"
#include "Atometa/Physics/ForceField.h"
#include "Atometa/Physics/MolecularDynamics.h"
#include "Atometa/Physics/EnergyMinimizer.h"

// Python C API
#include <Python.h>

namespace Atometa {
namespace Python {

    /**
     * Python API for Atometa
     * 
     * Usage in Python:
     * 
     * import atometa
     * 
     * # Create molecule
     * water = atometa.Molecule("Water")
     * o = water.add_atom("O", [0, 0, 0])
     * h1 = water.add_atom("H", [0.96, 0, 0])
     * h2 = water.add_atom("H", [-0.24, 0.93, 0])
     * water.add_bond(o, h1)
     * water.add_bond(o, h2)
     * 
     * # Energy minimization
     * minimizer = atometa.EnergyMinimizer()
     * minimizer.minimize(water)
     * 
     * # Molecular dynamics
     * md = atometa.MolecularDynamics()
     * md.set_temperature(300.0)
     * md.set_timestep(0.001)
     * md.run(water, steps=10000)
     * 
     * # Save
     * water.save("water.xyz")
     */

    class PyAtometa {
    public:
        static PyObject* Initialize();
        static void Shutdown();

    private:
        // Module methods
        static PyMethodDef s_Methods[];
        static PyModuleDef s_Module;

        // Molecule wrapper
        struct PyMolecule {
            PyObject_HEAD
            Molecule* molecule;
        };

        static PyTypeObject PyMoleculeType;
        static PyObject* PyMolecule_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
        static void PyMolecule_Dealloc(PyMolecule* self);
        static PyObject* PyMolecule_AddAtom(PyMolecule* self, PyObject* args);
        static PyObject* PyMolecule_AddBond(PyMolecule* self, PyObject* args);
        static PyObject* PyMolecule_GetFormula(PyMolecule* self);
        static PyObject* PyMolecule_GetEnergy(PyMolecule* self);
        static PyObject* PyMolecule_Save(PyMolecule* self, PyObject* args);
        static PyObject* PyMolecule_Load(PyMolecule* self, PyObject* args);

        // MD wrapper
        struct PyMD {
            PyObject_HEAD
            MolecularDynamics* md;
        };

        static PyTypeObject PyMDType;
        static PyObject* PyMD_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
        static void PyMD_Dealloc(PyMD* self);
        static PyObject* PyMD_SetTemperature(PyMD* self, PyObject* args);
        static PyObject* PyMD_SetTimeStep(PyMD* self, PyObject* args);
        static PyObject* PyMD_Run(PyMD* self, PyObject* args);
        static PyObject* PyMD_GetStatistics(PyMD* self);

        // Minimizer wrapper
        struct PyMinimizer {
            PyObject_HEAD
            EnergyMinimizer* minimizer;
        };

        static PyTypeObject PyMinimizerType;
        static PyObject* PyMinimizer_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
        static void PyMinimizer_Dealloc(PyMinimizer* self);
        static PyObject* PyMinimizer_Minimize(PyMinimizer* self, PyObject* args);
        static PyObject* PyMinimizer_SetAlgorithm(PyMinimizer* self, PyObject* args);

        // Utility functions
        static PyObject* CreateWater(PyObject* self, PyObject* args);
        static PyObject* CreateMethane(PyObject* self, PyObject* args);
        static PyObject* CreateBenzene(PyObject* self, PyObject* args);
        static PyObject* GetPeriodicTable(PyObject* self, PyObject* args);
    };

    // Helper macros
    #define PY_CHECK_NULL(obj) \
        if (!obj) { \
            PyErr_SetString(PyExc_RuntimeError, "Object is null"); \
            return nullptr; \
        }

    #define PY_CHECK_TYPE(obj, type) \
        if (!PyObject_TypeCheck(obj, &type)) { \
            PyErr_SetString(PyExc_TypeError, "Invalid type"); \
            return nullptr; \
        }

} // namespace Python
} // namespace Atometa

// C interface for Python module initialization
extern "C" {
    PyMODINIT_FUNC PyInit_atometa(void);
}