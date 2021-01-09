import ac_diskstructs.impl as LIB


class HashMapIterator(object):
    def __init__(self, ptr, key_type):
        self.__impl = ('HashMapIteratorNextInt' if key_type == int else 'HashMapIteratorNext')
        self.__ptr = ptr

    def delete(self):
        LIB.HashMapIteratorDelete(self.__ptr)

        self.delete = lambda: None

    def __del__(self):
        self.delete()

    def __iter__(self):
        return self

    def __next__(self):
        item = getattr(LIB, self.__impl)(self.__ptr)

        if item is None:
            self.delete()
            raise StopIteration()

        return item


def open_impl(path, rw, bucket_count):
    if bucket_count is not None:
        return LIB.HashMapNew(path, bucket_count)

    if rw:
        return LIB.HashMapOpenRW(path)

    return LIB.HashMapOpen(path)


class HashMap(object):
    def __init__(self, path, *, key_type=None, rw=False, bucket_count=None):
        self.__ptr = None

        if key_type is None:
            key_type = int

        elif key_type != bytes:
            raise Exception('Supported key types are: int (default), bytes');

        self.__ptr = open_impl(path, rw, bucket_count)
        self.__key_type = key_type

    # Free memory
    def delete(self):
        if self.__ptr is not None:
            LIB.HashMapDelete(self.__ptr)

        self.delete = lambda: None

    def __del__(self):
        self.delete()

    # Finish write operations
    def close(self):
        LIB.HashMapClose(self.__ptr)

        self.close = lambda: None

    def __getitem__(self, key):
        rv = getattr(LIB, 'HashMapGetInt' if isinstance(key, int) else 'HashMapGet')(self.__ptr, key)

        if rv is None:
            raise KeyError(key)

        return rv

    def get(self, key, default=None):
        try:
            return self[key]

        except KeyError:
            return default

    def __setitem__(self, key, value):
        getattr(LIB, 'HashMapInsertInt' if isinstance(key, int) else 'HashMapInsert')(self.__ptr, key, value)

    def bucket_count(self):
        return LIB.HashMapBucketCount(self.__ptr)

    def items(self):
        return HashMapIterator(LIB.HashMapGetAll(self.__ptr), self.__key_type)
