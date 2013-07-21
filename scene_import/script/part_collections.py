# -*- coding: utf-8 -*-

d = {}

def register_part(name, ap):
    global d
    assert(name not in d)
    d[name] = ap

def get_part(name):
    if name not in d:
        raise Exception("part '%s' not found" % name)
    return d[name]
