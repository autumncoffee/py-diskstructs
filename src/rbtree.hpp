#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace NACPyDiskStructs {
    PyObject* RBTreeOpen(PyObject*, PyObject* args);
    PyObject* RBTreeOpenRW(PyObject*, PyObject* args);
    PyObject* RBTreeNew(PyObject*, PyObject* args);

    PyObject* RBTreeDelete(PyObject*, PyObject* args);

    PyObject* RBTreeInsert(PyObject*, PyObject* args);

    PyObject* RBTreeGet(PyObject*, PyObject* args);
    PyObject* RBTreeGetAll(PyObject*, PyObject* args);
    PyObject* RBTreeIteratorDelete(PyObject*, PyObject* args);

    PyObject* RBTreeIteratorNext(PyObject*, PyObject* args);
}
