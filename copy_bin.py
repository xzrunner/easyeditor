import os
import shutil

shutil.copy(r'.\easycomplex\platform\msvc2008\easycomplex\x86\Release\easycomplex.exe', r'..\ejoy\coco-tools\editor_bin\easycomplex.exe')
shutil.copy(r'.\easyanim\platform\msvc2008\easyanimation\x86\Release\easyanimation.exe', r'..\ejoy\coco-tools\editor_bin\easyanimation.exe')
shutil.copy(r'.\easyscale9\platform\msvc2008\easyscale9\x86\Release\easyscale9.exe', r'..\ejoy\coco-tools\editor_bin\easyscale9.exe')
shutil.copy(r'.\coceditor\platform\msvc2008\cocpackage\x86\Release\cocpackage.exe', r'..\ejoy\coco-tools\editor_bin\cocpackage_load.exe')
shutil.copy(r'.\easydb\platform\msvc2008\easydb_cl\x86\Release\easydb_cl.exe', r'..\ejoy\coco-tools\editor_bin\easydb_cl.exe')
shutil.copy(r'.\easydb\platform\msvc2008\easydb\x86\Release\easydb.exe', r'..\ejoy\coco-tools\editor_bin\easydb.exe')
shutil.copy(r'.\easyimage\platform\msvc2008\easyimage\x86\Release\easyimage.exe', r'..\ejoy\coco-tools\editor_bin\easyimage.exe')
shutil.copy(r'.\easymesh\platform\msvc2008\easymesh\x86\Release\easymesh.exe', r'..\ejoy\coco-tools\editor_bin\easymesh.exe')
shutil.copy(r'.\easyparticle3d\platform\msvc2008\easyparticle3d\x86\Release\easyparticle3d.exe', r'..\ejoy\coco-tools\editor_bin\easyparticle3d.exe')
shutil.copy(r'.\easyshape\platform\msvc2008\easyshape\x86\Release\easyshape.exe', r'..\ejoy\coco-tools\editor_bin\easyshape.exe')

os.system("pause")
