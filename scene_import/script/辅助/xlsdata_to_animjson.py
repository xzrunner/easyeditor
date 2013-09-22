# -*- coding: utf-8 -*-

tpl_main = '''
{
	"fps" : 30,
	"layer" : 
	[
		
		{
			"frame" : 
			[
%(frames)s
			],
			"name" : "Layer1"
		}
	],
	"name" : "%(name)s"
}
'''

tpl_frame = r'''
				{
					"actor" : 
					[
						
						{
							"angle" : 0.0,
							"filepath" : "..\\..\\pngs\\%(png_name)s.png",
							"name" : "",
							"position" : 
							{
								"x" : 0.0,
								"y" : 0.0
							},
							"x mirror" : %(x_mirror)s,
							"x scale" : 1.0,
							"x shear" : 0.0,
							"y mirror" : false,
							"y scale" : 1.0,
							"y shear" : 0.0
						}
					],
					"id" : 0,
					"time" : %(time)d,
					"tween" : false
				}'''

FRAME_PER_IMAGE = 45
NAME = 'turret_1006_lv7'
EXPORT = False
DATA = """
1006_3_lv7_01	1	
1006_3_lv7_02	1	
1006_3_lv7_03	1	
1006_3_lv7_04	1	
1006_3_lv7_05	1	
1006_3_lv7_06	1	
1006_3_lv7_07	1	
1006_3_lv7_08	1	
"""

i = 0
last_data = None
data = []
for line in DATA.splitlines():
    if not line:
        continue

    png_name, _, x_mirror = line.split('\t')
    x_mirror = (x_mirror == '1') and "true" or "false"
    time = i * FRAME_PER_IMAGE + 1

    entry_data = dict(png_name=png_name, x_mirror=x_mirror, time=time)

    entry = tpl_frame % entry_data
    last_data = entry_data
    data.append(entry)

    i += 1
last_data["time"] = i * FRAME_PER_IMAGE
last = tpl_frame % last_data
data.append(last)
frames = ','.join(data)

name = ''
if EXPORT: name = NAME
output = tpl_main % vars()

out_file = NAME + '_anim.json'
open(out_file, 'wb').write(output)
