# -*- coding: utf-8 -*-

import os, sys
from xls_loader import load_xls
from tpack import frames, indexes
from id_service import set_last_id
from output import open_output, close_output, out
from part_collections import register_part, get_part
from picture import Picture

# CFG_ROOT = ur'..\data\xls'
# COMP_FILE = u'场景建筑-图素表.xls'
# ANIM_FILE = u'场景建筑-动画表.xls'

# assert(len(sys.argv) == 2)

# output_filename = sys.argv[1]

def do_build_ep_lua(output_filename, comp_cfg_xls, anim_cfg_xls):
    open_output(output_filename)

    out("texture(1)")

    # ======================================================================
    #
    # Pictures
    #
    # ======================================================================

    pic_index_name_map = {}
    for name, index in indexes.iteritems():
        pic_index_name_map[index] = name

    for i, frame in enumerate(frames):
        name = pic_index_name_map[i]
        set_last_id(i)
        Picture(name, i, frame).output(out)

    # ======================================================================
    #
    # Animations
    #
    # ======================================================================

    all_animations = []

    print u"开始处理图素表 ------------------------------------------"
    from cfg_proc_comp import component_handler
    sheets = load_xls(comp_cfg_xls)
    for sheet_name, line_tokens in sheets:
        print "sheet_name:", sheet_name
        component_handler(sheet_name, line_tokens, all_animations)

    for ap in all_animations:
        if ap.typename:
            register_part(ap.typename, ap)

    print u"开始处理动画表 ------------------------------------------"
    from cfg_proc_anim import animation_handler
    sheets = load_xls(anim_cfg_xls)
    for sheet_name, line_tokens in sheets:
        print "sheet_name:", sheet_name
        results = animation_handler(sheet_name, line_tokens)
        for info in results:
            print("====", info['typename'])
            ap = get_part(info['typename'])
            ap.actions = info['actions']

    for ap in all_animations:
        ap.output(out)

    close_output()
