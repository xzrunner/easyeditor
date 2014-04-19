# 打包粒子数据

import os

def _run_cmd(cmd):
    print cmd
    os.system(cmd.encode('cp936'))

cmd = "..\\editor_bin\\easydb_cl.exe pack data\\particles data\\out.lua"
_run_cmd(cmd)

os.system("pause")