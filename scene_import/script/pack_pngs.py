# -*- coding: utf-8 -*-

import os
from config import config

TMP_FILE = 'pack_pngs.output.txt'
PREFIX = 'TexturePacker:: warning: '

def _run_cmd(cmd):
    os.system(cmd.encode('cp936'))

def _pack_aux(out_image_filename, tpack_lua, png_root):
    cmd = ' '.join([
        'TexturePacker',
        '--max-width 2048',
        '--max-height 2048',
        '--algorithm MaxRects',
        '--maxrects-heuristics Best',
        '--pack-mode Best',
        '--premultiply-alpha',
        '--sheet "%s"' % out_image_filename,
        '--data "%s"' % tpack_lua,
        '--format corona-imagesheet',
        '--scale %s' % config.PACK_SCALE,
        '--scale-mode Smooth',
        config.TEX_PACKER_FLAGS,
        '"%s"' % png_root,
        '2> %s' % TMP_FILE
        ])
    _run_cmd(cmd)

    f = open(TMP_FILE, 'rb')
    lines = f.readlines()
    print("f:", f)
    print("contents:")
    for line in lines:
        print line.strip()
    print("EOF")
    f.close()

    os.system('del /Q/S "%s"' % TMP_FILE)

    if lines and len(lines) >= 1 and lines[0].strip() == 'TexturePacker:: error: Not all sprites could be packed into the texture!':
        return False

    assert(not lines)
    return True

def do_pack_pngs(basename, tpack_lua, png_root):
    assert(basename.lower().endswith(config.IMAGE_EXT))
    assert(tpack_lua.lower().endswith('.lua'))

    lua_list = []

    names = os.listdir(png_root)
    for name in names:
        if not name.startswith('pack_'):
            continue
        path = os.path.join(png_root, name)
        if not os.path.isdir(path):
            continue
        pack_index = int(name[5:])
        filename = basename[:-4] + str(pack_index) + config.IMAGE_EXT
        lua = tpack_lua[:-4] + str(pack_index) + ".lua"
        lua_list.append(lua)
        if not _pack_aux(filename, lua, path):
            return False, []

    return True, lua_list
