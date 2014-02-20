# -*- coding: utf-8 -*-

import os, sys

COCPACK = r'..\..\editor_bin\cocpackage.exe'
LUA = r'..\..\tools\lua\lua52.exe'
EPBIN = r'..\..\tools\lua\epbin.lua'
CONV = r'..\data\lua\building_convert.lua'

PATH_SHARE_EP = r'd:\share\coc\ep\ '

_pjoin = os.path.join

def _run_cmd(cmd):
    print cmd
    os.system(cmd)

def _mkdir(path):
    if not os.path.isdir(path):
        _run_cmd('MKDIR "%s"' % path)
    return path

def data_dir(*args):
    return _pjoin('..', 'data', *args)

def output_dir(*args):
    return _pjoin('..', 'output', *args)

WORK_ITEMS = [
        {
            'editor_json':  data_dir('editor_json'),
            'texpack_json': data_dir('texpack_json', 'buildings2'),
            'texpack_tex':  data_dir('texpack_tex'),
            'tmp_lua':      output_dir('buildings2.tmp.lua'),
            'scale':        '1.0',
            'output_dir':   output_dir(),
            'output_lua':   output_dir('buildings2.lua'),
            'output_epp':    output_dir('buildings2.epp'),
            'output_epd':    output_dir('buildings2.epd'),
        },
        #(
        #    r'..\data\json',
        #    r'..\data\pack\buildings2',
        #    r'..\data\output\buildings2.tmp.lua',
        #    '1.0',
        #    r'..\data\output\buildings2.lua',
        #    r'..\data\output\buildings2.ep',
        #),
        #(
        #    r'..\data\json',
        #    r'..\data\pack\buildings2_lowres',
        #    r'..\data\output\buildings2_lowres.tmp.lua',
        #    '0.5',
        #    r'..\data\output\buildings2_lowres.lua',
        #    r'..\data\output\buildings2_lowres.ep',
        #),
    ]

if len(sys.argv) != 2:
    print "usage: %s options" % os.path.basename(sys.argv[0])
    print ""
    print "  options: 1 - pack Editor-json & TexPacker-json"
    print "           2 - apply building's scale"
    print "           3 - build EP"
    print "           4 - copy EP to share dir"
    print ""

    options = raw_input("Please enter options(1234): ") or "1234"
else:
    options = sys.argv[1]

for entry in WORK_ITEMS:
    _mkdir(entry["output_dir"])

# for ed_json, tp_json, tmp_lua, scale, lua, ep in WORK_ITEMS:
for entry in WORK_ITEMS:
    if '1' in options:
        _run_cmd('%s %s %s %s %s' % (COCPACK, entry["editor_json"], entry["texpack_json"], entry["tmp_lua"], entry["scale"]))

    if '2' in options:
        _run_cmd('%s %s %s %s' % (LUA, CONV, entry["tmp_lua"], entry["output_lua"]))

    if '3' in options:
        _run_cmd("copy /Y %s %s" % (_pjoin(entry["texpack_tex"], '*'), entry["output_dir"]))
        _run_cmd('%s %s -pd -pkmc %s' % (LUA, EPBIN, entry["output_lua"]))

    if '4' in options:
        _run_cmd('copy /Y %s %s' % (entry["output_epp"], PATH_SHARE_EP))
        _run_cmd('copy /Y %s %s' % (entry["output_epd"], PATH_SHARE_EP))

print ""
print "================================================="
print ""
print "  FINISHED. press any key to exit"
print ""
print "================================================="
os.system("pause")
