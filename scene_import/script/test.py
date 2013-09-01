# -*- coding: utf-8 -*-

dir1 = ur'D:\dev_coco\coco-tools\scene_import\data\raw_pngs'
dir2 = ur'D:\dev_coco\coco-tools\scene_import\data\export'

import arnanlib

f = open('testresult.txt', 'wb')
assert(f)

difflist, diffmap = arnanlib.diffdir(dir1, dir2,
        skip_dirs='ignore', skip_exts='.py')
arnanlib.diffdir_print(difflist, diffmap,
        ignore_nochange=True, out=f, ignore_ext='.dat')

f.close()
