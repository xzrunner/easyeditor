# -*- coding: utf-8 -*-

import os
import re
from output import open_output, close_output, out
from id_service import alloc_id
from tpack import frames, indexes
from part_collections import get_part
from picture import Picture
from config import config
from global_state import png_map

def relpath_from(dst, src):
    common_prefix = os.path.commonprefix([dst, src])
    src_dir = os.path.dirname(src)
    rel1 = os.path.relpath(common_prefix, src_dir)
    rel2 = os.path.relpath(dst, common_prefix)
    rlt = os.path.join(rel1, rel2)
    if rlt.startswith('.\\'):
        return rlt[2:]
    else:
        return rlt


class JsonAnimation(object):
    def __init__(self):
        self.export = None
        self.typename = None
        self.components = None
        self.actions = None

    def __repr__(self):
        return "[J-ANI %s]" % (self.export or "?")

    # def extract_pic(self, all_pictures, pic_map):
    #     for comp in self.components:
    #         if comp['type'] != u'图片':
    #             continue
    #         pic_name = comp['name']
    #         pic_mirror = comp['mirror']
    #         pic_key = "%s:%s" % (pic_name, pic_mirror)
    #         if pic_key in pic_map:
    #             comp['pic'] = pic_map[pic_key]
    #         else:
    #             p = Picture(pic_name, pic_mirror)
    #             comp['pic'] = p
    #             pic_map[pic_key] = p
    #             all_pictures.append(p)

    def output(self):
        print self

        assert(self.typename)

        m = re.match(r".*_(\d\d\d\d)[ag]?.*", self.typename)
        if not m:
            os.system("!!!!!!!!!!!!!!!!!!!!!!!!!", self.typename)
        art_id = m.group(1)
        json_path = os.path.join(config.OUTDIR_JSON, art_id, self.typename+"_complex.json")
        open_output(json_path)

        out("{")

        if self.export:
            out('"name": "%s",' % self.export, 1)
        else:
            out('// "%s' % self.typename, 1)

        # -- component
        assert(self.components)
        out('"sprite": [', 1)
        for comp in self.components:
            comp_name, comp_path = self.__get_comp_id(comp)

            if not os.path.isfile(comp_path):
                out('// FIXME: %s, %s' % (comp_name, comp_path), 2)
                continue

            out('{', 2)
            out('"angle": 0.0,', 3)
            out('"filepath": "%s",' % relpath_from(comp_path, json_path).replace('\\', '\\\\'), 3)
            out('"name": "%s",' % comp_name, 3)
            out('"position": {', 3)
            out('"x": 0.0,', 4)
            out('"y": 0.0', 4)
            out('},', 3)
            out('"scale": 1.0,', 3)
            out('"x mirror": false,', 3)
            out('"y mirror": false', 3)

            if comp == self.components[-1]:
                out('}', 2)
            else:
                out('},', 2)
        out(']', 1)

        # # -- actions
        # if not self.actions:
        #     self.actions = self.__build_default_action()
        # for action in self.actions:
        #     out("{", 1)
        #     if action['action_name'] != 'default':
        #         out('action = "%s",' % action['action_name'], 2)
        #     for frame in action['frames']:
        #         for x in xrange(frame['elapsed']):
        #             out('{ %s },' % (','.join(map(str, frame['components']))), 2)
        #     out("},", 1)

        out("}") # close animation

        close_output()

    def __get_comp_id(self, comp):
        comp_type = comp['type']
        comp_name = comp['name']

        if comp_type == u'图片':
            assert comp_name in png_map, comp_name
            return "", png_map[comp_name]
        elif comp_type == u'turret部件':
            ref_ap = get_part(comp_name)
            return "turret", comp_name
        elif comp_type == u'ammo部件':
            ref_ap = get_part(comp_name)
            return "ammo", comp_name
        elif comp_type == u'resource部件':
            ref_ap = get_part(comp_name)
            return "resource", comp_name
        elif comp_type == u'部件':
            ref_ap = get_part(comp_name)
            return "", comp_name
        else:
            print comp['type']
            raise Exception(u"unknown component type: " + comp['type'])

    def __build_default_action(self):
        components = range(len(self.components))
        return [ {
            'action_name': 'default',
            'frames': [ { 'elapsed':1, 'components': components } ],
            } ]
