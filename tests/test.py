#!/usr/bin/env python

from ac_test_hashmap import test_HashMap
from ac_test_heap import test_Heap
from ac_test_rbtree import test_RBTree

import os
import sys
from tempfile import mkstemp
import shutil


class TestDir(object):
    def __init__(self):
        (fh, path_) = mkstemp()
        self.__root = path_ + '-test'

        os.mkdir(self.__root)

        os.close(fh)
        os.remove(path_)

        self.__counter = 0

    def file(self):
        self.__counter += 1

        return os.path.join(self.__root, str(self.__counter))

    def remove(self):
        shutil.rmtree(self.__root)


test_dir = TestDir()


def test_all():
    test_HashMap(test_dir)
    test_Heap(test_dir)
    test_RBTree(test_dir)


try:
    test_all()

except:
    exc = sys.exc_info()[1]

    try:
        test_dir.remove()

    except:
        pass

    raise exc


test_dir.remove()

print('OK')
