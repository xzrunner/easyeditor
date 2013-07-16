# -*- coding: utf-8 -*-

d = {}

def register_part(name, ap):
    global d
    assert(name not in d)
    d[name] = ap

def get_part(name):
    assert(name in d)
    return d[name]
