# -*- coding: utf-8 -*-

import os, sys
from config import config

root = ur'D:\dev\dev_coco\coco-tools\scene_import'

config.PACK_SCALE = 1.0
config.OUT_SCALE = 10.0
config.OUTDIR_JSON = os.path.join(root, ur'build\json')
config.OUTDIR_PNG = os.path.join(root, ur'build\pngs')

# USE PNG
# config.IMAGE_EXT = '.png'
# config.EPBIN_FLAGS = '-png8'
# config.TEX_PACKER_FLAGS = '--texture-format png'

# USE PVR
config.IMAGE_EXT = '.pvr'
config.EPBIN_FLAGS = '-pvr'
config.TEX_PACKER_FLAGS = '--texture-format pvr2 --opt PVRTC4'

share_root = ur'e:\share\coc'
raw_png_root = os.path.join(root, ur'data\raw_pngs')
comp_cfg_xls = os.path.join(root, ur'data\xls\场景建筑-图素表.xls')
anim_cfg_xls = os.path.join(root, ur'data\xls\场景建筑-动画表.xls')
# picked_png_root = os.path.join(root, ur'build\pngs')

tpack_image = os.path.join(root, ur"build\tex_lua_for_ep\buildings2" + config.IMAGE_EXT)
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
do_pick_pngs(raw_png_root, comp_cfg_xls, config.OUTDIR_PNG)

from build_json import do_build_json
do_build_json(comp_cfg_xls, anim_cfg_xls)

# print
# print "============================================================"
# print "generate: tpack.lua tpack.png"
# print "============================================================"
# print

# from pack_pngs import do_pack_pngs
# ok, lua_list = do_pack_pngs(tpack_image, tpack_lua, picked_png_root)
# if not ok:
#     print "pack_pngs error!"
#     sys.exit(1)

# print
# print "============================================================"
# print "generate: tpack.py"
# print "============================================================"
# print

# os.system('%s build_tpack_py.lua tpack.py %s' % (lua_exe,
#     ' '.join(['"%s"' % n for n in lua_list])))

# print
# print "============================================================"
# print "generate: buildings2.lua"
# print "============================================================"
# print

# from build_ep_lua import do_build_ep_lua
# do_build_ep_lua(epgen_lua, comp_cfg_xls, anim_cfg_xls)

# print
# print "============================================================"
# print "generate: buildings2.ep"
# print "============================================================"
# print

# if os.path.isfile(epgen_ep):
#     os.system('del /Q/F "%s"' % epgen_ep)

# os.system('%s "%s" %s "%s"' % (lua_exe, epgen_script, config.EPBIN_FLAGS, epgen_lua))

# print
# print "============================================================"
# print "copy: buildings2.ep to share folder"
# print "============================================================"
# print

# os.system('copy "%s" "%s"' % (epgen_ep, epgen_share_dir))

