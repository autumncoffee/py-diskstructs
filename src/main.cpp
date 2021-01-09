#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "hashmap.hpp"
#include "heap.hpp"
#include "rbtree.hpp"

static PyMethodDef ACPyDiskStructsFuncs[] = {
    // *** HashMap ***

    {"HashMapOpen", (PyCFunction)NACPyDiskStructs::HashMapOpen, METH_VARARGS, ""},
    {"HashMapOpenRW", (PyCFunction)NACPyDiskStructs::HashMapOpenRW, METH_VARARGS, ""},
    {"HashMapNew", (PyCFunction)NACPyDiskStructs::HashMapNew, METH_VARARGS, ""},

    {"HashMapDelete", (PyCFunction)NACPyDiskStructs::HashMapDelete, METH_VARARGS, ""},

    {"HashMapInsert", (PyCFunction)NACPyDiskStructs::HashMapInsert, METH_VARARGS, ""},
    {"HashMapInsertInt", (PyCFunction)NACPyDiskStructs::HashMapInsertInt, METH_VARARGS, ""},
    {"HashMapClose", (PyCFunction)NACPyDiskStructs::HashMapClose, METH_VARARGS, ""},

    {"HashMapBucketCount", (PyCFunction)NACPyDiskStructs::HashMapBucketCount, METH_VARARGS, ""},

    {"HashMapGet", (PyCFunction)NACPyDiskStructs::HashMapGet, METH_VARARGS, ""},
    {"HashMapGetInt", (PyCFunction)NACPyDiskStructs::HashMapGetInt, METH_VARARGS, ""},

    {"HashMapGetAll", (PyCFunction)NACPyDiskStructs::HashMapGetAll, METH_VARARGS, ""},
    {"HashMapIteratorDelete", (PyCFunction)NACPyDiskStructs::HashMapIteratorDelete, METH_VARARGS, ""},

    {"HashMapIteratorNext", (PyCFunction)NACPyDiskStructs::HashMapIteratorNext, METH_VARARGS, ""},
    {"HashMapIteratorNextInt", (PyCFunction)NACPyDiskStructs::HashMapIteratorNextInt, METH_VARARGS, ""},

    // *** Heap ***

    {"HeapOpen", (PyCFunction)NACPyDiskStructs::HeapOpen, METH_VARARGS, ""},
    {"HeapNew", (PyCFunction)NACPyDiskStructs::HeapNew, METH_VARARGS, ""},

    {"HeapDelete", (PyCFunction)NACPyDiskStructs::HeapDelete, METH_VARARGS, ""},

    {"HeapInsert", (PyCFunction)NACPyDiskStructs::HeapInsert, METH_VARARGS, ""},
    {"HeapClose", (PyCFunction)NACPyDiskStructs::HeapClose, METH_VARARGS, ""},

    {"HeapRecordSize", (PyCFunction)NACPyDiskStructs::HeapRecordSize, METH_VARARGS, ""},

    {"HeapGetAll", (PyCFunction)NACPyDiskStructs::HeapGetAll, METH_VARARGS, ""},
    {"HeapIteratorDelete", (PyCFunction)NACPyDiskStructs::HeapIteratorDelete, METH_VARARGS, ""},

    {"HeapIteratorNext", (PyCFunction)NACPyDiskStructs::HeapIteratorNext, METH_VARARGS, ""},

    // *** RBTree ***

    {"RBTreeOpen", (PyCFunction)NACPyDiskStructs::RBTreeOpen, METH_VARARGS, ""},
    {"RBTreeOpenRW", (PyCFunction)NACPyDiskStructs::RBTreeOpenRW, METH_VARARGS, ""},
    {"RBTreeNew", (PyCFunction)NACPyDiskStructs::RBTreeNew, METH_VARARGS, ""},

    {"RBTreeDelete", (PyCFunction)NACPyDiskStructs::RBTreeDelete, METH_VARARGS, ""},

    {"RBTreeInsert", (PyCFunction)NACPyDiskStructs::RBTreeInsert, METH_VARARGS, ""},

    {"RBTreeGet", (PyCFunction)NACPyDiskStructs::RBTreeGet, METH_VARARGS, ""},
    {"RBTreeGetAll", (PyCFunction)NACPyDiskStructs::RBTreeGetAll, METH_VARARGS, ""},
    {"RBTreeIteratorDelete", (PyCFunction)NACPyDiskStructs::RBTreeIteratorDelete, METH_VARARGS, ""},

    {"RBTreeIteratorNext", (PyCFunction)NACPyDiskStructs::RBTreeIteratorNext, METH_VARARGS, ""},

    {nullptr, nullptr, 0, nullptr}
};

static PyModuleDef ACPyDiskStructsModule = {
    PyModuleDef_HEAD_INIT,
    "impl",
    nullptr,
    -1,
    ACPyDiskStructsFuncs
};

PyMODINIT_FUNC PyInit_impl(void) {
    return PyModule_Create(&ACPyDiskStructsModule);
}
