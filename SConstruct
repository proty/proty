import os

env = Environment(ENV = os.environ)

def CheckProgram(context, name):
  context.Message("Checking for %s..." % name)
  result = not int(os.system("which %s > /dev/null" % name))
  context.Result(result)
  return result

# ########## Configure ##########
conf = Configure(env, custom_tests = {'CheckProgram' : CheckProgram})

# Find a compiler
if conf.CheckProgram("clang++"):
  env.Replace(CXX = "clang++", LINK = "clang++")
elif conf.CheckProgram("g++"):
  env.Replace(CXX = "g++", LINK = "g++")
else:
  print "No c++ compiler found. Please install clang or g++."
  Exit(1)

# Check for llvm
if not conf.CheckProgram("llvm-config"):
  print "llvm-config not found. Please install llvm."
  Exit(1)

env = conf.Finish()
# ########## Configure end ##########

env.SetOption('num_jobs', '2')
env.ParseConfig('llvm-config --cppflags --ldflags --libs core jit interpreter linker native bitwriter')
env.Append(CPPPATH = env.Dir('./include'))

# Pretty output
env.Replace(SHOBJSUFFIX = ".o") # .o suffix for shared objects
env.Replace(CXXCOMSTR = "c++ $SOURCE", LINKCOMSTR = "ld $TARGET") # comstrings for static objects
env.Replace(SHCXXCOMSTR = "c++ $SOURCE", SHLINKCOMSTR = "ld $TARGET") # comstrings for shared objects

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

objs = [
  Glob('src/parser/*.cc'),
  Glob('src/compiler/*.cc'),
  Glob('src/model/*.cc'),
]

libpyrite = env.SharedLibrary('pyrite', objs)

env.Replace(LINKFLAGS="")
pyrite = env.Program('pyrite', ['src/pyrite.cc', libpyrite])

prefix = ARGUMENTS.get('prefix', '/usr/local')
env.Install(prefix + '/bin', [pyrite])
env.Install(prefix + '/lib', [libpyrite])
env.Install(prefix + '/lib/pyrite', Glob('lib/*'))
env.Install(prefix + '/include/pyrite', Glob('include/*'))
env.Alias('install', [prefix + '/bin', prefix +'/lib',
                      prefix + '/include' , prefix + '/include/pyrite'])
