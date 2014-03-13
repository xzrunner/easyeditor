# -*- coding: utf-8 -*-

import os, sys
import re
import json

j = os.path.join

for root, dirs, names in os.walk(j('..', 'data', 'editor_json')):
    for name in names:
        if not name.lower().endswith('_complex.json'):
            continue
        m = re.match(r'(building_\d+_lv\d+)_complex.json', name, re.I)
        if not m:
            continue
        path = j(root, name)
        expect_name = m.group(1)
        with open(path, 'r') as f:
            t = json.load(f)
        if t['name'] != expect_name:
            print "==== NAME ERROR ===="
            print "path:", path
            print "expect name:", expect_name
            print "given name:", t['name']
            t['name'] = expect_name

            print "fix it ..."
            with open(path, 'w') as f:
                json.dump(t, f, indent=4)
