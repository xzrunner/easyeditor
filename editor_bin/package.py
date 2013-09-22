
import sys
import os

pwd = os.getcwd()
res = pwd+"\\..\\sg_ui\\"

tex = pwd+"\\ui21.json"
tex2 = pwd+"\\ui2"
png = pwd+"\\ui21.png"

out = pwd+"\\ui2.ori.lua"

# texture packer
cmd = "TexturePacker  --algorithm MaxRects --maxrects-heuristics Best --pack-mode Best --premultiply-alpha  --sheet %s --texture-format png  --data %s  --format  json-array %s" % (png, tex, res)
print(cmd)
os.system(cmd)

# package
cmd = "cocpackage.exe %s %s %s" % (res, tex2, out)
print("\n"+cmd)
os.system(cmd) 

