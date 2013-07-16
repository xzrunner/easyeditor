# -*- coding: utf-8 -*-

outf = None

def open_output(filename):
    global outf
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
