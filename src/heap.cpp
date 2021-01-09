#include "heap.hpp"

#include <ac-library/containers/persistent/binary_heap/binary_heap.hpp>
#include <cstdint>

namespace NACPyDiskStructs {
    using namespace NAC;

    PyObject* HeapOpen(PyObject*, PyObject* args) {
        const char* path;

        if (!PyArg_ParseTuple(args, "s", &path)) {
            return nullptr;
        }

        return Py_BuildValue("K", (uintptr_t)new TPersistentBinaryHeap(path));
    }

    PyObject* HeapNew(PyObject*, PyObject* args) {
        const char* path;
        unsigned long long int recordSize;

        if (!PyArg_ParseTuple(args, "sK", &path, &recordSize)) {
            return nullptr;
        }

        return Py_BuildValue("K", (uintptr_t)new TPersistentBinaryHeap(path, recordSize));
    }

    PyObject* HeapDelete(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        delete (TPersistentBinaryHeap*)ptr;

        Py_RETURN_NONE;
    }

    PyObject* HeapInsert(PyObject*, PyObject* args) {
        unsigned long long int ptr;
        const char* value;
        Py_ssize_t valueLength;

        if (!PyArg_ParseTuple(args, "Ks#", &ptr, &value, &valueLength)) {
            return nullptr;
        }

        ((TPersistentBinaryHeap*)ptr)->Insert(value);

        Py_RETURN_NONE;
    }

    PyObject* HeapClose(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        ((TPersistentBinaryHeap*)ptr)->Close();

        Py_RETURN_NONE;
    }

    PyObject* HeapRecordSize(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        return Py_BuildValue("K", ((TPersistentBinaryHeap*)ptr)->RecordSize());
    }

    PyObject* HeapGetAll(PyObject*, PyObject* args) {
        unsigned long long int ptr;
        const char* prefix;
        Py_ssize_t prefixLength;

        if (!PyArg_ParseTuple(args, "Ks#", &ptr, &prefix, &prefixLength)) {
            return nullptr;
        }

        return Py_BuildValue("K", (uintptr_t)new TPersistentBinaryHeap::TIterator(((TPersistentBinaryHeap*)ptr)->GetAll(TBlob(prefixLength, prefix))));
    }

    PyObject* HeapIteratorDelete(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        delete (TPersistentBinaryHeap::TIterator*)ptr;

        Py_RETURN_NONE;
    }

    PyObject* HeapIteratorNext(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        TBlob data;

        if (((TPersistentBinaryHeap::TIterator*)ptr)->Next(data)) {
            return Py_BuildValue("y#", data.Data(), data.Size());
        }

        Py_RETURN_NONE;
    }
}
