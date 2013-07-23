local fd = io.open(arg[1]..'.lua', 'w')
local multiple = 6 
local gf = require "gen_frame"


local function write_pic(fd, ti,  i, v)
    local _sourceX, _sourceY, _sourceWidth, _sourceHeight = v.sourceX*multiple, v.sourceY*multiple, v.sourceWidth*multiple, v.sourceHeight*multiple
    local _width, _height = v.width*multiple , v.height*multiple

	fd:write('\n\npicture {\n')
	local v1  = {-(_sourceWidth/2)+_sourceX+_width, -(_sourceHeight/2)+_sourceY}
	local v2 = {v1[1], v1[2]+_height}
	local v3 = {-(_sourceWidth/2)+_sourceX, v2[2]}
	local v4 = {v3[1], v1[2]}

	fd:write(string.format('\tid = %d,\n\t{tex=%d, src={%d,%d, %d,%d, %d,%d, %d,%d}, screen={%d,%d, %d,%d, %d,%d, %d,%d}}\n', 
		i, -- index
		ti, -- tex idx
		v.x+v.width,v.y, v.x+v.width,v.y+v.height, v.x,v.y+v.height, v.x,v.y,  -- src
		v1[1],v1[2], v2[1],v2[2], v3[1],v3[2], v4[1],v4[2] )
	)
	fd:write('}\n')
end

local function write_animation(fd, i, k, t)
	fd:write('\n\n\nanimation {\n')
	fd:write(string.format('\texport = "%s",\n\tid= %d,\n', k, i))

	-- write component
	fd:write('\tcomponent = {\n')
	for i=1 ,#t.component do
		fd:write(string.format('\t\t{id = %d},\n', t.component[i]))
	end
	fd:write('\t},\n')

	-- write frame
	fd:write('\t{\n')
	for i=1 , #t.frames do
		local m = 2
		if gf[k] then
			m = gf[k][i]
		end

		for n=1,m do
			fd:write('\t\t{')
			for j=1,#t.frames[i] do
				fd:write(string.format('%d, ', t.frames[i][j]))
			end
			fd:write('},\n')
		end
	end
	fd:write('\t}\n')


	fd:write('}\n')
end

local function gen_key( str )
	local  s = string.sub(str, 1, -7)
	local _f = string.sub(str, -5, -1)
	local _s, n =  string.gsub(s, '/', '_')

	local frame_step = string.byte(_f) - 48 
	local a, b, c = string.byte(_f, -1)- 48, string.byte(_f, -2) - 48, string.byte(_f, -3)-48
	local frame_idx = c*100 + b*10 + a
	return _s,  frame_idx, frame_step
end

local function _check_id(t, fis)
	for i,v in ipairs(t) do
		if v.idx == fis then
			return i-1
		end
	end

	error('not match!')
end

local function generate_animation_table(frameidx_map, begin_id)
	local t = {}
	for k,v in pairs(frameidx_map) do
		local _key, _fi, _fs = gen_key(k)
		t[_key] = t[_key] or {}
		t[_key]._component = t[_key]._component or {}
		t[_key].frames = t[_key].frames or {}
		t[_key].frames[_fi] = t[_key].frames[_fi] or {}
		table.insert(t[_key]._component, {idx  =  _fi*10+_fs, v_id = begin_id+v-1})
		table.sort(t[_key]._component, function (a, b)
			return a.idx < b.idx
			end)
	end

	for k,v in pairs(frameidx_map) do
		local _key, _fi, _fs = gen_key(k)
		t[_key].component = t[_key].component or {}
		for i=1,#t[_key]._component do
			t[_key].component[i] = t[_key]._component[i].v_id
		end

		local _fis  = _fi*10+_fs
		t[_key].frames[_fi][_fs] = _check_id(t[_key]._component, _fis)
	end

	return t
end


local function get_texture_number()
	local ret = 1
	local file = arg[1]..'1.lua'
	local  _fd=io.open(file)
	if not _fd then
		error('not find '..file)
	end

	_fd:close()
	while true do
	 	_fd=io.open(file) 
		if not _fd then
			break
		end
		_fd:close()
		ret = ret+1
		file = arg[1]..ret..'.lua'
	end

	return ret-1 
end

local function gen_texture_list()
	local sheetInfo_list = {}
	local texture_number = get_texture_number()
	local count = 0
	for i=1,texture_number do
		local st = require(arg[1]..i)
		st.name = arg[1]..i..'.lua'
		st.pic_begin_id = count
		sheetInfo_list[i] = st
		count = count + #(st.sheet.frames)
	end
	return sheetInfo_list
end

local function export(fd, sheetInfo_list)
	local id = 0
	fd:write('texture('..#sheetInfo_list..')\n\n')

	for i,v in ipairs(sheetInfo_list) do
		fd:write('\n-----pic------\n')
		print('write pic from: '..v.name)
		
		-- write picture
		for ip,vp in ipairs(v.sheet.frames) do
			write_pic(fd, i-1, v.pic_begin_id+ip-1, vp)
			id = v.pic_begin_id+ip
		end
	end

	for i,v in ipairs(sheetInfo_list) do
		print('write animation from:'..v.name)
		fd:write('\n-----anim------\n')
		-- generate animation table
		local animation_table = generate_animation_table(v.frameIndex, v.pic_begin_id)
	--	gf.print_lua_table(animation_table)
		-- write animation
		for k,v in pairs(animation_table) do
			write_animation(fd, id, k, v)
			id = id+1
		end
	end
end

-- generate lua
local sheetInfo_list = gen_texture_list()

-- export
export(fd, sheetInfo_list)
fd:close()
