#!/usr/bin/python3

import os
import sys
import subprocess
import time
import glob
import copy
import platform

BUILD_DATE = str(time.strftime('%H:%M:%S %d-%m-%Y', time.gmtime()))

NAME = 'engine'
LINUX = '.'+platform.architecture()[0]
WINDOWS = '.'+platform.architecture()[0]+'.exe'
CC = ''
CC_LINUX = 'gcc'
CC_WIN = 'x86_64-w64-mingw32-gcc'
#CC_WIN = 'x86_64-w64-mingw32-clang'

PATH_BIN = 'bin/'
PATH_SRC = 'src/'
PATH_OBJ = 'obj/'
PATH_LIB = 'lib/'
SDL2 = os.getcwd()+'/'+PATH_LIB

FLAGS = ['-L'+SDL2, '-I'+SDL2, '-march=x86-64', '-mtune=native',
         '-Wall', '-W', '-DBUILD_DATE='+'"'+BUILD_DATE+'"']
FLAGS_RELEASE = ['-Os', '-no-pie', '-s']
FLAGS_DEBUG = ['-O2', '-pipe', '-g']
LIBS = []
LIBS_LINUX = ['-lm', '-lSDL2']
LIBS_LINUX_STATIC = ['-lc', '-lm', '-ldl', '-lpthread', '-static-libgcc',
                     '-Wl,-Bstatic,--whole-archive', '-lSDL2_linux', '-Wl,--no-whole-archive']

LIBS_WIN = ['-lmingw32', '-lSDL2main_win', '-lSDL2_win', '-mwindows']
LIBS_WIN_STATIC = ['-Wl,-Bstatic,--whole-archive', '-lSDL2main_win', '-lSDL2_win', '-Wl,--no-whole-archive',
                   '-mwindows', '-ldinput8', '-ldxguid', '-ldxerr8', '-luser32', '-lgdi32', '-lwinmm', '-limm32', '-lole32', '-loleaut32', '-lshell32', '-lsetupapi', '-lversion', '-luuid', '-static-libgcc']


class time_stamp:
    def __init__(self):
        try:
            file = open('.timestamp', 'r')
        except FileNotFoundError:
            file = open(".timestamp", "w+")
        self.list = file.read()
        file.close()
        self.new_list = ''
        self.recompile = []
        self.name_update = ''
        self.dep_list = {}
        self.source = []

    def check(self, file_name):
        if self.list.find(file_name) == -1:
            self.name_update = file_name
            return True
        else:
            for i in self.list.split('\n'):
                j = i.split(';')
                if file_name == j[0]:
                    if os.path.getmtime(PATH_SRC+file_name) == float(j[1]):
                        for r in self.recompile:
                            for dep in self.dep_list[file_name]['dep']:
                                if dep == r:
                                    self.recompile.append(file_name)
                                    self.name_update = file_name
                                    return True
                        self.new_list += file_name+';'+j[1]+'\n'
                        self.name_update = ''
                        return False
                    else:
                        self.recompile.append(file_name)
                        self.name_update = file_name
                        return True

    def update_stemp(self):
        if self.name_update != '':
            self.new_list += self.name_update+';' + \
                str(os.path.getmtime(PATH_SRC+self.name_update))+'\n'

    def write(self):
        file = open('.timestamp', 'w')
        file.write(self.new_list)
        file.close()

    def generate_source_list(self):
        dep_list = {}
        file_list = glob.glob(PATH_SRC+'*.c')
        file_list += glob.glob(PATH_SRC+'*.h')
        for i in file_list:
            name = i[len(PATH_SRC):len(i)]
            dep_list[name] = {'dep': []}
            f = os.open(i, os.O_RDONLY)
            buf = str(os.read(f, 1024))
            os.close(f)
            find_next_include = 0
            while True:
                find_include = buf.find('#include "', find_next_include)
                if find_include == -1:
                    break
                for index, char in enumerate(buf[find_include:]):
                    if char == '.':
                        include_name = buf[find_include +
                                           10: find_include+index]+'.h'
                        try:
                            file_list.index(PATH_SRC+include_name)
                        except ValueError:
                            pass
                        else:
                            dep_list[name]['dep'].append(include_name)
                        find_next_include = find_include+index
                        break
        self.dep_list = copy.deepcopy(dep_list)
        while True:
            for i in dep_list:
                if len(dep_list[i]['dep']) == 0:
                    self.source.append(i)
                else:
                    for index, char in enumerate(dep_list[i]['dep']):
                        try:
                            self.source.index(char)
                        except ValueError:
                            break
                        else:
                            if len(dep_list[i]['dep'])-1 == index:
                                self.source.append(i)
            for i in self.source:
                try:
                    del dep_list[i]
                except KeyError:
                    pass
            if len(dep_list) == 0:
                break


def versioning():

    version_template = '''
#pragma once
#define BUILD_VERSION "{0}"
#define BUILD_DATE "{1}"
'''
    try:
        buildver = open('.buildver', 'r')
        buildver.write(1)
    except FileNotFoundError:
        buildver = open(".buildver", "w+")
    version = buildver.read()
    buildver.close()

    buildver = open('.buildver', 'w')
    version = str(int(version)+1)
    buildver.write(version)
    buildver.close()

    buildver = open(PATH_SRC+'version.h', 'w')
    buildver.write(version_template.format(version, compile_time))
    buildver.close()


def build():
    start = time.time()
    # versioning()
    recompiling = time_stamp()
    recompiling.generate_source_list()
    for i in recompiling.source:
        var = []
        if recompiling.check(i):
            print(r'rekompiluje '+i)
            if i.find('.c') != -1:
                var = [CC] + FLAGS + ['-c', PATH_SRC+i, '-o', PATH_OBJ+i+'.o']
                try:
                    subprocess.check_call(var)
                except subprocess.CalledProcessError:
                    sys.exit(1)
            recompiling.update_stemp()
    recompiling.write()
    obj = []
    for i in recompiling.source:
        if i.find('.c') != -1:
            obj.append(PATH_OBJ+i+'.o')
    var = [CC] + FLAGS + LIBS + obj + ['-o', PATH_BIN+NAME]
    try:
        subprocess.check_call(var)
    except subprocess.CalledProcessError:
        sys.exit()
    end = time.time()
    print("czas kompilacji %.4fs" % (end - start))


def clean(name):
    for i in glob.glob(PATH_OBJ+'*'):
        os.remove(i)
        try:
            os.remove(PATH_BIN+name)
        except OSError:
            pass
        f = open('.timestamp', 'w')
        f.write('')
        f.close()


if __name__ == "__main__":
    if len(sys.argv) == 1:
        sys.argv.append('help')

    elif sys.argv[1] == 'build':
        CC = CC_LINUX
        FLAGS += FLAGS_DEBUG
        LIBS = LIBS_LINUX
        NAME += LINUX+'.debug'
        build()

    elif sys.argv[1] == 'release':
        tmp_name = NAME
        CC = CC_LINUX
        FLAGS += FLAGS_RELEASE
        LIBS = LIBS_LINUX
        NAME = tmp_name + LINUX
        clean(NAME)
        build()

        CC = CC_WIN
        FLAGS += FLAGS_RELEASE
        LIBS = LIBS_WIN_STATIC
        NAME = tmp_name + WINDOWS
        clean(NAME)
        build()
        clean('')

    elif sys.argv[1] == 'run':
        subprocess.call([PATH_BIN+NAME+LINUX+'.debug'])

    elif sys.argv[1] == 'clean':
        clean(NAME+LINUX+'.debug')

    else:
        print('build kompiluje')
        print('run uruchamia')
        print('clean czyści')
