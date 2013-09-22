# -*- coding: utf-8 -*-

import os, sys, re

CHECK_DIR = 'json'

def handle_file(path):
    f = open(path, "r")
    c = f.read()
    f.close()

    rel = os.path.dirname(path)
    for m in re.finditer(r'filepath": "([^"]+)"', c):
        filepath = os.path.join(rel, m.group(1).replace('\\\\', '\\'))
        if not os.path.isfile(filepath):
            print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
            print path
            print filepath
            os.system("pause")
            sys.exit(1)

for root, dirs, names in os.walk(CHECK_DIR):
    for name in names:
        if name.lower().endswith('.json'):
            path = os.path.join(root, name)
            handle_file(path)
