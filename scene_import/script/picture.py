# -*- coding: utf-8 -*-

from id_service import alloc_id
from tpack import tex_count, frames, indexes

PACK_SCALE = 0.5
OUT_SCALE = 8.0

# ======================================================================
#
# Pictures
#
# ======================================================================

class Picture(object):
    def __init__(self, name, mirror):
        self.name = name
        self.id = None
        self.mirror = mirror

        assert(name in indexes)

        frame = frames[indexes[name]]
        self.tex = frame['tex']

        x, y, w, h = frame['x'], frame['y'], frame['width'], frame['height']
        self.src = (x, y, x+w, y+h)

        xx, yy, ww, hh = frame['sourceX'], frame['sourceY'], frame['sourceWidth'], frame['sourceHeight']
        cx, cy = ww*0.5, hh*0.5
        xx1 = xx - cx
        yy1 = yy - cy
        xx2 = xx1 + w
        yy2 = yy1 + h

        k = OUT_SCALE/PACK_SCALE
        xx1 *= k
        yy1 *= k
        xx2 *= k
        yy2 *= k

        assert(xx1 % 1 == 0)
        assert(yy1 % 1 == 0)
        assert(xx2 % 1 == 0)
        assert(yy2 % 1 == 0)

        if mirror:
            self.screen = (xx2, yy1, xx1, yy2)
        else:
            self.screen = (xx1, yy1, xx2, yy2)

    def alloc_id(self):
        self.id = alloc_id()

    def output(self, out):
        assert(self.id)
        x11, y11, x12, y12 = self.src
        x21, y21, x22, y22 = self.screen
        out('picture { -- %s' % self.name)
        out("id = %d," % self.id, 1)
        out("{ tex = %d, src = { %d,%d, %d,%d, %d,%d, %d,%d }, screen = { %d,%d, %d,%d, %d,%d, %d,%d }}," % (self.tex,
                x11,y11, x12,y11, x12,y12, x11,y12,
                x21,y21, x22,y21, x22,y22, x21,y22), 1)
        out("}")
