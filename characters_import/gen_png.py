import sys
import os
import shutil
import time
from hashlib import md5

ani_page = 3
target_path = sys.argv[1]



tmp_dir = './tmp_gen'

def md5_file(file_name):
	m = md5()
	file = open(file_name, "rb")
	m.update(file.read())
	file.close()
	return m.hexdigest()

def _gen_md5_luafile(target_path, func):
	dir_list = os.listdir(target_path)
	for f in dir_list:
		path = target_path +'/'+f
		if os.path.isdir(path):
			_gen_md5_luafile(path, func)
		else :
			if os.path.splitext(path)[1] == '.png':
				func(path)

def gen_md5_luafile(target_path):
	file = open('gen_md5.lua', "w")
	file.write("return {\n")
	def __(path):
		file.write('\t ["%s"] = "%s",\n' % (path, md5_file(path)))
	_gen_md5_luafile(target_path, __)
	file.write("\n}")
	print("gen md5 luafile")
	file.close()

def _gen_ani(target_path):
	cmd = 'lua52 gen_fl.lua %s' % (target_path)
	print(cmd)
	os.system(cmd)

def _gen_dir(target_path):
	ret_list = []
	_i = 1
	os.mkdir(tmp_dir)
	_gen_path = '%s/tc%d/' % (tmp_dir, _i)
	ret_list.append(_gen_path)		
	# os.mkdir(_gen_path)

	count = 1
	for _dir in os.listdir(target_path):
		p_dir = target_path + '/' + _dir
		if os.path.isdir(p_dir):
			if count%(ani_page+1) == 0:
				_i = _i + 1
				_gen_path = '%s/tc%d/' % (tmp_dir, _i)
				ret_list.append(_gen_path)
				# os.mkdir(_gen_path)
			count = count + 1
			print('copy', p_dir, _gen_path)
			shutil.copytree(p_dir, _gen_path+_dir)

	# copy shadow.tga
	if os.path.isfile(target_path+'/shadow.tga'):
		shutil.copy(target_path+'/shadow.tga', tmp_dir+'/tc1/')

	return ret_list

def  _gen_png(tc_list):
	ret_list = []
	count = 1
	for tc in tc_list:
		cmd = 'TexturePacker --width 2048 --height 2048 --algorithm MaxRects --maxrects-heuristics Best --pack-mode Best --premultiply-alpha  --sheet ./characters2%d.png --texture-format png  --data ./characters2%d.lua  --format  corona-imagesheet --scale 0.5  %s' % (count, count, tc)
		ret_list.append('./characters2%d.png' % (count))
		count = count + 1
		print(cmd)
		os.system(cmd)
	return ret_list

def _gen_lua():
	print('copy '+target_path+'/animation.lua', './')
	shutil.copy(target_path+'/animation.lua', './')
	cmd = 'lua52  gen_pa.lua characters2'
	print(cmd)
	os.system(cmd)

def _gen_ep():
	cmd = 'lua52 ../tools/lua/epbin.lua -png8 ./characters2.lua'
	print(cmd)
	os.system(cmd)


# gen md5 lua file
gen_md5_luafile(target_path)

# gen animation lua file
_gen_ani(target_path)

# gen temp dir & copy png
tc_list = _gen_dir(target_path)

# gen png file
png_list = _gen_png(tc_list)

#gen lua file
_gen_lua()

# gen ep file
_gen_ep()