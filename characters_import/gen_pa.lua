local animation = require 'animation'
local fd = io.open(arg[1]..'.lua', 'w')
local multiple = 10 
local _sf = string.format

local function _str2id(sheetInfo_list, str)
	for i,sheetInfo in ipairs(sheetInfo_list) do
		local _id= sheetInfo.frameIndex[str]
		if _id then return sheetInfo.pic_begin_id+_id-1 end
	end
	error(_sf('not match "%s" id.', str))
end

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

local function write_frame(fd, v)
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


local function write_animation(sheetInfo_list, fd, ani_name, ani_table, id)
	fd:write('\n\n\nanimation {\n')
	fd:write(_sf('\texport = "%s",\n', ani_name))
	fd:write(_sf('\tid = %d,\n', id))

	fd:write('\tcomponent = {\n')
	for i=1,#ani_table.str_component do
		fd:write(_sf('\t\t{ id = %d},\n', _str2id(sheetInfo_list, ani_table.str_component[i])))
	end
	fd:write('\t},\n')

	fd:write('\t{\n')
	for i=1,#ani_table.frame do
		fd:write('\t\t{')
		for j=1,#ani_table.frame[i] do
			write_frame(fd, ani_table.frame[i][j])
			-- fd:write(_sf(' %d,', ani_table.frame[i][j]))
		end
		fd:write('},\n')
	end
	fd:write('\t},\n')

	fd:write('}')
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

local function export(sheetInfo_list)
	local id = 0
	fd:write('texture('..#sheetInfo_list..')\n\n')

	for i,v in ipairs(sheetInfo_list) do
		fd:write('\n-----pic------\n')
		print('write pic from: '..v.name)

		--- write picture
		for ip,vp in ipairs(v.sheet.frames) do
			write_pic(fd, i-1, v.pic_begin_id+ip-1, vp)
			id = v.pic_begin_id+ip
		end
	end

	print('write animation\n')
	for k,v in pairs(animation) do
		write_animation(sheetInfo_list, fd, k, v, id)
		id = id + 1
	end
end


local sheetInfo_list = gen_texture_list()
export(sheetInfo_list)
fd:close()


