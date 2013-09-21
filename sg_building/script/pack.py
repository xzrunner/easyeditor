# -*- coding: utf-8 -*-

import os, sys

COCPACK = r'..\..\editor_bin\cocpackage.exe'
LUA = r'..\..\tools\lua\lua52.exe'
EPBIN = r'..\..\tools\lua\epbin.lua'
CONV = r'..\data\lua\building_convert.lua'

PATH_EDITOR_JSON = r'..\data\json'
PATH_TPACK_JSON = r'..\data\pack\buildings2'
PATH_EP_LUA_TMP = r'..\data\pack\buildings2.tmp.lua'
PATH_EP_LUA = r'..\data\pack\buildings2.lua'
PATH_EP = r'..\data\pack\buildings2.ep'
PATH_SHARE_EP = r'd:\share\coc\ep\ '

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

    options = raw_input("Please enter options: ")
else:
    options = sys.argv[1]

if '1' in options:
    run_cmd('%s %s %s %s' % (COCPACK, PATH_EDITOR_JSON, PATH_TPACK_JSON, PATH_EP_LUA_TMP))

if '2' in options:
    run_cmd('%s %s %s %s' % (LUA, CONV, PATH_EP_LUA_TMP, PATH_EP_LUA))

if '3' in options:
    run_cmd('%s %s -pvr %s' % (LUA, EPBIN, PATH_EP_LUA))

if '4' in options:
    run_cmd('copy /Y %s %s' % (PATH_EP, PATH_SHARE_EP))

print ""
print "================================================="
print ""
print "  FINISHED. press any key to exit"
print ""
print "================================================="
os.system("pause")
