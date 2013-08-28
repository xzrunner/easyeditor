import sys
import os
import shutil
import time
from hashlib import md5

ani_page = 1
target_path = sys.argv[1]
ani_page_list = [
['2001dashi1', '2002jumin1',
'2003daoke1', '2003daoke3', '2003daoke5', '2003daoke6',
'2004shenshe1', '2004shenshe3', '2004shenshe5','2004shenshe6',
'2005mojin1', '2005mojin3', '2005mojin5',
'2007sishi1', '2007sishi3','2007sishi5',],

['2006jiabing1', '2006jiabing3', '2006jiabing5', '2006jiabing6',
'2009daoshi1', '2009daoshi3', '2009daoshi5',
'2014yingyan1'],

['2008tianzai1', 
'2010shennv1', '2010shennv2', '2010shennv3',
'2011dapeng1'],

['2011dapeng2','2011dapeng3',
'2013baji1']

]

ani_map = {}
for l in ani_page_list:
	for k in l:
		ani_map[k] = True

tmp_dir = './tmp_gen'

def rm_ani(target_path):
	dir_list = os.listdir(target_path)
	for f in dir_list:
		path = target_path + '/'+f
		if os.path.isdir(path) and (not (f in ani_map.keys())):
			print('rm  ani', path)
			shutil.rmtree(path)

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

def gen_texture_luafile(file_list):
	file = open('gen_texture.lua', "w")
	file.write('return {\n')
	for texture_file in file_list:
		file.write('\t "%s",\n' % (texture_file))
	file.write('\n}')
	print('gen texture luafile')
	file.close()

def _gen_ani(target_path):
	cmd = 'lua52 gen_fl.lua %s' % (target_path)
	print(cmd)
	os.system(cmd)

def _gen_dir(target_path):
	ret_list = []
	_i =0
	os.mkdir(tmp_dir)

	for tc_idx, ani_list in enumerate(ani_page_list):
		_gen_path = '%s/tc%d/' % (tmp_dir, tc_idx+1)
		for ani in ani_list:
			p_dir = target_path + '/' + ani
			if os.path.isdir(p_dir):
				print('copy', p_dir, _gen_path)
				shutil.copytree(p_dir, _gen_path+ani)
		ret_list.append(_gen_path)
	# copy shadow.tga
	if os.path.isfile(target_path+'/shadow.tga'):
		shutil.copy(target_path+'/shadow.tga', tmp_dir+'/tc1/')
	return ret_list


def  _gen_png(tc_list):
	ret_list = []
	count = 1
	for tc in tc_list:
		cmd = 'TexturePacker  --algorithm MaxRects --maxrects-heuristics Best --pack-mode Best --premultiply-alpha  --sheet ./characters2%d.png --texture-format png  --data ./characters2%d.lua  --format  corona-imagesheet  %s' % (count, count, tc)
		l_file = './characters2%d.lua' %(count)
		print(cmd)
		os.system(cmd)
		if os.path.isfile(l_file):
			print('append %s file.' % (l_file))
			ret_list.append('characters2%d' %(count))
			count = count + 1
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

#del ani
rm_ani(target_path)

# gen md5 lua file
gen_md5_luafile(target_path)

# gen animation lua file
_gen_ani(target_path)


# gen temp dir & copy png
tc_list = _gen_dir(target_path)

# gen png file
texture_list = _gen_png(tc_list)

# gen texture lua file
gen_texture_luafile(texture_list)

#gen lua file
_gen_lua()

# gen ep file
_gen_ep()