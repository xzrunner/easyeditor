
local _sf = string.format

local detail = {
  ["daoke"]     = {name = '6163-tongyong-1.PNG',  x=0, y=-6 },
  ["shenshe"]   = {name = '6164-tongyong-1.PNG',  x=0, y=0 },
  ["mojin"]     = {name = '6165-tongyong-1.PNG',  x=0, y=-3 },
  ["jiabing"]   = {name = '6166-tongyong-1.PNG',  x=0, y=0 },
  ["sishi"]     = {name = '6167-tongyong-1.PNG',  x=0, y=-8 },
  ["tianzai"]   = {name = '6168-tongyong-1.PNG',  x=0, y=0 },
  ["daoshi"]    = {name = '6169-tongyong-1.PNG',  x=0, y=-0.5 },
  ["shennv"]    = {name = '6170-tongyong-1.PNG',  x=0, y=-6 },
  ["dapeng"]    = {name = '6171-tongyong-1.PNG',  x=4, y=6 },
  ["xueying"]   = {name = '6196-tongyong-1.PNG',  x=0, y=0 },
  ["light"]     = {name = "6290-tongyong-1.PNG",  x=0, y=0 },
  ["healing"]   = {name = "6291-tongyong-1.PNG",  x=0, y=0 },
  ["rage"]      = {name = "6292-tongyong-1.PNG",  x=0, y=0 },
  ["jump"]      = {name = "6293-tongyong-1.PNG",  x=0, y=0 },
  ["langqi"]    = {name = "army_langqi_main.png", x=-1, y=-16 },
  ["fujia"]     = {name = "army_fujia_main.png",  x=0, y=-6 },
  ["wuzhu"]     = {name = "army_wuzhu_main.png",  x=0, y=0 },
}

local show_icon_cfg = {
  {tag = 'daoke'},
  {tag = 'shenshe'},
  {tag = 'mojin'},
  {tag = 'jiabing'},
  {tag = 'sishi'},
  {tag = 'tianzai'},
  {tag = 'daoshi'},
  {tag = 'shennv'},
  {tag = 'dapeng'},
  {tag = 'xueying'},
  {tag = 'light'},
  {tag = 'healing'},
  {tag = 'rage'},
  {tag = 'jump'},
  {tag = 'langqi'},
  {tag = 'fujia'},
  {tag = 'wuzhu'},
}

local troop_cfg = {
  {level = 6, tag = 'daoke'},
  {level = 6, tag = 'shenshe'},
  {level = 6, tag = 'mojin'},
  {level = 6, tag = 'jiabing'},
  {level = 6, tag = 'sishi'},
  {level = 6, tag = 'tianzai'},
  {level = 6, tag = 'daoshi'},
  {level = 3, tag = 'shennv'},
  {level = 6, tag = 'dapeng'},
  {level = 5, tag = 'xueying'},
  {level = 6, tag = 'langqi'},
  {level = 6, tag = 'fujia'},
  {level = 6, tag = 'wuzhu'},
}


local troop_spell_cfg = {
  {level = 6, tag = 'daoke'},
  {level = 6, tag = 'shenshe'},
  {level = 6, tag = 'mojin'},
  {level = 6, tag = 'jiabing'},
  {level = 6, tag = 'sishi'},
  {level = 6, tag = 'tianzai'},
  {level = 6, tag = 'daoshi'},
  {level = 3, tag = 'shennv'},
  {level = 6, tag = 'dapeng'},
  {level = 5, tag = 'xueying'},
  {level = 5, tag = 'light'},
  {level = 5, tag = 'healing'},
  {level = 5, tag = 'rage'},
  {level = 2, tag = 'jump'},
  {level = 6, tag = 'langqi'},
  {level = 6, tag = 'fujia'},
  {level = 6, tag = 'wuzhu'},
}

local spell_cfg = {
  {level = 5, tag = 'light'},
  {level = 5, tag = 'healing'},
  {level = 5, tag = 'rage'},
  {level = 2, tag = 'jump'},
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
  -70, -97,
  -42, -97,
  -14, -97,
   14, -97,
   42, -97,
   70, -97,
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

    local tag = info.tag
    local v = detail[tag]
    info.name = v.name
    info.x = v.x
    info.y = v.y

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
_gen_json(troop_cfg, "../sg_ui/troop_icon_anim.json")

-- gen troop_spell
_gen_json(troop_spell_cfg, "../sg_ui/troop_spell_anim.json")

-- gen spell
_gen_json(spell_cfg, "../sg_ui/spell_icon_anim.json")

-- gen show icon
_gen_json(show_icon_cfg, "../sg_ui/troop_spell_show_icon_anim.json")

