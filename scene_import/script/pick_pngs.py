# -*- coding: utf-8 -*-

import os, sys
from xls_loader import load_xls

# RAW_DATA_ROOT = ur'D:\dev_coco\tmp_building_import\data\场景建筑'
# CFG_ROOT = ur'D:\dev_coco\tmp_building_import\data\场景建筑配置'
# DST_ROOT = ur'D:\dev_coco\tmp_building_import\build\pngs'
# COMP_FILE = u'场景建筑-图素表.xls'
# ANIM_FILE = u'场景建筑-动画表.xls'

# RAW_DATA_ROOT = sys.argv[1]
# XLS_PATH = sys.argv[2]
# DST_ROOT = sys.argv[3]

def run_cmd(cmd):
    os.system(cmd.encode('cp936'))

def handler(sheet_name, line_tokens, results):
    for row, tokens in enumerate(line_tokens):
        if row <= 1:
            continue
        element_type = tokens[4]
        if element_type != u'图片':
            continue
        name = tokens[5]
        if not name:
            continue
        pack_index = tokens[6]
        results.append((name, pack_index))

def do_pick_pngs(raw_png_root, xls_filepath, output_root):
    raw_data_map = {}
    for root, dirs, names in os.walk(raw_png_root):
        if 'ignore' in dirs:
            dirs.remove('ignore')
        for name in names:
            if not name.lower().endswith('.png'):
                continue
            path = os.path.join(root, name)
            name = name.lower()
            if name in raw_data_map:
                print "error! duplicated name:", name
                print "A)", path
                print "B)", raw_data_map[name]
                sys.exit(1)
            raw_data_map[name] = path


    # path = os.path.join(XLS_PATH, COMP_FILE)

    sheets = load_xls(xls_filepath)

    all_files = []

    for sheet_name, line_tokens in sheets:
        print sheet_name
        handler(sheet_name, line_tokens, all_files)

    if os.path.isdir(output_root):
        run_cmd(u'rd /Q/S "%s"' % output_root)
    run_cmd(u'mkdir "%s"' % output_root)


    for name, pack_index in all_files:
        key = name.lower() + ".png"
        # print name
        # print raw_data_map[key]

        pack_dir = os.path.join(output_root, 'pack_%d' % pack_index)
        if not os.path.isdir(pack_dir):
            run_cmd(u'mkdir "%s"' % pack_dir)

        src = raw_data_map[key]
        dst = os.path.join(pack_dir, name+".png")
        # print "src:", src
        # print "dst:", dst

        cmd = u'copy /Y "%s" "%s" > NUL' % (src, dst)
        run_cmd(cmd)
