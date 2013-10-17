
local _sf = string.format

local show_icon_cfg = {
	{name = '6163-tongyong-1.PNG', x = -20, y = 92}, -- daoke
	{name = '6164-tongyong-1.PNG', x = -24, y = 106}, -- shenshe
	{name = '6165-tongyong-1.PNG', x = -45, y = 148}, -- mojin
	{name = '6166-tongyong-1.PNG', x = -41, y = 43}, -- jiabing
	{name = '6167-tongyong-1.PNG', x = -34, y = 162}, -- sishi
	{name = '6168-tongyong-1.PNG', x = -1, y = 88}, -- tianzai
	{name = '6169-tongyong-1.PNG', x = -16, y = -31}, -- daoshi
	{name = '6170-tongyong-1.PNG', x = -21, y = 35}, -- shennv
	{name = '6171-tongyong-1.PNG', x = 50, y = 69}, -- dapeng
	{name = '6196-tongyong-1.PNG', x = -9, y = -48}, -- fixme xueying
	{name = "6290-tongyong-1.PNG", x = 19, y = 38}, -- light
	{name = "6291-tongyong-1.PNG", x = 19, y = 38}, -- healing
	{name = "6292-tongyong-1.PNG", x = 19, y = 38}, -- rage
	{name = "6293-tongyong-1.PNG", x = 19, y = 38}, -- jump
}

local troop_cfg = {
	{name = '6163-tongyong-1.PNG', x = -20, y = 92, level = 6}, -- daoke
	{name = '6164-tongyong-1.PNG', x = -24, y = 106, level = 6}, -- shenshe
	{name = '6165-tongyong-1.PNG', x = -45, y = 148, level = 5}, -- mojin

	{name = '6163-tongyong-1.PNG', x = -20, y = 92, level = 1}, -- fill nil
	
	{name = '6166-tongyong-1.PNG', x = -41, y = 43, level = 6}, -- jiabing
	{name = '6167-tongyong-1.PNG', x = -34, y = 162, level = 5}, -- sishi

	{name = '6163-tongyong-1.PNG', x = -20, y = 92, level = 1}, -- fill nil
	
	{name = '6168-tongyong-1.PNG', x = -1, y = 88, level = 6}, -- tianzai
	{name = '6169-tongyong-1.PNG', x = -16, y = -31, level = 5}, -- daoshi

	{name = '6163-tongyong-1.PNG', x = -20, y = 92, level = 1}, -- fill nil

	{name = '6170-tongyong-1.PNG', x = -21, y = 35, level = 3}, -- shennv
	{name = '6171-tongyong-1.PNG', x = 50, y = 69, level = 3}, -- dapeng

	{name = '6163-tongyong-1.PNG', x = -20, y = 92, level = 3}, -- fill nil

	{name = '6196-tongyong-1.PNG', x = -9, y = -48, level = 5}, -- fixme xueying
}


local spell_cfg = {
	{name = "6290-tongyong-1.PNG", x = 19, y = 38, level = 5 }, -- light
	{name = "6291-tongyong-1.PNG", x = 19, y = 38, level = 5}, -- healing
	{name = "6292-tongyong-1.PNG", x = 19, y = 38, level = 5}, -- rage
	{name = "6293-tongyong-1.PNG", x = 19, y = 38, level = 2}, -- jump
}


local hero_cfg = {
	
}

local ex_body_format = [[
{
    "fps" : 30,
	"layer" : 
	[
		
		{
			"frame" : 
			[
%s
			],
			"name" : "Layer0"
		}
	],
	"name" : ""
}
]]

local _time_count = 1
local ex_frame_format = [[
				{
					"actor" :
					[
%s
					],
					"id" : 0,
					"time" : %d,
					"tween" : false
				}]]


local ex_actor_format = [[
						{
							"angle" : 0.0,
							"filepath" : "%s",
							"name" : "",
							"position" :
							{
								"x" : %f,
								"y" : %f
							},
							"x mirror" : false,
							"x scale" : 1.0,
							"x shear" : 0.0,
							"y mirror" : false,
							"y scale" : 1.0,
							"y shear" : 0.0
						}]]

local ex_start_format = [[
						{
							"angle": 0.0,
							"filepath": "6101-tongyong-1.png",
							"name": "",
							"position":
							{
								"x": %f,
								"y": %f
							},
							"x mirror" : false,
							"x scale" : 0.3,
							"x shear" : 0.0,
							"y mirror" : false,
							"y scale" : 0.3,
							"y shear" : 0.0
						}]]

local start_point = {
	-55.0, -68.0,
	-26, -68,
	2, -68,
	30.5, -68,
	55.250, -68,
	83.750, -68
}

local function _gen_start_actor(num)
	local ret = ""
	for i=1,num do
		ret = ret.._sf(ex_start_format, start_point[2*i-1], start_point[2*i])
        if i~= num then
            ret = ret..",\n"
        end
	end

	return ret
end

local function _gen_char_actor(file, x, y, level)
	local ret = ""
	ret = _sf(ex_actor_format, file, x, y)
	if level then
        ret = ret..",\n"
		ret = ret.._gen_start_actor(level)
	end
	return ret
end


local function _gen_frame(info, level)
	local ret = ""
	ret = _sf(ex_frame_format, 
		_gen_char_actor(info.name, info.x, info.y, level),
		_time_count
		)
	_time_count = _time_count + 1
	return ret
end


local function _gen(cfg)
	local ret = ""
	for i=1,#cfg do
		local info = cfg[i]
		if info.level then
			for i=1,info.level do
				ret = ret.._gen_frame(info, i) -- gen start icon
	            if i~= info.level then
	                ret = ret..",\n"
	            end
			end
		else
			ret = ret.._gen_frame(info)
		end
        if i ~= #cfg then
            ret = ret..",\n"
        end
	end
	return ret
end

local function _gen_json(cfg, file)
	_time_count = 1
	local ret = _sf(ex_body_format, _gen(cfg))
	local fd = io.open(file, "w")
	fd:write(ret)
	fd:close()	
end


-- gen troop
-- _gen_json(troop_cfg, "../sg_ui/troop_icon_anim.json")

-- gen spell
-- _gen_json(spell_cfg, "../sg_ui/spell_icon_anim.json")


-- gen show icon
 _gen_json(show_icon_cfg, "../sg_ui/troop_spell_show_icon_anim.json")








