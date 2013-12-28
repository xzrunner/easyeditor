# -*- coding: utf-8 -*-

import sys
import os
import re
import json

# USE PNG
EXT = 'png'
MODE = '-pd -png8'
TEX_FORMAT = 'png'
EXTRA_OPT = '--opt RGBA8888 --size-constraints AnySize'

# USE PVR
#EXT = 'pvr'
#MODE = '-pd -pvr'
#TEX_FORMAT = 'pvr2'
#EXTRA_OPT = '--opt PVRTC4'

_abspath = os.path.abspath
_pjoin = os.path.join

def _run_cmd(cmd):
    print "* * * *)", cmd
    os.system(cmd.encode('cp936'))

def _mkdir(name):
    path = _pjoin(pwd, name)
    if not os.path.isdir(path):
        _run_cmd('MKDIR "%s"' % path)
    return path

pwd = os.getcwd()
build = _mkdir('build_loading_mmzb')


LUA = _pjoin(pwd, '..', 'tools', 'lua', 'lua52.exe')
EPBIN = _pjoin(pwd, '..', 'tools', 'lua', 'epbin.lua')

res = _abspath(_pjoin(pwd, '..', 'sg_loading_mmzb'))
png = _pjoin(build, 'loading_mmzb%d.'+EXT)
tex = _pjoin(build, 'loading_mmzb%d.json')
tex2 = _pjoin(build, "loading_mmzb")
loading_lua = _pjoin(build, "loading_mmzb.lua")

# texture packer
def call_tex_pack(png, tex, res, redundant_files):
    TMP_FILE = _pjoin(pwd, 'texturepacker.output.log')
    if os.path.isfile(TMP_FILE):
        _run_cmd('del /Q/F "%s"' % TMP_FILE)

    cmd = ' '.join([
        'TexturePacker',
        '--algorithm MaxRects',
        '--maxrects-heuristics Best',
        '--pack-mode Best',
        '--premultiply-alpha',
        '--sheet %s' % png,
        '--texture-format %s' % TEX_FORMAT,
        '%s' % EXTRA_OPT,
        '--data %s' % tex,
        '--format json-array',
        '--enable-rotation',
        '--shape-padding 2',
        '--border-padding 0',
        '--inner-padding 0',
        '--extrude 1',
        '"%s"' % res,
        '2> %s' % TMP_FILE,
        ])
    _run_cmd(cmd)

    f = open(TMP_FILE, 'rb')
    lines = f.readlines()
    print "<<---------------------------------------------->>"
    for line in lines:
        print line.strip()
    print "<<<EOF>>>"
    f.close()

    _run_cmd('del /Q/F "%s"' % TMP_FILE)

    if lines and len(lines) >= 1:
        assert lines[0].strip() == 'TexturePacker:: error: Not all sprites could be packed into the texture!', lines[0].strip()
        for line in lines[1:]:
            m = re.match(r'^TexturePacker:: warning: (.*)$', line.strip())
            assert m, line.strip()
            redundant_files.append(m.group(1))
        print "FALSE!"
        return False

    assert not lines
    print "TRUE!"
    return True

def pack_textures():
    png1 = png % 1
    tex1 = tex % 1
    redundant_files = []
    if not call_tex_pack(png1, tex1, res, redundant_files):
        sys.exit(1)

# texturepacker
pack_textures()

# package
cmd = "cocpackage_load.exe %s %s %s" % (res, tex2, loading_lua)
_run_cmd(cmd) 

# build ep
_run_cmd('%s %s %s %s' % (LUA, EPBIN, MODE, loading_lua))
