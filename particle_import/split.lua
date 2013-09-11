-- 把excel导出的lua文件分解成编辑器可以读的格式
-- 存到./particles下面，所有paricle文件
-- filepath: excel导出的lua文件
-- data: 原始数据文件buildings.sc.lua

local filepath, data = ...

local pics = {}
local anis = {}
local tex = 0

local function picture(tbl)
	table.insert(pics, tbl)
end

local function animation(tbl)
	table.insert(anis, tbl)
end

local function texture(n)
	tex = n
end

f = assert(loadfile(data, "t", { picture = picture, animation = animation, texture = texture}))
f()

-- local function split(str, pat)
--    local t = {}  -- NOTE: use {n = 0} in Lua-5.0
--    local fpat = "(.-)" .. pat
--    local last_end = 1
--    local s, e, cap = str:find(fpat, 1)
--    while s do
--       if s ~= 1 or cap ~= "" then
-- 	 table.insert(t,cap)
--       end
--       last_end = e+1
--       s, e, cap = str:find(fpat, last_end)
--    end
--    if last_end <= #str then
--       cap = str:sub(last_end)
--       table.insert(t, cap)
--    end
--    return t
-- end

local function isanim(name)
	local anim = nil
	for _,v in pairs(anis) do
		if v.export == name then
			anim = v
			break
		end
	end
	if not anim then
		print("error", name)
	end
	assert(anim)
	local frames = anim[1]
	return #frames > 1
end

local l = 0.03
-- local k = 0.025
-- local g = 0.0001
local k = 0.25
local g = 0.001
-- local s = 0.002
local s = 0.01
local r = 1/360
local lf = 0.001

local f = assert(loadfile(filepath))
local t = f()

for _, v in pairs(t) do
	local f = io.open("particles/".._.."_particle.json", "w")

	f:write("{\n")
	f:write("\t\"name\" : \""..v.name.."\",\n")
	f:write("\t\"package\" : \""..v.package.."\",\n")
	f:write("\t\"count\" : "..v.count..",\n")
	f:write("\t\"layer\" : "..v.layer..",\n")
	f:write("\t\"emission_time\" : "..v.emission_time/l..",\n")
	f:write("\t\"min_life\" : "..v.min_life/l..",\n")
	f:write("\t\"max_life\" : "..v.max_life/l..",\n")
	f:write("\t\"min_hori\" : "..v.min_hori..",\n")
	f:write("\t\"max_hori\" : "..v.max_hori..",\n")
	f:write("\t\"min_vert\" : "..v.min_vert..",\n")
	f:write("\t\"max_vert\" : "..v.max_vert..",\n")
	f:write("\t\"min_spd\" : "..v.min_spd/k..",\n")
	f:write("\t\"max_spd\" : "..v.max_spd/k..",\n")
	f:write("\t\"gravity\" : "..v.gravity/g..",\n")
	f:write("\t\"inertia\" : "..v.inertia..",\n")
	f:write("\t\"fadeout_time\" : "..v.fadeout_time/l..",\n")
	if v.bounce then	
		f:write("\t\"bounce\" : true,\n")
	else
		f:write("\t\"bounce\" : false,\n")
	end
	if v.additive_blend then
		f:write("\t\"additive_blend\" : true,\n")
	else
		f:write("\t\"additive_blend\" : false,\n")
	end
	f:write("\t\"start_radius\" : "..v.start_radius..",\n")
	if v.orient_to_movement then
		f:write("\t\"orient_to_movement\" : true,\n")
	else
		f:write("\t\"orient_to_movement\" : false,\n")
	end
	if v.orient_to_parent then
		f:write("\t\"orient_to_parent\" : true,\n")
	else
		f:write("\t\"orient_to_parent\" : false,\n")
	end
	f:write("\t\"components\" : \n")
	f:write("\t[\n")
	for _,u in pairs(v.components) do
		f:write("\t\t{\n")
		if isanim(u.name) then
			f:write("\t\t\t\"filepath\" : \"../middle/"..u.name.."_anim.json\",\n")
		else
			f:write("\t\t\t\"filepath\" : \"../middle/"..u.name.."_complex.json\",\n")
		end
		f:write("\t\t\t\"name\" : \"\",\n")
		f:write("\t\t\t\"start_scale\" : "..u.start_scale/s..",\n")
		f:write("\t\t\t\"end_scale\" : "..u.end_scale/s..",\n")
		f:write("\t\t\t\"min_rotate\" : "..u.min_rotate/r..",\n")
		f:write("\t\t\t\"max_rotate\" : "..u.max_rotate/r..",\n")
		f:write("\t\t\t\"start_z\" : "..u.start_z.."\n")
		if _ == #v.components then
			f:write("\t\t}\n")
		else
			f:write("\t\t},\n")
		end
	end
	f:write("\t]\n")

	f:write("}\n")

--	print(v.layer)
	f:close()

end
