import os
import math
import sys

SRC_POS = sys.argv[1]
TEX = int(sys.argv[2])
FILE_NAME = sys.argv[3]
OUTPUT = sys.argv[4]
INDEX = int(sys.argv[5])
START_ID = int(sys.argv[6])

# SRC_POS = '46,562, 82,562, 82,526, 46,526'
# OUTPUT = 'pentagon.lua'
EDGE = 250*0.6
# INDEX = 19
# START_ID = 0
# TEX = 0

def rotate(src, rad, dst):
	dst[0] = src[0] * math.cos(rad) - src[1] * math.sin(rad)
	dst[1] = src[0] * math.sin(rad) + src[1] * math.cos(rad)


def gen_vertex(edge, data, vertex):
	unit = math.pi*2/5
	angle = math.pi*0.5
	src = [edge, 0]
	i = 0
	while i < 5:
		dst = [0, 0]
		rotate(src, angle, dst)
		vertex[i*2] = dst[0]*float(data[i])
		vertex[i*2+1] = -dst[1]*float(data[i])
		angle += unit
		i += 1


def write2file(files, vertex):
	global START_ID

	files.write('picture {\n')
	files.write('\tid = '+str(START_ID)+',\n')
	START_ID += 1

	i = 0
	while i < 5:
		files.write('\t{ tex = %d, src = { %s }, screen = { ' % (TEX, SRC_POS))
		files.write('%d, %d, %d, %d, %d, %d, %d, %d' % (vertex[i*2]*16, vertex[i*2+1]*16, vertex[((i+1)%5)*2]*16, vertex[((i+1)%5)*2+1]*16, 0, 0, 0, 0))
		files.write(" } },\n")
		i += 1

	files.write('}\n')


def write_anim(files, name, start_id, size):
	global START_ID

	files.write('animation {\n')
	files.write('\texport = \"'+name+'\",\n')
	files.write('\tid = '+str(START_ID)+',\n')
	START_ID += 1
	#  components
	files.write('\tcomponent = {\n')
	i = 0
	while i < size:
		files.write('\t\t{ id = '+str(start_id+i)+', },\n')
		i += 1
	files.write('\t},\n')
	# frames
	files.write('\t{\n')
	i = 0
	while i < size:
		files.write('\t\t{\n')
		files.write('\t\t\t{index = '+str(i)+', mat = {1024, 0, 0, 1024, 0, 0}},\n')
		files.write('\t\t},\n')
		i += 1
	files.write('\t},\n')
	files.write('}\n')


def gen_pentagon():
	files = open(OUTPUT, 'w')

	skip = 0
	name = ''
	start_id = 0
	for line in open(FILE_NAME):
		# skip 2 lines
		skip += 1
		if skip <= 2:
			continue

		# remove '\n'
		items = line[:-1].split(',')

		# print anim
		if items[0] != name and items[0] != '':
			if name != '':
				write_anim(files, name, start_id, START_ID-start_id)			
			name = items[0]
			start_id = START_ID	

		# print picture
		data = [0]*5
		i = 0
		while i < 5:
			data[i] = items[INDEX+i]
			i += 1
		vertex = [0]*10
		gen_vertex(EDGE, data, vertex)
		write2file(files, vertex)

	write_anim(files, name, start_id, START_ID-start_id)

gen_pentagon()