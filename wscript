VERSION = '0.1.0'
APPNAME = 'yamlcast'


def options(opt):
    opt.load('compiler_cxx')
    opt.load('unittest_gtest')

    opt.add_option('--enable-gcov',
                   action='store_true',
                   default=False,
                   dest='gcov',
                   help='only for debug')


def configure(conf):
    conf.env.CXXFLAGS += ['-O2', '-Wall', '-g', '-pipe']
    conf.load('compiler_cxx')
    conf.load('unittest_gtest')

    conf.check_cfg(package='pficommon', args='--cflags --libs')
    conf.check_cfg(package='yaml-0.1', args='--cflags --libs')

    if conf.options.gcov:
        conf.env.append_value('CXXFLAGS', '-fprofile-arcs')
        conf.env.append_value('CXXFLAGS', '-ftest-coverage')
        conf.env.append_value('LINKFLAGS', '-lgcov')
        conf.env.append_value('LINKFLAGS', '-fprofile-arcs')
        conf.env.append_value('LINKFLAGS', '-ftest-coverage')

    conf.env.VERSION = VERSION


def build(bld):
    bld(source='yamlcast.pc.in',
        prefix=bld.env['PREFIX'],
        exec_prefix='${prefix}',
        libdir=bld.env['LIBDIR'],
        includedir='${prefix}/include',
        PACKAGE=APPNAME,
        VERSION=VERSION)

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


def gcovr(ctx):
    excludes = [
        '.*\\.unittest-gtest.*',
        '.*_test\\.cpp',
    ]

    args = 'gcovr --branches -r . '
    for e in excludes:
        args += ' -e "%s"' % e

    ctx.exec_command(args)
