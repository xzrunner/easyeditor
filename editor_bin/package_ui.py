# -*- coding:utf-8 -*- 
README = \
"usepage:" \
"pyhton package_ui.py <module>  [target]" \
"module:" \
"   -load:  只是生成ui2.ori.lua" \
"   -pack:  生成png贴图以及lua文件" \
"target:" \
"   common: 默认打包目标"\
"   360: 打包360"\
"   efun: 打包efun"


import sys
import os
import re
import json

_abspath = os.path.abspath
_pjoin = os.path.join

modle = sys.argv[1]
target = "common"
if len(sys.argv)>=3:
    target = sys.argv[2]


def _run_cmd(cmd):
    print "_run_cmd -------------::", cmd
    os.system(cmd.encode('cp936'))

def _mkdir(name):
    path = _pjoin(pwd, name)
    if not os.path.isdir(path):
        _run_cmd('MKDIR "%s"' % path)
    return path


pwd = os.getcwd()

PYTHON = _pjoin(pwd, '..', 'tools', 'Python27', 'python.exe')

tex_json = {}


pack_dir = _abspath(_pjoin(pwd, "../", "sg_ui"))
base_res = _abspath(_pjoin(pwd, '..', 'sg_ui/common'))
sub_res_map = {
    '360' :  _abspath(_pjoin(pwd, '..', 'sg_ui/360')),
    'efun' : _abspath(_pjoin(pwd, '..', 'sg_ui/efun'))
}

tmp_pack = _abspath(_pjoin(pwd, "tmp_pack"))
res = tmp_pack

_mkdir('ui_output')
png = _pjoin(pwd, 'ui_output', "ui2%d.png")
tex = _pjoin(pwd, 'ui_output', "ui2%d.json")
tex2 = _pjoin(pwd, 'ui_output', "ui2")
out = _pjoin(pwd, 'ui_output', "ui2.ori.lua")

ui2_conver = _pjoin(pwd, "ui_output", "ui2.conver.lua")
ui2_manual = _pjoin(pwd, "ui_output", "ui2.manual.lua")
ui2 = _pjoin(pwd, "ui_output", "ui2.lua")
ep = _pjoin(pwd, "ui_output", "ui2")
epbin = _pjoin(pwd, "../tools/lua/epbin.lua")

def _rm_tmp_pack_dir():
    if os.path.exists(tmp_pack):
        _run_cmd('RD /Q/S "%s"' % tmp_pack)

def _have_file(dir):
    for file in os.listdir(dir):
        return True
    return False


def _gen_pack_dir():
    global res
    if sub_res_map.has_key(target):
        target_path = sub_res_map[target]
        if _have_file(target_path+"\\png"):
            _mkdir(res)
            _run_cmd('COPY /Y "%s\\png\\*.png" "%s\\"' % (base_res, res))
            _run_cmd('COPY /Y "%s\\png\\*.png" "%s\\"' % (target_path, res))
            return
    res = base_res + "\\png"

def _gen_jsons_dir():
    global res
    if sub_res_map.has_key(target):
        target_path = sub_res_map[target]
        if _have_file(target_path+"\\json"):
            _mkdir(res)
            _run_cmd('COPY /Y "%s\\json\\*.json" "%s\\"' % (base_res, res))
            _run_cmd('COPY /Y "%s\\json\\*.json" "%s\\"' % (target_path, res))
            return
    res = base_res + "\\json"

def _del_file(name):
    if os.path.isfile(name):
        _run_cmd('DEL /Q/F "%s"' % name)

# texture packer
def call_tex_pack(png, tex, res, redundant_files):
    TMP_FILE = _pjoin(pwd, 'ui_output', 'texturepacker.output.log')
    if os.path.isfile(TMP_FILE):
        _run_cmd('del /Q/F "%s"' % TMP_FILE)

    cmd = ' '.join([
        'TexturePacker',
        '--algorithm MaxRects',
        '--maxrects-heuristics Best',
        '--pack-mode Best',
        '--premultiply-alpha',
        '--sheet %s' % png,
        '--texture-format png',
        '--extrude 1',
        '--data %s' % tex,
        '--format json-array',
        '"%s"' % res,
        '2> %s' % TMP_FILE,
        ])
    _run_cmd(cmd)

    f = open(TMP_FILE, 'rb')
    lines = f.readlines()
    print "<<---------------------------------------------->>"
    for line in lines:
        print line.strip()
    print "<<<EOF>>>"
    f.close()

    _run_cmd('del /Q/F "%s"' % TMP_FILE)

    if lines and len(lines) >= 1:
        assert lines[0].strip() == 'TexturePacker:: error: Not all sprites could be packed into the texture!', lines[0].strip()
        for line in lines[1:]:
            m = re.match(r'^TexturePacker:: warning: (.*)$', line.strip())
            assert m, line.strip()
            redundant_files.append(m.group(1))
        print "FALSE!"
        return False

    assert not lines
    print "TRUE!"
    return True

def _tmpdir(name):
    path = _pjoin(pwd, 'ui_output', name)
    if os.path.isdir(path):
        _run_cmd('RD /Q/S "%s"' % path)
    _run_cmd('MKDIR "%s"' % path)
    print "_tmpdir ==============>", path
    return path

def pack_textures():
    _gen_pack_dir()
    png1 = png % 1
    tex1 = tex % 1
    redundant_files = []
    tex_json[0] = tex1
    if call_tex_pack(png1, tex1, res, redundant_files):
        _del_file('ui22.png')
        _del_file('ui22.json')
        return

    print "redundant_files:", redundant_files
    tmpdir1 = _tmpdir('tmp1')
    tmpdir2 = _tmpdir('tmp2')

    _run_cmd('COPY /Y "%s\\*.png" "%s\\"' % (res, tmpdir1))
    for name in redundant_files:
        _run_cmd('MOVE "%s\\%s" "%s\\"' % (tmpdir1, name, tmpdir2))

    png1 = png % 1
    tex1 = tex % 1
    redundant_files = []
    assert call_tex_pack(png1, tex1, tmpdir1, redundant_files)
    assert not redundant_files

    png2 = png % 2
    tex2 = tex % 2
    tex_json[1] = tex2
    redundant_files = []
    assert call_tex_pack(png2, tex2, tmpdir2, redundant_files)
    assert not redundant_files

    _run_cmd('RD /Q/S "%s"' % tmpdir1)
    _run_cmd('RD /Q/S "%s"' % tmpdir2)
    _rm_tmp_pack_dir()


def _json2map(json_file):
    handle = open(json_file)
    str = handle.read()
    handle.close()
    return json.loads(str)

def _get_pentagon_src(json_file):
    data = _json2map(json_file)
    for f in data["frames"]:
        if f["filename"] == "6450-gaoguang-1.png":
            frame = f["frame"]
            return [frame["x"]+2, frame["y"]+frame["h"]-2,  frame["x"]+frame["w"]-2, frame["y"]+frame["h"]-2,  frame["x"]+frame["w"]-2, frame["y"]+2,  frame["x"]+2, frame["y"]+2]
    return False


######################################### pentagon ########################################################
def get_pentagon():
    for k in tex_json:
        v = tex_json[k]
        src = _get_pentagon_src(v)
        if src:
            src_pos = ('"%d,%d, %d,%d, %d,%d, %d,%d"' % (src[0],src[1],  src[2],src[3], src[4],src[5], src[6],src[7]))
            tex = k
            return src_pos, tex
    assert(False)

def run_pentagon():
    src_pos, tex = get_pentagon()
    # gen char
    cmd = '%s ../pentagon_import/gen_pentagon.py %s %d ../pentagon_import/character.csv ../pentagon_import/pt_char.lua 19 %d'  % (PYTHON, src_pos, tex, 10527)
    _run_cmd(cmd)
    # gen hero
    cmd = '%s ../pentagon_import/gen_pentagon.py %s %d ../pentagon_import/hero.csv ../pentagon_import/pt_hero.lua 7 %d' % (PYTHON, src_pos, tex, 11527)
    _run_cmd(cmd)


######################################### gen ep ########################################################
def gen_ep():
    _del_file(ui2)
    _run_cmd("type %s >> %s" % (ui2_conver, ui2))
    _run_cmd("type %s >> %s" % (out, ui2))
    _run_cmd("type %s >> %s" % (ui2_manual, ui2))

    print("---------gen ios ep file.----------------")
    _run_cmd(r"..\tools\lua\lua %s -pd -png8 %s %s.ios" % (epbin, ui2, ep))
    print("---------gen android ep file.----------------")
    _run_cmd(r"..\tools\lua\lua %s -pd -pkmc %s %s.android" % (epbin, ui2, ep))


######################################### package ########################################################
def pack_load():
    _gen_jsons_dir()
    cmd = "cocpackage_load.exe %s %s %s" % (res, tex2, out)
    _run_cmd(cmd) 
    _rm_tmp_pack_dir()

######################################### merge ######################################################## 
def pack_merge():
    cmd = "type ..\\pentagon_import\\pt_char.lua >> %s" % out
    _run_cmd(cmd)
    cmd = " type ..\\pentagon_import\\pt_hero.lua >> %s" % out
    _run_cmd(cmd)

######################################### deff ########################################################
def  _gen_exist_filelist():
    deff = {}
    for k in tex_json:
        v = tex_json[k]
        data = _json2map(v)
        for frame_data in data['frames']:
            filename = frame_data['filename']
            fe = os.path.splitext(filename)
            deff[fe[0]] = True
    return deff

def _gen_update_filelist():
    up_file = []
    deff = _gen_exist_filelist()
    files = os.listdir(res)
    for f in files:
        fe = os.path.splitext(f)
        if fe[1] == '.png' and deff.has_key(fe[0]) != True:
            print('catch file:', f, deff.has_key(f))
            up_file.append(f)
    return up_file

def pack_up():
    up_file = _gen_update_filelist()
    tmpup = _tmpdir('tmp_up')
    for k in up_file:
        _run_cmd('COPY /Y "%s\\%s" "%s\\"' % (res, k, tmpup))
    
    redundant_files = []
    png_up = png % 3
    tex_up = tex % 3
    call_tex_pack(png_up, tex_up, tmpup, redundant_files)
    _run_cmd('RD /Q/S "%s"' % tmpup)

########################################################
######################## run ###########################
########################################################
if modle == '-load':
    tex_json[0] = tex % 1
    tex_json[1] = tex % 2
    run_pentagon()
    pack_load()
    pack_merge()
    gen_ep()

elif modle == '-pack':
    pack_textures()
    run_pentagon()
    pack_load()
    pack_merge()
    gen_ep()

elif modle == '-update':
    tex_json[0] = tex % 1
    tex_json[1] = tex % 2
    pack_up()
    run_pentagon()
    pack_load()
    pack_merge()
    gen_ep()

else  :
    print(README)
