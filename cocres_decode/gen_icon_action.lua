

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
}

local head = [[
{
	"name" : "all_role",
	"sprite" : 
	[
%s
	],
	"xmax" : 0.0,
	"xmin" : 0.0,
	"ymax" : 0.0,
	"ymin" : 0.0
}
]]


local body = [[
		{
			"add color" : "0x00000000",
			"angle" : 0.0,
			"clip" : false,
			"filepath" : "%s",
			"multi color" : "0xffffffff",
			"name" : "",
			"position" : 
			{
				"x" : %d,
				"y" : %d
			},
			"tag" : "%s",
			"x mirror" : false,
			"x offset" : 0.0,
			"x scale" : 1.0,
			"x shear" : 0.0,
			"y mirror" : false,
			"y offset" : 0.0,
			"y scale" : 1.0,
			"y shear" : 0.0
		}

]]

local ex_start_format = [[
		
		{

			"add color" : "0x00000000",
			"angle" : 0.0,
			"clip" : false,
			"filepath" : "6101-tongyong-1.png",
			"multi color" : "0xffffffff",
			"name" : "",
			"position" : 
			{
				"x" : %d,
				"y" : %d
			},
			"tag" : "%s",
			"x mirror" : false,
			"x offset" : 0.0,
			"x scale" : 0.3,
			"x shear" : 0.0,
			"y mirror" : false,
			"y offset" : 0.0,
			"y scale" : 0.3,
			"y shear" : 0.0
		}]]



local start_point = {
  {-70, -97},
  {-42, -97},
  {-14, -97},
  {14, -97},
  {42, -97},
  {70, -97},
}


local function _gens(count, begin)
	local ret = {}
	for k,v in pairs(detail) do
		for i=begin, count do
			ret[#ret+1] = k..(i)
		end
	end
	return table.concat(ret, ";")
end

local function _genr(role)
	local ret = {}
	local k, v = role, detail[role]
	for i=0, #start_point do
		ret[#ret+1] = k..(i)
	end
	return table.concat(ret, ";")
end

local function gen_start()
	local ret = {}
	for i=1, #start_point do
		local x = start_point[i][1]
		local y = start_point[i][2]
		ret[#ret+1] = ex_start_format:format(x, y, _gens(#start_point, i))
	end
	return table.concat(ret, ",")
end

local function gen_role()
	local ret = {}
	for k,v in pairs(detail) do
		local path = v.name
		local x = v.x
		local y = v.y
		ret[#ret+1] = body:format(path, x, y, _genr(k))
	end
	return table.concat(ret, ",")
end



local function gen_file(file)
	local roles = gen_role()
	local start = gen_start()

	local _body = roles..","..start
	local ret = head:format(_body)

	local fd = io.open(file, "w")
	fd:write(ret)
	fd:close()
end

--- gen
gen_file("../sg_ui/all_role_complex.json")
print("gen_file  success!")







