#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace NACPyDiskStructs {
    PyObject* HeapOpen(PyObject*, PyObject* args);
    PyObject* HeapNew(PyObject*, PyObject* args);

    PyObject* HeapDelete(PyObject*, PyObject* args);

    PyObject* HeapInsert(PyObject*, PyObject* args);
    PyObject* HeapClose(PyObject*, PyObject* args);

    PyObject* HeapRecordSize(PyObject*, PyObject* args);

    PyObject* HeapGetAll(PyObject*, PyObject* args);
    PyObject* HeapIteratorDelete(PyObject*, PyObject* args);

    PyObject* HeapIteratorNext(PyObject*, PyObject* args);
}
