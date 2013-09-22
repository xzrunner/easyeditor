# -*- coding: utf-8 -*-

import os

outf = None

def run_cmd(cmd):
    os.system(cmd.encode('cp936'))

def open_output(filename):
    global outf

    d = os.path.dirname(filename)
    if not os.path.isdir(d):
        run_cmd(u'mkdir "%s"' % d)

    outf = open(filename, "w")
    assert(outf)

def close_output():
    global outf
    outf.close()

def out(s, indent=0):
    global outf
    prefix = "\t" * indent
    outf.write(prefix)
    outf.write(s)
    outf.write("\n")
