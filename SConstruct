# -*- coding: utf-8 -*-
import os
from datetime import datetime

env = Environment(ENV = os.environ)

env.VERSION = "0.01"
env.COPYRIGHT = "Thomas Gatzweiler"

# #################### Configure ####################
def CheckProgram(context, name):
  context.Message("Checking for %s..." % name)
  result = not int(os.system("which %s > /dev/null 2>&1" % name))
  context.Result(result)
  return result

def CheckFile(context, name):
  context.Message("Checking for directory %s..." % name)
  result = os.path.exists(name)
  context.Result(result)
  return result

conf = Configure(env, custom_tests = {'CheckProgram': CheckProgram, 'CheckFile': CheckFile})

# Find a compiler
if conf.CheckProgram("clang++"):
  env.Replace(CXX = "clang++", LINK = "clang++")
else:
  print "clang++ compiler not found. Please install clang."
  Exit(1)

# Check for llvm
if not conf.CheckProgram("llvm-config"):
  print "llvm-config not found. Please install llvm."
  Exit(1)

env = conf.Finish()
# #################### Configure end ####################

env.SetOption('num_jobs', '2')
env.ParseConfig('llvm-config --cppflags --ldflags --libs core jit interpreter linker native bitwriter')
env.Append(CPPPATH = env.Dir('./include'))

# Pretty output
env.Replace(SHOBJSUFFIX = ".o") # .o suffix for shared objects
env.Replace(CXXCOMSTR = "c++ $SOURCE", LINKCOMSTR = "ld $TARGET") # comstrings for static objects
env.Replace(SHCXXCOMSTR = "c++ $SOURCE", SHLINKCOMSTR = "ld $TARGET") # comstrings for shared objects

# Get prefix
prefix = ARGUMENTS.get('prefix', '/usr/local')

# Choose build mode
mode = ARGUMENTS.get('mode', 'debug')
if mode == 'release':
  env.Append(CXXFLAGS = '-O3 -w')
  print "RELEASE BUILD"
elif mode == 'debug':
  env.Append(CXXFLAGS = '-O0 -Wall -g')
  print "DEBUG BUILD"
else:
  print "Error: expected 'debug' or 'release', found: " + mode
  Exit(1)

# #################### Generate version.hh ##############
config_template = """#ifndef PYRITE_VERSION_HH
#define PYRITE_VERSION_HH

#define PYRITE_VERSION    "%s"
#define PYRITE_COPYRIGHT  "Copyright © 2009-%i %s"
#define BUILD_TIME        "%s"
#define BUILD_MODE        "%s"
#define BUILD_PREFIX      "%s"

#endif""" % (env.VERSION, datetime.today().year, env.COPYRIGHT,
            datetime.now().strftime("%d %b %Y, %H:%M:%S"), mode, prefix)

config_file = open("include/config.hh", "w")
config_file.write(config_template)
config_file.close()
# #################### version.hh END ###################


##### COMPILER

objs = [
  Glob('src/parser/*.cc'),
  Glob('src/compiler/*.cc'),
]

libpyrite = env.SharedLibrary('pyrite', objs)

env.Replace(LINKFLAGS="")
pyrite = env.Program('pyrite', ['src/pyrite.cc', libpyrite])


##### VM

objs = [
  Glob('src/vm/*.cc'),
]

vmenv = env.Clone()
vmenv.Replace(CXXFLAGS="-emit-llvm -O3", CPPDEFINES="")
vmenv.Replace(LINK="llvm-link", LINKFLAGS="", SHLINKFLAGS="", LIBPATH="", LIBS="")
pyritevm = vmenv.Program('lib/vm.bc', objs)

##### INSTALL

env.Install(prefix + '/bin', [pyrite])
env.Install(prefix + '/lib', [libpyrite])
env.Install(prefix + '/lib/pyrite', Glob('lib/*'))
env.Install(prefix + '/include/pyrite', Glob('include/*'))
env.Alias('install', [prefix + '/bin', prefix +'/lib',
                      prefix + '/include' , prefix + '/include/pyrite'])

if 'deb' in COMMAND_LINE_TARGETS:
  SConscript("tools/deb/SConscript", exports="env")
