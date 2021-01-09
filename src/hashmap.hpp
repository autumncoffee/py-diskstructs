#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace NACPyDiskStructs {
    PyObject* HashMapOpen(PyObject*, PyObject* args);
    PyObject* HashMapOpenRW(PyObject*, PyObject* args);
    PyObject* HashMapNew(PyObject*, PyObject* args);

    PyObject* HashMapDelete(PyObject*, PyObject* args);

    PyObject* HashMapInsert(PyObject*, PyObject* args);
    PyObject* HashMapInsertInt(PyObject*, PyObject* args);
    PyObject* HashMapClose(PyObject*, PyObject* args);

    PyObject* HashMapBucketCount(PyObject*, PyObject* args);

    PyObject* HashMapGet(PyObject*, PyObject* args);
    PyObject* HashMapGetInt(PyObject*, PyObject* args);

    PyObject* HashMapGetAll(PyObject*, PyObject* args);
    PyObject* HashMapIteratorDelete(PyObject*, PyObject* args);

    PyObject* HashMapIteratorNext(PyObject*, PyObject* args);
    PyObject* HashMapIteratorNextInt(PyObject*, PyObject* args);
}
