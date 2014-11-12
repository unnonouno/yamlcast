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


def cpplint(ctx):
    filters = [
        '-runtime/references',
        '-legal/copyright',
        '-build/include_order',
    ]
    cpplint_args = '--filter=%s --extensions=cpp,hpp' % ','.join(filters)

    src_dir = ctx.path.find_node('src')
    files = []
    for f in src_dir.ant_glob('**/*.cpp **/*.hpp'):
        files.append(f.path_from(ctx.path))

    args = 'cpplint.py %s %s 2>&1 | grep -v "^\(Done\|Total\)"' \
           % (cpplint_args, ' '.join(files))
    result = ctx.exec_command(args)
    if result == 0:
        ctx.fatal('cpplint failed')
