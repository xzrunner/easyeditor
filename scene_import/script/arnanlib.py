# -*- coding: utf-8 -*-

import os, sys
import hashlib

ST_DELETE = 1
ST_NOCHANGE = 2
ST_CHANGED = 3
ST_ADD = 4

def getfilemd5(filename):
    md5 = hashlib.md5()
    with open(filename,'rb') as f: 
        for chunk in iter(lambda: f.read(128*md5.block_size), b''): 
             md5.update(chunk)
    return md5.hexdigest()

def diffdir(dir1, dir2, **opt):
    skip_dirs = set([x.lower() for x in opt.get('skip_dirs', '').split(';')])
    skip_exts = set([x.lower() for x in opt.get('skip_exts', '').split(';')])
    difflist = []
    diffmap = {}

    for root, dirs, names in os.walk(dir1):
        for dirname in dirs:
            if dirname in skip_dirs:
                dirs.remove(dirname)
        for name in names:
            if skip_exts:
                _, ext_name = os.path.splitext(name)
                if ext_name in skip_exts:
                    continue
            path = os.path.join(root, name)
            rpath = os.path.relpath(path, dir1).lower()
            md5 = getfilemd5(path)
            difflist.append(rpath)
            diffmap[rpath] = [ST_DELETE, md5, None]

    for root, dirs, names in os.walk(dir2):
        for dirname in dirs:
            if dirname in skip_dirs:
                dirs.remove(dirname)
        for name in names:
            if skip_exts:
                _, ext_name = os.path.splitext(name)
                if ext_name in skip_exts:
                    continue
            path = os.path.join(root, name)
            rpath = os.path.relpath(path, dir2).lower()
            md5 = getfilemd5(path)
            if rpath in diffmap:
                entry = diffmap[rpath]
                entry[2] = md5
                if entry[1] == entry[2]:
                    entry[0] = ST_NOCHANGE
                else:
                    entry[0] = ST_CHANGED
            else:
                difflist.append(rpath)
                diffmap[rpath] = [ST_ADD, None, md5]

    return difflist, diffmap

def diffdir_print(difflist, diffmap, **opt):
    ext_list = opt.get('ignore_ext', '').split(',')
    ext_map = set([x.lower() for x in ext_list])
    f = opt.get('out', sys.stdout)
    for rpath in difflist:
        _, extname = os.path.splitext(rpath)
        if extname.lower() in ext_map:
            continue
        entry = diffmap[rpath]
        rpath = rpath.encode('utf-8')
        st = entry[0]
        if st == ST_DELETE:
            f.write("-\t"); f.write(rpath); f.write("\n")
        elif st == ST_NOCHANGE:
            if not opt.get('ignore_nochange', False):
                f.write("=\t"); f.write(rpath); f.write("\n")
        elif st == ST_CHANGED:
            f.write("!\t"); f.write(rpath); f.write("\n")
        elif st == ST_ADD:
            f.write("+\t"); f.write(rpath); f.write("\n")
        else:
            raise Exception("unknown state: " + st)

