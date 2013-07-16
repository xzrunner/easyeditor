# -*- coding: utf-8 -*-

import os

def run_cmd(cmd):
    os.system(cmd.encode('cp936'))

def do_pack_pngs(tpack_png, tpack_lua, png_root):
    cmd = ' '.join([
        'TexturePacker',
        '--width 2048',
        '--height 2048',
        '--algorithm MaxRects',
        '--maxrects-heuristics Best',
        '--pack-mode Best',
        '--premultiply-alpha',
        '--sheet "%s"' % tpack_png,
        '--texture-format png',
        '--data "%s"' % tpack_lua,
        '--format corona-imagesheet',
        '--scale 0.5',
        '--scale-mode Smooth',
        '"%s"' % png_root,
        ])
    run_cmd(cmd)
