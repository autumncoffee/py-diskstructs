#!/usr/bin/env python

from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext
from tempfile import mkstemp
import os
import sys
import shutil
import urllib.request as http
import tarfile
from ssl import create_default_context


AC_COMMON = ('ac-common', 'cxx-common', 'v1.0.1')
AC_LIBRARY = ('ac-library', 'cxx-library', 'v1.0.4')


class BuildExtCommand(build_ext):
    __ac_include = None

    def initialize_options(self, *args, **kwargs):
        build_ext.initialize_options(self, *args, **kwargs)

        if self.include_dirs is None:
            self.include_dirs = []

        self.__ac_include = mkincludes()

        self.include_dirs.append(self.__ac_include)

    def run(self, *args, **kwargs):
        try:
            build_ext.run(self, *args, **kwargs)

        except:
            exc = sys.exc_info()[1]

            self.__cleanup()

            raise exc

        self.__cleanup()

    def build_extension(self, ext, *args, **kwargs):
        ext.sources.extend(ac_sources(self.__ac_include))

        build_ext.build_extension(self, ext, *args, **kwargs)

    def __cleanup(self):
        if self.__ac_include is not None:
            try:
                shutil.rmtree(self.__ac_include)

            except:
                pass

            self.__ac_include = None

    def __del__(self):
        self.__cleanup()


def mkincludes():
    (fh, path_) = mkstemp()
    path = path_ + '-inc'
    os.mkdir(path)

    import certifi

    sslctx = create_default_context(cafile=certifi.where())

    try:
        os.close(fh)
        os.remove(path_)

        for (dst, proj, ver) in [
            AC_COMMON,
            AC_LIBRARY,
        ]:
            url = 'https://github.com/autumncoffee/{}/archive/{}.tar.gz'.format(proj, ver)

            sys.stderr.write('Loading {}...\n'.format(url))

            res = http.urlopen(url, context=sslctx)
            (fh, tmp) = mkstemp()

            try:
                os.write(fh, res.read())
                os.close(fh)
                fh = None

                tar = tarfile.open(tmp)
                tar.extractall(path=path)
                tar.close()

            except:
                exc = sys.exc_info()[1]

                try:
                    os.unlink(tmp)

                except:
                    pass

                if fh is not None:
                    os.close(fh)

                raise exc

            os.unlink(tmp)

            os.rename(
                os.path.join(path, '{}-{}'.format(proj, ver.lstrip('v'))),
                os.path.join(path, dst),
            )

    except:
        exc = sys.exc_info()[1]

        try:
            shutil.rmtree(path)

        except:
            pass

        raise exc

    return path


def ac_sources(root):
    ac_common = os.path.join(root, AC_COMMON[0])
    ac_library = os.path.join(root, AC_LIBRARY[0])

    return [
        *lscpp(ac_common),
        *lscpp(os.path.join(ac_common, 'utils')),
        *lscpp(os.path.join(ac_library, 'containers', 'persistent', 'immutable_hashmap')),
        *lscpp(os.path.join(ac_library, 'containers', 'persistent', 'binary_heap')),
        *lscpp(os.path.join(ac_library, 'containers', 'rbtree')),
        os.path.join(ac_library, 'contrib', 'murmur', 'MurmurHash3.cpp'),
    ]


def lscpp(dir_):
    out = []

    for node in os.listdir(dir_):
        if node.endswith('.cpp'):
            out.append(os.path.join(dir_, node))

    return out


def if_darwin(list_):
    if sys.platform == 'darwin':
        return list_

    return []


ac_diskstructs = Extension(
    'ac_diskstructs.impl',
    language='c++',

    extra_compile_args=[
        '-std=c++17',
        *if_darwin(['-stdlib=libc++']),
    ],

    extra_link_args=[
        *if_darwin(['-stdlib=libc++']),
    ],

    include_dirs=[
        'contrib',
    ],

    sources=[
        *lscpp('src'),
        os.path.join('contrib', 'absl', 'numeric', 'int128.cc'),
    ],
)


setup(
    name='ac_diskstructs',
    version='1.0.1',
    packages=find_packages(),
    ext_modules=[
        ac_diskstructs,
    ],

    setup_requires=[
        'certifi',
    ],

    install_requires=[
        'certifi',
    ],

    cmdclass={
        'build_ext': BuildExtCommand,
    },
)
