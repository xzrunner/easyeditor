# -*- coding: utf-8 -*-

import os, sys, re
from subprocess import Popen

PNG_DIR = r'..\data\pngs'
JSON_DIR = r'..\data\json'

for name in os.listdir(PNG_DIR):
    if name.endswith('.PNG'):
        new_name = name.lower()
        print PNG_DIR, ":", name, "->", new_name
        Popen(['cmd', '/C', 'ren', name, new_name],  cwd=PNG_DIR)

for root, dirs, names in os.walk(JSON_DIR):
    for name in names:
        if name.lower().endswith('.json'):
            inp = os.path.join(root, name)
            f = open(inp, "rb")
            c = f.read()
            f.close()

            if c.find(".PNG") == -1:
                continue

            bak = os.path.join(root, name + ".tmp")
            f = open(bak, "wb")
            f.write(c)
            f.close()

            print ">>>>", name
            c, n = re.subn(r'(filepath"\s*:\s*"[^"]+)\.PNG"', r'\1.png"', c)
            print "<<<<", n

            oup = os.path.join(root, name)
            f = open(oup, "wb")
            f.write(c)
            f.close()
