# coding: utf8

'''
使用方法:

从 ..\sg_ui 中把要缩放的图片 copy 到 target 目录中
然后运行 main.py
结果会输出到 output 目中
'''

import os, sys
import Image
import json

SCALE_TO = 0.5
INV_SCALE_TO = 1.0 / SCALE_TO

def dir_wrap(base_dir):
    return lambda x: os.path.join(base_dir, x)

DIR_SG_UI = os.path.join('..', '..', 'sg_ui')
DIR_SG_UI_ = dir_wrap(DIR_SG_UI)

def get_source():
    return os.listdir('target')

def find_reference_in_complex(t, target):
    flag = False
    for s in t['sprite']:
        if s['filepath'].lower() == target.lower():
            # print s['x scale'], s['y scale']
            s['x scale'] *= INV_SCALE_TO
            s['y scale'] *= INV_SCALE_TO
            # print s['x scale'], s['y scale']
            flag = True
    return flag

def find_reference_in_anim(t, target):
    flag = False
    for layer in t['layer']:
        for frame in layer['frame']:
            for actor in frame['actor']:
                if actor['filepath'].lower() == target.lower():
                    actor['x scale'] *= INV_SCALE_TO
                    actor['y scale'] *= INV_SCALE_TO
                    flag = True
    return flag

def find_reference_in_9patch(t, target):
    flag = False
    for s in t['sprite']:
        if s['filepath'].lower() == target.lower():
            s['x scale'] *= INV_SCALE_TO
            s['y scale'] *= INV_SCALE_TO
    return flag

def find_reference_aux(t, path, target):
    path_ = path.lower()
    if path_.endswith('_complex.json'):
        return find_reference_in_complex(t, target)
    elif path_.endswith('_anim.json'):
        return find_reference_in_anim(t, target)
    elif path_.endswith('_9patch.json'):
        return find_reference_in_9patch(t, target)
    elif path_.endswith('_fontblank.json'):
        return False
    else:
        raise Exception("unknown json filetype:" + path)

def find_reference(target):
    for root, dirs, names in os.walk(DIR_SG_UI):
        for name in names:
            name_ = name.lower()
            if not name_.endswith('.json'):
                continue
            path = os.path.join(root, name)
            try:
                t = None
                with open(path, 'r') as f:
                    t = json.load(f)
                if find_reference_aux(t, path, target):
                    outpath = os.path.join('output', name)
                    with open(outpath, 'w') as f:
                        json.dump(t, f)
            except:
                print ">>>>>>>>>>>", name
                raise

def resize_image(name):
    img = Image.open(DIR_SG_UI_(name))
    width, height = img.size
    print "size:", width, height
    img = img.resize(
            ( int(float(width) * float(SCALE_TO))
            , int(float(height) * float(SCALE_TO))), Image.ANTIALIAS)
    img.save(os.path.join("output", name))

if __name__ == '__main__':
    for src in get_source():
        try:
            resize_image(src)
            find_reference(src)
        except:
            print "---------------", src
            raise

