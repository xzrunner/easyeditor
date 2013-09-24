import os, sys
import json
import math
import Image

tmp_dir = './tmp_gen'
output_dir = './output'
if not os.path.exists(tmp_dir):
	os.mkdir(tmp_dir)
if not os.path.exists(output_dir):
	os.mkdir(output_dir)

if len(sys.argv) != 4:
	print "need 1.src file; 2.export name; 3.png's package"
else:
	imgpath = sys.argv[3]
images = []
index = 1
while os.path.isfile(imgpath+str(index)+'.png'):
	images.append(Image.open(imgpath+str(index)+'.png'))
	index += 1


# 0 3
# 1 2
def quad_type(x, y, quad):
	cx = (quad[0]+quad[2]+quad[4]+quad[6])/4
	cy = (quad[1]+quad[3]+quad[5]+quad[7])/4
	if x < cx and y < cy:
		return 0
	elif x < cx and y > cy:
		return 1
	elif x > cx and y > cy:
		return 2
	elif x > cx and y < cy:
		return 3
	else:
		return -1


def format_dquad(squad, dquad):
	assert(squad[1] == (squad[0]+3)%4)
	if squad[0] == 0:
		return dquad
	elif squad[1] == 0:
		return [dquad[1], dquad[2], dquad[3], dquad[0]]
	elif squad[2] == 0:
		return [dquad[2], dquad[3], dquad[0], dquad[1]]
	elif squad[3] == 0:
		return [dquad[3], dquad[0], dquad[1], dquad[2]]
	else:
		assert(0)


def decode_pic(pic):
	json_file = open(tmp_dir+'/'+pic)
	json_data = json.load(json_file)
	json_file.close()

	out = {}
	out["name"] = json_data.get("export", "")
	out["sprite"] = []
	for item in json_data:
		if item.isdigit():
			src = json_data[item]
			dst = {}
			dst["filepath"] = str(json_data["id"])+'_'+item+'.png'

			src_data = src["src"]
			squad = [
				quad_type(src_data[0], src_data[1], src_data),
				quad_type(src_data[2], src_data[3], src_data),
				quad_type(src_data[4], src_data[5], src_data),
				quad_type(src_data[6], src_data[7], src_data)
			]
			screen_data = src["screen"]
			dquad = [
				quad_type(screen_data[0], screen_data[1], screen_data),
				quad_type(screen_data[2], screen_data[3], screen_data),
				quad_type(screen_data[4], screen_data[5], screen_data),
				quad_type(screen_data[6], screen_data[7], screen_data)
			]
			dquad = format_dquad(squad, dquad)
#			print squad, dquad

			max_x = max(src_data[0], src_data[2], src_data[4])
			min_x = min(src_data[0], src_data[2], src_data[4])
			max_y = max(src_data[1], src_data[3], src_data[5])
			min_y = min(src_data[1], src_data[3], src_data[5])
			images[src["tex"]].crop((min_x, min_y, max_x, max_y)).save(output_dir+'/'+dst["filepath"])

			dst["name"] = ''
			dst["position"] = {}
			dst["position"]["x"] = (screen_data[0]+screen_data[2]+screen_data[4]+screen_data[6])/4/16.0
			dst["position"]["y"] = (screen_data[1]+screen_data[3]+screen_data[5]+screen_data[7])/4/16.0
			dst["x shear"] = 0
			dst["y shear"] = 0

			rotate = False
			dst["angle"] = 0
			dst["x mirror"] = False
			dst["y mirror"] = False
			if dquad[0] == 1 and dquad[1] == 2 and dquad[2] == 3 and dquad[3] == 0:
				dst["x mirror"] = True
				dst["angle"] = math.pi
			elif dquad[0] == 3 and dquad[1] == 0 and dquad[2] == 1 and dquad[3] == 2:
				dst["x mirror"] = True
			elif dquad[0] == 2 and dquad[1] == 3 and dquad[2] == 0 and dquad[3] == 1:
				dst["y mirror"] = True
				dst["angle"] = -math.pi*0.5
				rotate = True					
			elif dquad[0] == 3 and dquad[1] == 2 and dquad[2] == 1 and dquad[3] == 0:
				dst["angle"] = -math.pi*0.5
				rotate = True				
			elif dquad[0] == 2 and dquad[1] == 1 and dquad[2] == 0 and dquad[3] == 3:
				dst["angle"] = math.pi
			elif dquad[0] == 1 and dquad[1] == 0 and dquad[2] == 3 and dquad[3] == 2:
				dst["angle"] = math.pi*0.5
				rotate = True
			elif dquad[0] == 0 and dquad[1] == 3 and dquad[2] == 2 and dquad[3] == 1:
				# origon
				rotate = False
			else:
				print "error: new type", dquad

			src_w = max(math.fabs(src_data[0]-src_data[2]), math.fabs(src_data[0]-src_data[6]))
			src_h = max(math.fabs(src_data[1]-src_data[3]), math.fabs(src_data[1]-src_data[7]))
			screen_w = max(math.fabs(screen_data[0]-screen_data[2]), math.fabs(screen_data[0]-screen_data[6])) / 16.0
			screen_h = max(math.fabs(screen_data[1]-screen_data[3]), math.fabs(screen_data[1]-screen_data[7])) / 16.0
			if rotate:
				dst["x scale"] = screen_h/src_w
				dst["y scale"] = screen_w/src_h
			else:
				dst["x scale"] = screen_w/src_w
				dst["y scale"] = screen_h/src_h				

			out["sprite"].append(dst)

	filepath = pic[:pic.find('_')]+'_complex.json'
	filepath = output_dir+'/'+filepath
	with open(filepath, 'w') as fp:
		json.dump(out, fp, sort_keys=True, indent = 2)


def decode_name(id):
	sid = str(id)
	if os.path.isfile(tmp_dir+'/'+sid+'_ani.tmp'):
		return sid+'_anim.json'
	else:
		return sid+'_complex.json'	

def decode_sprite(components, src):
	dst = {}
	dst["name"] = ''
	dst["x mirror"] = dst["y mirror"] = False
	dst["position"] = {}
	if type(src) == int:
		dst["filepath"] = decode_name(components[src])
		dst["position"]["x"] = dst["position"]["y"] = 0
		dst["angle"] = 0
		dst["x scale"] = dst["y scale"] = 1
		dst["x shear"] = dst["y shear"] = 0
	else:
		dst["filepath"] = decode_name(components[src["index"]])
		mat = src["mat"]
		dst["position"]["x"] = mat[4] / 16.0
		dst["position"]["y"] = -mat[5] / 16.0
		ang1 = math.atan(-mat[2]/mat[0])
		ang2 = math.atan(mat[1]/mat[3])
		if math.fabs(ang1-ang2) < 0.00001:
			if ang1 == 0:
				dst["x scale"] = mat[0]/1024.0/math.cos(ang1)
				dst["y scale"] = mat[3]/1024.0/math.cos(ang1)		
			else:
				dst["x scale"] = -mat[2]/1024.0/math.sin(ang1)
				dst["y scale"] = mat[1]/1024.0/math.sin(ang1)
			dst["angle"] = ang1
			dst["x shear"] = dst["y shear"] = 0				
		else:
			# no rotate
			dst["x scale"] = mat[0]/1024.0
			dst["y scale"] = mat[3]/1024.0
			dst["x shear"] = float(mat[2])/mat[3]
			dst["y shear"] = float(mat[1])/mat[0]
			dst["angle"] = 0
	return dst


def decode_ani2complex(src, dst):
	components = []
	for c in src["component"]:
		components.append(c["id"])

	dst["sprite"] = []
	for s in src["1"][0]:
		dst["sprite"].append(decode_sprite(components, s))


def decode_ani2animation(src, dst):
	components = []
	for c in src["component"]:
		components.append(c["id"])

	dst["fps"] = 30
	dst["layer"] = []
	layer = {}
	layer["frame"] = []
	time = 1
	for f in src["1"]:
		frame = {}
		frame["id"] = 0
		frame["time"] = time
		frame["tween"] = False
		frame["actor"] = []
		for s in f:
			frame["actor"].append(decode_sprite(components, s))
		layer["frame"].append(frame)
		time = time+1
	dst["layer"].append(layer)


def decode_ani(ani):
	json_file = open(tmp_dir+'/'+ani)
	json_data = json.load(json_file)
	json_file.close()

	out = {}
	out["name"] = json_data.get("export", "")
	filepath = output_dir+'/'
	if out["name"] == "":
		filepath += ani[:ani.find('_')]
	else:
		filepath += out["name"]
	if len(json_data["1"]) > 1:
		decode_ani2animation(json_data, out)
		filepath += '_anim.json'
	else:
		decode_ani2complex(json_data, out)
		filepath += '_complex.json'

	with open(filepath, 'w') as fp:
		json.dump(out, fp, sort_keys=True, indent = 2)	

# 1. extract mid files
os.system('lua extract.lua '+sys.argv[1]+' '+sys.argv[2]+' '+tmp_dir)

# 2. transform
for files in os.listdir(tmp_dir):
	if files.endswith('_pic.tmp'):
		decode_pic(files)
	if files.endswith('_ani.tmp'):
		decode_ani(files)
	if files.endswith('_cmp.tmp'):
		decode_ani(files)		




