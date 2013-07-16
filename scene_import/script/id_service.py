# -*- coding: utf-8 -*-

last_id = 0

def set_last_id(n):
    global last_id
    last_id = n

def alloc_id():
    global last_id
    last_id += 1
    return last_id
