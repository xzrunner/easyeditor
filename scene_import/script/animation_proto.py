# -*- coding: utf-8 -*-

from tpack import frames, indexes
from part_collections import get_part

class AnimationProto(object):
    def __init__(self):
        self.id = None
        self.export = None
        self.typename = None
        self.components = None
        self.actions = None

    def __repr__(self):
        return "[AnimationProto %d - %s]" % ((self.id or 0), (self.export or "?"))

    def output(self, out):
        # print self

        out("animation {")

        if self.export:
            out('export = "%s",' % self.export, 1)
        else:
            out('-- typename = "%s",' % self.typename, 1)

        # -- id
        assert(self.id)
        out('id = %d,' % self.id, 1)

        # -- component
        assert(self.components)
        out('component = {', 1)
        for comp in self.components:
            comp_name, comp_id = self.__get_comp_id(comp)
            if comp_name:
                out('{ name = "%s", id = %d },' % (comp_name, comp_id), 2)
            else:
                out('{ id = %d },' % comp_id, 2)
        out('},', 1)

        # -- actions
        if not self.actions:
            self.actions = self.__build_default_action()
        for action in self.actions:
            out("{", 1)
            if action['action_name'] != 'default':
                out('action = "%s",' % action['action_name'], 2)
            for frame in action['frames']:
                for x in xrange(frame['elapsed']):
                    out('{ %s },' % (','.join(map(str, frame['components']))), 2)
            out("},", 1)

        out("}") # close animation

    def __get_comp_id(self, comp):
        comp_type = comp['type']
        comp_name = comp['name']

        if comp_type == u'图片':
            assert(comp_name in indexes)
            comp_id = indexes[comp_name]
            return None, comp_id
        elif comp_type == u'turret部件':
            ref_ap = get_part(comp_name)
            return "turret", ref_ap.id
        elif comp_type == u'ammo部件':
            ref_ap = get_part(comp_name)
            return "ammo", ref_ap.id
        elif comp_type == u'resource部件':
            ref_ap = get_part(comp_name)
            return "resource", ref_ap.id
        else:
            print comp['type']
            raise Exception(u"unknown component type: " + comp['type'])

    def __build_default_action(self):
        components = range(len(self.components))
        return [ {
            'action_name': 'default',
            'frames': [ { 'elapsed':1, 'components': components } ],
            } ]
