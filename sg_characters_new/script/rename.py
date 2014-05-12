import os

def processDir(args, dirname, filenames):
	for filename in filenames:
		if filename.find('.')>0:
			# print filename
			# newname=file+
			newname=filename.replace("-", "_")
			# print newname
			os.rename(os.path.join(dirname,filename),os.path.join(dirname,newname))

os.path.walk(r'2010shennv', processDir, None )