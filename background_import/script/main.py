# -*- coding: utf-8 -*-

import os, sys

root = ur'..'
share_root = ur'e:\share\coc'
raw_png_root = os.path.join(root, ur'data\rawpngs')
# comp_cfg_xls = os.path.join(root, ur'data\xls\场景建筑-图素表.xls')
# anim_cfg_xls = os.path.join(root, ur'data\xls\场景建筑-动画表.xls')
# picked_png_root = os.path.join(root, ur'build\pngs')

# tpack_png = os.path.join(root, ur"build\tex_lua_for_ep\background_player2.png")
# tpack_lua = os.path.join(root, ur"build\tex_lua_for_ep\tpack.lua")

# src_epgen_lua = os.path.join(root, ur"data\lua_for_ep\background_player2.lua")
# epgen_lua = 
# epgen_ep  = 
epgen_script = os.path.join(root, ur"..\tools\lua\epbin.lua")
epgen_share_dir = os.path.join(share_root, ur"ep")

lua_exe = os.path.join(root, ur"..\tools\lua\lua52.exe")

work_items = [
    {
        'lua': os.path.join(root, ur"data\background_player2.lua"),
        'ep': os.path.join(root, ur"data\background_player2.ep"),
    },
    {
        'lua': os.path.join(root, ur"data\background_player3.lua"),
        'ep': os.path.join(root, ur"data\background_player3.ep"),
    },
]

# print
# print "============================================================"
# print "pick: PNGS"
# print "============================================================"
# print
# 
# # from pick_pngs import do_pick_pngs
# # do_pick_pngs(raw_png_root, comp_cfg_xls, picked_png_root)
# 
# os.system('copy /Y "%s\*" "%s\"' % (raw_png_root, picked_png_root))
# 
# print
# print "============================================================"
# print "generate: tpack.lua tpack.png"
# print "============================================================"
# print
# 
# from pack_pngs import do_pack_pngs
# do_pack_pngs(tpack_png, tpack_lua, picked_png_root)
# 
# print
# print "============================================================"
# print "generate: tpack.py"
# print "============================================================"
# print
# 
# os.system('%s build_tpack_py.lua tpack.py %s' % (lua_exe, tpack_lua))
# 
# print
# print "============================================================"
# print "generate: buildings2.lua"
# print "============================================================"
# print
# 
# # from build_ep_lua import do_build_ep_lua
# # do_build_ep_lua(epgen_lua, comp_cfg_xls, anim_cfg_xls)
# 
# os.sysmte('copy /Y "%s" "%s"' % (src_epgen_lua, epgen_lua))


for item in work_items:
    epgen_ep = item['ep']
    epgen_lua = item['lua']

    print
    print "============================================================"
    print "generate: %s" % epgen_ep
    print "============================================================"
    print

    if os.path.isfile(epgen_ep):
        os.system('del /Q/F "%s"' % epgen_ep)

    os.system('%s "%s" -png8 "%s"' % (lua_exe, epgen_script, epgen_lua))

    print
    print "============================================================"
    print "copy: %s to %s" % (epgen_ep, epgen_share_dir)
    print "============================================================"
    print

    os.system('copy "%s" "%s"' % (epgen_ep, epgen_share_dir))

