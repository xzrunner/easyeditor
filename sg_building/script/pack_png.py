# -*- coding: utf-8 -*-

import os, sys

COCPACK = r'..\..\editor_bin\cocpackage.exe'
LUA = r'..\..\tools\lua\lua52.exe'
EPBIN = r'..\..\tools\lua\epbin.lua'
CONV = r'..\data\lua\building_convert.lua'

PATH_SHARE_EP = r'd:\share\coc\ep\ '

WORK_ITEMS = [
        (
            r'..\data\json',
            r'..\data\pack\buildings2png',
            r'..\data\pack\buildings2png.tmp.lua',
            '1.0',
            r'..\data\pack\buildings2png.lua',
            r'..\data\pack\buildings2png.ep',
        ),
        # (
        #     r'..\data\json',
        #     r'..\data\pack\buildings2_lowrespng',
        #     r'..\data\pack\buildings2_lowrespng.tmp.lua',
        #     '0.5',
        #     r'..\data\pack\buildings2_lowrespng.lua',
        #     r'..\data\pack\buildings2_lowrespng.ep',
        # ),
    ]

def run_cmd(cmd):
    print cmd
    os.system(cmd)

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

for ed_json, tp_json, tmp_lua, scale, lua, ep in WORK_ITEMS:
    if '1' in options:
        run_cmd('%s %s %s %s %s' % (COCPACK, ed_json, tp_json, tmp_lua, scale))

    if '2' in options:
        run_cmd('%s %s %s %s' % (LUA, CONV, tmp_lua, lua))

    if '3' in options:
        run_cmd('%s %s -png8 %s' % (LUA, EPBIN, lua))

    if '4' in options:
        run_cmd('copy /Y %s %s' % (ep, PATH_SHARE_EP))

print ""
print "================================================="
print ""
print "  FINISHED. press any key to exit"
print ""
print "================================================="
os.system("pause")
