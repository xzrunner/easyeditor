# -*- coding: utf-8 -*-

import os, sys

COCPACK = r'..\..\editor_bin\cocpackage.exe'
LUA = r'..\..\tools\lua\lua52.exe'
EPBIN = r'..\..\tools\lua\epbin.lua'

PATH_SHARE_EP = r'd:\share\coc\ep\ '

WORK_ITEMS = [
        (
            '..\\data\\json_scene1',
            '..\\data\\pack\\scene_bg1',
            '..\\data\\pack\\scene_bg1.lua',
            '..\\data\\pack\\scene_bg1.ep'
        ),
        (
            '..\\data\\json_scene2',
            '..\\data\\pack\\scene_bg2',
            '..\\data\\pack\\scene_bg2.lua',
            '..\\data\\pack\\scene_bg2.ep'
        ),
        (
            '..\\data\\json_scene3',
            '..\\data\\pack\\scene_bg3',
            '..\\data\\pack\\scene_bg3.lua',
            '..\\data\\pack\\scene_bg3.ep'
        ),
    ]

def run_cmd(cmd):
    print cmd
    os.system(cmd)

if len(sys.argv) != 2:
    print "usage: %s options" % os.path.basename(sys.argv[0])
    print ""
    print "  options: 1 - pack Editor-json & TexPacker-json"
    print "           2 - ((DO NOTHING))"
    print "           3 - build EP"
    print "           4 - copy EP to share dir"
    print ""

    options = raw_input("Please enter options(1234): ") or "1234"
else:
    options = sys.argv[1]

for ed_json, tp_json, lua, ep in WORK_ITEMS:
    if '1' in options:
        run_cmd('%s %s %s %s' % (COCPACK, ed_json, tp_json, lua))

    if '3' in options:
        run_cmd('%s %s -pvr %s' % (LUA, EPBIN, lua))

    if '4' in options:
        run_cmd('copy /Y %s %s' % (ep, PATH_SHARE_EP))

print ""
print "================================================="
print ""
print "  FINISHED. press any key to exit"
print ""
print "================================================="
os.system("pause")
