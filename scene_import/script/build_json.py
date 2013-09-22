# -*- coding: utf-8 -*-

import os, sys
from xls_loader import load_xls
from tpack import tex_count, frames, indexes
from id_service import set_last_id
from output import open_output, close_output, out
from part_collections import register_part, get_part

def do_build_json(comp_cfg_xls, anim_cfg_xls):
    # open_output(output_filename)

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

    # all_pictures = []
    # pic_map = {}

    # for ap in all_animations:
    #     ap.extract_pic(all_pictures, pic_map)

    # for pic in all_pictures:
    #     pic.alloc_id()

    # for ap in all_animations:
    #     ap.alloc_id()

    # for pic in all_pictures:
    #     pic.output(out)

    for ap in all_animations:
        ap.output()
        # ap.output(out)

    # close_output()
