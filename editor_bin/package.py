
import sys
import os
import re
import json

_abspath = os.path.abspath
_pjoin = os.path.join

pwd = os.getcwd()

tex_json = {}

res = _abspath(_pjoin(pwd, '..', 'sg_ui'))

png = _pjoin(pwd, "ui2%d.png")
tex = _pjoin(pwd, "ui2%d.json")

tex2 = _pjoin(pwd, "ui2")

out = _pjoin(pwd, "ui2.ori.lua")

def _run_cmd(cmd):
    print "_run_cmd -------------::", cmd
    os.system(cmd.encode('cp936'))

def _del_file(name):
    if os.path.isfile(name):
        _run_cmd('DEL /Q/F "%s"' % name)

# texture packer
def call_tex_pack(png, tex, res, redundant_files):
    TMP_FILE = _pjoin(pwd, 'texturepacker.output.log')
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
    path = _pjoin(pwd, name)
    if os.path.isdir(path):
        _run_cmd('RD /Q/S "%s"' % path)
    _run_cmd('MKDIR "%s"' % path)
    print "_tmpdir ==============>", path
    return path

def pack_textures():
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


def _get_pentagon_src(json_file):
    handle = open(json_file)
    str = handle.read()
    handle.close()
    data = json.loads(str)
    for f in data["frames"]:
        if f["filename"] == "6450-gaoguang-1.png":
            frame = f["frame"]
            return [frame["x"]+2, frame["y"]+frame["h"]-2,  frame["x"]+frame["w"]-2, frame["y"]+frame["h"]-2,  frame["x"]+frame["w"]-2, frame["y"]+2,  frame["x"]+2, frame["y"]+2]
    return False

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
    cmd = "python ../pentagon_import/gen_pentagon.py %s %d ../pentagon_import/character.csv ../pentagon_import/pt_char.lua 19 %d"  % (src_pos, tex, 10527)
    _run_cmd(cmd)
    # gen hero
    cmd = "python ../pentagon_import/gen_pentagon.py %s %d ../pentagon_import/hero.csv ../pentagon_import/pt_hero.lua 7 %d" % (src_pos, tex, 11527)
    _run_cmd(cmd)

# texturepacker
pack_textures()

# pentagon
run_pentagon()

# package
cmd = "cocpackage_load.exe %s %s %s" % (res, tex2, out)
_run_cmd(cmd) 

# merge 
cmd = "type ..\\pentagon_import\\pt_char.lua >> %s" % out
_run_cmd(cmd)
cmd = " type ..\\pentagon_import\\pt_hero.lua >> %s" % out
_run_cmd(cmd)
