import ac_diskstructs.impl as LIB


class RBTreeIterator(object):
    def __init__(self, ptr):
        self.__ptr = ptr

    def delete(self):
        LIB.RBTreeIteratorDelete(self.__ptr)

        self.delete = lambda: None

    def __del__(self):
        self.delete()

    def __iter__(self):
        return self

    def __next__(self):
        item = LIB.RBTreeIteratorNext(self.__ptr)

        if item is None:
            self.delete()
            raise StopIteration()

        return item


def open_impl(path, rw, create):
    if create:
        return LIB.RBTreeNew(path)

    if rw:
        return LIB.RBTreeOpenRW(path)

    return LIB.RBTreeOpen(path)


class RBTree(object):
    def __init__(self, path, *, rw=False, create=False):
        self.__ptr = open_impl(path, rw, create)

    # Free memory
    def delete(self):
        LIB.RBTreeDelete(self.__ptr)

        self.delete = lambda: None

    def __del__(self):
        self.delete()

    def __getitem__(self, key):
        rv = LIB.RBTreeGet(self.__ptr, key)

        if rv is None:
            raise KeyError(key)

        return rv

    def get(self, key, default=None):
        try:
            return self[key]

        except KeyError:
            return default

    def __setitem__(self, key, value):
        LIB.RBTreeInsert(self.__ptr, key, value)

    def get_all(self, prefix):
        return RBTreeIterator(LIB.RBTreeGetAll(self.__ptr, prefix))
