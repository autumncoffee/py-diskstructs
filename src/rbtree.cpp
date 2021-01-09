#include "rbtree.hpp"

#include <ac-library/containers/rbtree/persistent.hpp>
#include <cstdint>

namespace NACPyDiskStructs {
    using namespace NAC;

    PyObject* RBTreeOpen(PyObject*, PyObject* args) {
        const char* path;

        if (!PyArg_ParseTuple(args, "s", &path)) {
            return nullptr;
        }

        auto out = new TPersistentRBTree(path);
        out->FindRoot();
        return Py_BuildValue("K", (uintptr_t)out);
    }

    PyObject* RBTreeOpenRW(PyObject*, PyObject* args) {
        const char* path;

        if (!PyArg_ParseTuple(args, "s", &path)) {
            return nullptr;
        }

        auto out = new TPersistentRBTree(path, TFile::ACCESS_RDWR);
        out->FindRoot();
        return Py_BuildValue("K", (uintptr_t)out);
    }

    PyObject* RBTreeNew(PyObject*, PyObject* args) {
        const char* path;

        if (!PyArg_ParseTuple(args, "s", &path)) {
            return nullptr;
        }

        return Py_BuildValue("K", (uintptr_t)new TPersistentRBTree(path, TFile::ACCESS_CREATE));
    }

    PyObject* RBTreeDelete(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        delete (TPersistentRBTree*)ptr;

        Py_RETURN_NONE;
    }

    PyObject* RBTreeInsert(PyObject*, PyObject* args) {
        unsigned long long int ptr;
        const char* key;
        Py_ssize_t keyLength;
        const char* value;
        Py_ssize_t valueLength;

        if (!PyArg_ParseTuple(args, "Ks#s#", &ptr, &key, &keyLength, &value, &valueLength)) {
            return nullptr;
        }

        ((TPersistentRBTree*)ptr)->Insert(
            TBlob(keyLength, key),
            TBlob(valueLength, value)
        );

        Py_RETURN_NONE;
    }

    PyObject* RBTreeGet(PyObject*, PyObject* args) {
        unsigned long long int ptr;
        const char* key;
        Py_ssize_t keyLength;

        if (!PyArg_ParseTuple(args, "Ks#", &ptr, &key, &keyLength)) {
            return nullptr;
        }

        if (auto value = ((TPersistentRBTree*)ptr)->Get(TBlob(keyLength, key))) {
            return Py_BuildValue("y#", value.Data(), value.Size());
        }

        Py_RETURN_NONE;
    }

    PyObject* RBTreeGetAll(PyObject*, PyObject* args) {
        unsigned long long int ptr;
        const char* prefix;
        Py_ssize_t prefixLength;

        if (!PyArg_ParseTuple(args, "Ks#", &ptr, &prefix, &prefixLength)) {
            return nullptr;
        }

        return Py_BuildValue("K", (uintptr_t)new TPersistentRBTree::TIterator(((TPersistentRBTree*)ptr)->GetAll(TBlob(prefixLength, prefix))));
    }

    PyObject* RBTreeIteratorDelete(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        delete (TPersistentRBTree::TIterator*)ptr;

        Py_RETURN_NONE;
    }

    PyObject* RBTreeIteratorNext(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        TBlob key;
        TBlob value;

        if (((TPersistentRBTree::TIterator*)ptr)->Next(key, value)) {
            auto* rv = PyTuple_New(2);

            PyTuple_SetItem(rv, 0, Py_BuildValue("y#", key.Data(), key.Size()));
            PyTuple_SetItem(rv, 1, Py_BuildValue("y#", value.Data(), value.Size()));

            return rv;
        }

        Py_RETURN_NONE;
    }
}
