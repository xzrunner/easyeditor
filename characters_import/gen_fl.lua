local lfs = require 'lfs'
local md5 = require 'gen_md5'
local gf =  require 'gen_frame'
local _sf = string.format

local shadow = 'shadow.tga'
-- local export_path = './export_characters/tc'
local export_path = arg[1]

local function md5_file(file_name)
	_md5 = md5[file_name]
	return _md5
end

local function _dump_png_file(path)
	local ret = {}
	local function _dump(p)
		for file in lfs.dir(p) do
			if file ~= '.' and file ~='..' then
				local f = p..'/'..file
				local attr = lfs.attributes(f)
				if attr.mode == 'directory' then
					_dump(f)
				else
					local kind = string.sub(f, -4, -1)
					if kind == '.png' then
						table.insert(ret, f)
					end
				end
			end
		end
	end
	_dump(path)
	return ret
end

local function _str2step_frame(str)
	local step = string.byte(str) - 48
	local a, b, c = string.byte(str, -1)- 48, string.byte(str, -2) - 48, string.byte(str, -3)-48
	local frame = c*100 + b*10 + a
	return step, frame
end


local function _gen_md5_map(file_list)
	local md5_full_map, md5_fall_map, md5_re_map, ani_map = {}, {}, {}, {}
	assert(type(file_list)=='table')
	for i,file_name in ipairs(file_list) do
		local _md5 = md5_file(file_name)
		local _ani = string.sub(file_name, string.len(export_path)+2, -11)
		_ani = string.gsub(_ani, '/', '_')
		local step, frame = _str2step_frame(string.sub(file_name, -9, -5))
		ani_map[_ani] = ani_map[_ani] or {}
		ani_map[_ani]._str_component = ani_map[_ani]._str_component or {}
		ani_map[_ani].str_component = ani_map[_ani].str_component or {}
		ani_map[_ani].frame = ani_map[_ani].frame or {}
		ani_map[_ani].frame[frame] = ani_map[_ani].frame[frame] or {}
		ani_map[_ani]._frame = ani_map[_ani]._frame or {}
		ani_map[_ani]._frame[frame] = ani_map[_ani]._frame[frame] or {}

		md5_full_map[file_name] = _md5
		if not md5_fall_map[_md5] then
			md5_fall_map[_md5] = file_name
		else
			table.insert(md5_re_map, {md5_fall_map[_md5], file_name})
		end

		local _id = frame*10+step
		table.insert(ani_map[_ani]._str_component, {id=_id, file=file_name})
		ani_map[_ani]._frame[frame][step] = _id
	end

	local function _id2idx(ani, id)
		for i,v in ipairs(ani._str_component) do
			if v.id == id then
				return i-1
			end
		end
		error('not match!')
	end

	for k,ani in pairs(ani_map) do
		table.sort( ani._str_component, function (a, b)
				return a.id < b.id 
			end)

		for i=1,#ani._frame do
			for j,id in ipairs(ani._frame[i]) do
				ani._frame[i][j] = _id2idx(ani, id)
			end
			local  shadow_scale = nil
			if gf[k] then shadow_scale = gf[k].shadow_scale end
			if not shadow_scale then
				table.insert(ani._frame[i], 1, #ani._str_component)
			else
				table.insert(ani._frame[i], 1, {index = #ani._str_component, 
					mat ={1024*shadow_scale, 0, 0, 1024*shadow_scale, 0, 0}})
			end
		end

		for i=1,#ani._str_component do
			local _f = md5_fall_map[md5_full_map[ani._str_component[i].file]]
			ani.str_component[i] = string.sub(_f, string.len(export_path)+2, -5)
		end
		ani.str_component[#ani._str_component+1] = shadow

		local _count = 1
		for i=1,#ani._frame do
			local m = 2
			if gf[k] then m = gf[k][i] or 2 end
			for j=1,m do
				ani.frame[_count] = ani._frame[i]
				_count = _count + 1
			end
		end

		ani._str_component = nil
		ani._frame = nil
	end

	return md5_full_map, md5_fall_map, md5_re_map, ani_map
end


local function _write_frame(fd, v)
	if type(v)=='table' then
		fd:write(_sf('{index = %d, ', v.index))
		if v.color then fd:write(_sf('color = %d, ', v.color)) end
		if v.mat then fd:write(_sf('mat = {%d, %d, %d, %d, %d, %d}}, ', v.mat[1], v.mat[2], v.mat[3], v.mat[4], v.mat[5], v.mat[6])) end
	elseif type(v) == 'number' then
		fd:write(_sf('%d, ', v))
	else
		error('the type is error.')
	end
end


-- gen map
local fm = _dump_png_file(export_path)
local full, fall, re, ani = _gen_md5_map(fm)

-- gen animation lua file
local fd = io.open(export_path..'/animation.lua', 'w')
fd:write('local animation = {\n')
for k,_ani in pairs(ani) do
	fd:write(_sf('\t["%s"]={\n', k))
	for k,com in pairs(_ani) do
		fd:write(_sf('\t\t%s={\n', k))
		if k == 'str_component' then
			for i=1,#com do
				fd:write(_sf('\t\t\t"%s",\n', com[i]))
			end
		elseif k =='frame' then
			for i=1,#com do
				fd:write('\t\t\t{')
				for j=1,#com[i] do
					--fd:write(_sf('%d, ', com[i][j]))
					_write_frame(fd, com[i][j])
				end
				fd:write('},\n')
			end
		end
		fd:write('\t\t},\n')
	end
	fd:write('\t},\n\n')
end
fd:write('}\n')
fd:write('return animation\n')
fd:close()

-- del repeat pic
for i,v in ipairs(re) do
	local cmd = 'rm -rf '..v[2]
	print(cmd)
	os.execute(cmd)
end




