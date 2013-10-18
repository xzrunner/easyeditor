import os, sys

ex_name = sys.argv[1]

cmd = " python transform.py  ./coco_ui/ui.ori.lua  "+ex_name+" ./coco_ui/ui.ori."

print(cmd)
os.system(cmd)