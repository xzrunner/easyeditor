
import sys
import os
import re

_abspath = os.path.abspath
_pjoin = os.path.join

pwd = os.getcwd()

res = _abspath(_pjoin(pwd, '..', 'sg_ui'))

png = _pjoin(pwd, "ui2%d.png")
tex = _pjoin(pwd, "ui2%d.json")

tex2 = _pjoin(pwd, "ui2")

out = _pjoin(pwd, "ui2.ori.lua")

def _run_cmd(cmd):
    print "_run_cmd -------------::", cmd
    os.system(cmd.encode('cp936'))

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
    if call_tex_pack(png1, tex1, res, redundant_files):
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
    redundant_files = []
    assert call_tex_pack(png2, tex2, tmpdir2, redundant_files)
    assert not redundant_files

    _run_cmd('RD /Q/S "%s"' % tmpdir1)
    _run_cmd('RD /Q/S "%s"' % tmpdir2)

pack_textures()

# package
cmd = "cocpackage_load.exe %s %s %s" % (res, tex2, out)
_run_cmd(cmd) 

