# -*- coding: utf-8 -*-

import os, sys

root = ur'..'
share_root = ur'e:\share\coc'
epgen_script = os.path.join(root, ur"..\tools\lua\epbin.lua")
epgen_share_dir = os.path.join(share_root, ur"ep")

lua_exe = os.path.join(root, ur"..\tools\lua\lua52.exe")

work_items = [
    {
        'lua': os.path.join(root, ur"data\misc.lua"),
        'ep': os.path.join(root, ur"data\misc.ep"),
    },
]

def convert_png_to_pvr(png):
    pvr = png[:-4] + ".pvr"
    os.system("IF EXIST out.plist del /F/Q out.plist")
    os.system("TexturePacker --premultiply-alpha --sheet %s --texture-format pvr2 --opt PVRTC2 %s" % (
        pvr,
        png
        ))
    os.system("IF EXIST out.plist del /F/Q out.plist")

for item in work_items:
    epgen_ep = item['ep']
    epgen_lua = item['lua']

    epgen_dir = os.path.dirname(epgen_lua)
    for v in os.listdir(epgen_dir):
        if v.lower().endswith('.png'):
            convert_png_to_pvr(os.path.join(epgen_dir, v))

    os.system("pause")

    print
    print "============================================================"
    print "generate: %s" % epgen_ep
    print "============================================================"
    print

    if os.path.isfile(epgen_ep):
        os.system('del /Q/F "%s"' % epgen_ep)

    os.system('%s "%s" -pvr "%s"' % (lua_exe, epgen_script, epgen_lua))

    print
    print "============================================================"
    print "copy: %s to %s" % (epgen_ep, epgen_share_dir)
    print "============================================================"
    print

    os.system('copy "%s" "%s"' % (epgen_ep, epgen_share_dir))

