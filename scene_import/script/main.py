# -*- coding: utf-8 -*-

import os, sys

root = ur'D:\dev_coco\coco-tools\scene_import'
share_root = ur'e:\share\coc'
raw_png_root = os.path.join(root, ur'data\raw_pngs')
comp_cfg_xls = os.path.join(root, ur'data\xls\场景建筑-图素表.xls')
anim_cfg_xls = os.path.join(root, ur'data\xls\场景建筑-动画表.xls')
picked_png_root = os.path.join(root, ur'build\pngs')

tpack_png = os.path.join(root, ur"build\tex_lua_for_ep\buildings2.png")
tpack_lua = os.path.join(root, ur"build\tex_lua_for_ep\tpack.lua")

epgen_lua = os.path.join(root, ur"build\tex_lua_for_ep\buildings2.lua")
epgen_ep  = os.path.join(root, ur"build\tex_lua_for_ep\buildings2.ep")
epgen_script = os.path.join(root, ur"..\tools\lua\epbin.lua")
epgen_share_dir = os.path.join(share_root, ur"ep")

lua_exe = os.path.join(root, ur"..\tools\lua\lua52.exe")

print
print "============================================================"
print "pick: PNGS"
print "============================================================"
print

from pick_pngs import do_pick_pngs
do_pick_pngs(raw_png_root, comp_cfg_xls, picked_png_root)

print
print "============================================================"
print "generate: tpack.lua tpack.png"
print "============================================================"
print

from pack_pngs import do_pack_pngs
ok, lua_list = do_pack_pngs(tpack_png, tpack_lua, picked_png_root)
if not ok:
    print "pack_pngs error!"
    sys.exit(1)

print
print "============================================================"
print "generate: tpack.py"
print "============================================================"
print

os.system('%s build_tpack_py.lua tpack.py %s' % (lua_exe,
    ' '.join(['"%s"' % n for n in lua_list])))

print
print "============================================================"
print "generate: buildings2.lua"
print "============================================================"
print

from build_ep_lua import do_build_ep_lua
do_build_ep_lua(epgen_lua, comp_cfg_xls, anim_cfg_xls)

print
print "============================================================"
print "generate: buildings2.ep"
print "============================================================"
print

if os.path.isfile(epgen_ep):
    os.system('del /Q/F "%s"' % epgen_ep)

os.system('%s "%s" -png8 "%s"' % (lua_exe, epgen_script, epgen_lua))

print
print "============================================================"
print "copy: buildings2.ep to share folder"
print "============================================================"
print

os.system('copy "%s" "%s"' % (epgen_ep, epgen_share_dir))

