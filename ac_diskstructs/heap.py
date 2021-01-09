import ac_diskstructs.impl as LIB


class HeapIterator(object):
    def __init__(self, ptr):
        self.__ptr = ptr

    def delete(self):
        LIB.HeapIteratorDelete(self.__ptr)

        self.delete = lambda: None

    def __del__(self):
        self.delete()

    def __iter__(self):
        return self

    def __next__(self):
        item = LIB.HeapIteratorNext(self.__ptr)

        if item is None:
            self.delete()
            raise StopIteration()

        return item


class Heap(object):
    def __init__(self, path, *, record_size=None):
        self.__ptr = (LIB.HeapOpen(path) if record_size is None else LIB.HeapNew(path, record_size))

    def record_size(self):
        return LIB.HeapRecordSize(self.__ptr)

    # Free memory
    def delete(self):
        LIB.HeapDelete(self.__ptr)

        self.delete = lambda: None

    def __del__(self):
        self.delete()

    # Finish write operations
    def close(self):
        LIB.HeapClose(self.__ptr)

        self.close = lambda: None

    def __getitem__(self, prefix):
        it = self.get_all(prefix)

        for rv in it:
            it.delete()
            return rv

        raise KeyError(prefix)

    def get(self, prefix, default=None):
        try:
            return self[prefix]

        except KeyError:
            return default

    def insert(self, value):
        LIB.HeapInsert(self.__ptr, value)

    def get_all(self, prefix):
        return HeapIterator(LIB.HeapGetAll(self.__ptr, prefix))
