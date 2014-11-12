VERSION = '0.1.0'
APPNAME = 'yamlcast'


def options(opt):
    opt.load('compiler_cxx')
    opt.load('unittest_gtest')


def configure(conf):
    conf.env.CXXFLAGS += ['-O2', '-Wall', '-g', '-pipe']
    conf.load('compiler_cxx')
    conf.load('unittest_gtest')

    conf.check_cfg(package='pficommon', args='--cflags --libs')
    conf.check_cfg(package='yaml-cpp', args='--cflags --libs',
                   atleast_version='0.5')


def build(bld):
    bld.recurse('src')
