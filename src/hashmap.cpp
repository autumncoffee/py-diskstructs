#include "hashmap.hpp"

#include <ac-library/containers/persistent/immutable_hashmap/hashmap.hpp>
#include <cstdint>

namespace NACPyDiskStructs {
    using namespace NAC;

    PyObject* HashMapOpen(PyObject*, PyObject* args) {
        const char* path;

        if (!PyArg_ParseTuple(args, "s", &path)) {
            return nullptr;
        }

        return Py_BuildValue("K", (uintptr_t)new TPersistentImmutableHashMap(path, TPersistentImmutableHashMap::DefaultSeed));
    }

    PyObject* HashMapOpenRW(PyObject*, PyObject* args) {
        const char* path;

        if (!PyArg_ParseTuple(args, "s", &path)) {
            return nullptr;
        }

        return Py_BuildValue("K", (uintptr_t)new TPersistentImmutableHashMap(path, TPersistentImmutableHashMap::DefaultSeed, /* rw = */true));
    }

    PyObject* HashMapNew(PyObject*, PyObject* args) {
        const char* path;
        unsigned long long int bucketCount;

        if (!PyArg_ParseTuple(args, "sK", &path, &bucketCount)) {
            return nullptr;
        }

        return Py_BuildValue("K", (uintptr_t)new TPersistentImmutableHashMap(path, bucketCount, TPersistentImmutableHashMap::DefaultSeed));
    }

    PyObject* HashMapDelete(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        delete (TPersistentImmutableHashMap*)ptr;

        Py_RETURN_NONE;
    }

    PyObject* HashMapInsert(PyObject*, PyObject* args) {
        unsigned long long int ptr;
        const char* key;
        Py_ssize_t keyLength;
        const char* value;
        Py_ssize_t valueLength;

        if (!PyArg_ParseTuple(args, "Ks#s#", &ptr, &key, &keyLength, &value, &valueLength)) {
            return nullptr;
        }

        ((TPersistentImmutableHashMap*)ptr)->Insert(
            TBlob(keyLength, key),
            TBlob(valueLength, value)
        );

        Py_RETURN_NONE;
    }

    PyObject* HashMapInsertInt(PyObject*, PyObject* args) {
        unsigned long long int ptr;
        unsigned long long int key;
        const char* value;
        Py_ssize_t valueLength;

        if (!PyArg_ParseTuple(args, "KKs#", &ptr, &key, &value, &valueLength)) {
            return nullptr;
        }

        ((TPersistentImmutableHashMap*)ptr)->Insert(
            (uint64_t)key,
            TBlob(valueLength, value)
        );

        Py_RETURN_NONE;
    }

    PyObject* HashMapClose(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        ((TPersistentImmutableHashMap*)ptr)->Close();

        Py_RETURN_NONE;
    }

    PyObject* HashMapBucketCount(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        return Py_BuildValue("K", ((TPersistentImmutableHashMap*)ptr)->BucketCount());
    }

    namespace {
        template<typename TKey>
        PyObject* HashMapGetImpl(uintptr_t ptr, TKey&& key) {
            auto data = ((TPersistentImmutableHashMap*)ptr)->Get(key);

            if (data) {
                return Py_BuildValue("y#", data.Data(), data.Size());
            }

            Py_RETURN_NONE;
        }
    }

    PyObject* HashMapGet(PyObject*, PyObject* args) {
        unsigned long long int ptr;
        const char* key;
        Py_ssize_t keyLength;

        if (!PyArg_ParseTuple(args, "Ks#", &ptr, &key, &keyLength)) {
            return nullptr;
        }

        return HashMapGetImpl(ptr, TBlob(keyLength, key));
    }

    PyObject* HashMapGetInt(PyObject*, PyObject* args) {
        unsigned long long int ptr;
        unsigned long long int key;

        if (!PyArg_ParseTuple(args, "KK", &ptr, &key)) {
            return nullptr;
        }

        return HashMapGetImpl(ptr, (uint64_t)key);
    }

    PyObject* HashMapGetAll(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        return Py_BuildValue("K", (uintptr_t)new TPersistentImmutableHashMap::TIterator(((TPersistentImmutableHashMap*)ptr)->GetAll()));
    }

    PyObject* HashMapIteratorDelete(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        delete (TPersistentImmutableHashMap::TIterator*)ptr;

        Py_RETURN_NONE;
    }

    namespace {
        template<typename TKey>
        PyObject* HashMapIteratorNextImpl(uintptr_t ptr, TKey& key) {
            TBlob data;

            if (((TPersistentImmutableHashMap::TIterator*)ptr)->Next(key, data)) {
                return Py_BuildValue("y#", data.Data(), data.Size());
            }

            Py_RETURN_NONE;
        }
    }

    PyObject* HashMapIteratorNext(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        TBlob key;
        auto* value = HashMapIteratorNextImpl(ptr, key);

        if (value == Py_None) {
            return value;
        }

        auto* rv = PyTuple_New(2);

        PyTuple_SetItem(rv, 0, Py_BuildValue("y#", key.Data(), key.Size()));
        PyTuple_SetItem(rv, 1, value);

        return rv;
    }

    PyObject* HashMapIteratorNextInt(PyObject*, PyObject* args) {
        unsigned long long int ptr;

        if (!PyArg_ParseTuple(args, "K", &ptr)) {
            return nullptr;
        }

        uint64_t key;
        auto value = HashMapIteratorNextImpl(ptr, key);

        if (value == Py_None) {
            return value;
        }

        auto* rv = PyTuple_New(2);

        PyTuple_SetItem(rv, 0, Py_BuildValue("K", key));
        PyTuple_SetItem(rv, 1, value);

        return rv;
    }
}
