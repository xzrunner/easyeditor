local bit32 = require "bit32"
local ppm = require "ppm"
local png = require "png"
local epconv = require "epconv"
local lzma = require "lzma"

local model, filename, compress = ...
local max_id = 0
local export = 0

local pic = {}
local ani = {}
local tex = 0

local memfile = { result = {} }

function memfile:write(str)
	table.insert(self.result, str)
end

local function picture(tbl)
	table.insert(pic, tbl)
end

local function animation(tbl)
	table.insert(ani, tbl)
end

local function texture(n)
	tex = n
end

f = assert(loadfile (filename , "t", { picture = picture , animation = animation , texture = texture }))
f()

local ANIMATION = 0
local PICTURE = 1
local CLIPUI = 2

local TEXTURE4 = 0
local TEXTURE8 = 1
local DATA = 2


local COMPONENT = 0
local SWITCH = 1
local LABEL = 2

local FID = 0
local FCOLOR = 1
local FMAT = 2
local FCLIP = 4

local function wstring(f,s)
	if s == nil then
		f:write(string.char(255))
	else
		assert (#s < 255)
		f:write(string.char(#s))
		f:write(s)
	end
end

local function wchar(f,c)
	f:write(string.char(c))
end

local function wshort(f,c)
	assert(c> -0x8000 and c < 0x8000)
	f:write(string.char(bit32.extract(c,0,8),bit32.extract(c,8,8)))
end

local function wlong(f,c)
	f:write(string.char(bit32.extract(c,0,8),bit32.extract(c,8,8),bit32.extract(c,16,8),bit32.extract(c,24,8)))
end

local function wpicture(f, t)
	wchar(f,PICTURE)
	wshort(f,t.id)
	wshort(f,#t)
	for _,v in ipairs(t) do
		wchar(f,v.tex)
		for _,v in ipairs(v.src) do
			wshort(f,v)
		end
		for _,v in ipairs(v.screen) do
			wlong(f,v)
		end
	end
end

local function wmat(f,mat)
	for _,v in ipairs(mat) do
		wlong(f, v)
	end
end

local function waction(f,t)
	wstring(f, t.action)
	wshort(f, #t)
	for _,frame in ipairs(t) do
		wshort(f,#frame)
		for _,v in ipairs(frame) do
			if type(v) == "number" then
				wchar(f, FID)
				wshort(f, v)
			else
				local type = FID
				if v.clip then
					type = type + FCLIP
				end
				if v.color then
					type = type + FCOLOR
				end
				if v.mat then
					type = type + FMAT
				end
				wchar(f, type)
				wshort(f, v.index)
				if v.color then
					wlong(f, v.color)
					wlong(f, v.add)
				end
				if v.mat then
					wmat(f, v.mat)
				end
			end
		end
	end
end

local function wlabel(f,v)
	wstring(f,v.name)
	wstring(f,v.font)
	wlong(f, v.color)
	wchar(f, v.size)
	wchar(f, v.align)
	wshort(f, v.width)
	wshort(f, v.height)
end

local function wanimation(f,t)
	if #t.component == 0 then
		print(t.export , t.id, "is empty")
	end
	if t.clipbox then
		wchar(f, CLIPUI)
		wshort(f,t.id)
		wlong(f, t.clipbox[1])
		wlong(f, t.clipbox[2])
		wlong(f, t.clipbox[3])
		wlong(f, t.clipbox[4])
	else
		wchar(f, ANIMATION)
		wshort(f,t.id)
	end
	wstring(f,t.export)

	-- component
	wshort(f,#t.component)
	for _,v in ipairs(t.component) do
		if v.id == nil then
			wchar(f,LABEL)
			wlabel(f,v)
		elseif v.name then
			wchar(f,SWITCH)
			wshort(f, v.id)
			wstring(f, v.name)
		else
			wchar(f, COMPONENT)
			wshort(f, v.id)
		end
	end

	-- action
	wshort(f,#t)
	for _,v in ipairs(t) do
		waction(f,v)
	end
end

local function check_id(v)
	if v.id > max_id then
		max_id = v.id
	end
	if v.export then
		export = export + 1
	end
end

--- detail

local function detail()
	memfile.result = {}
	local f = memfile
	for _,v in pairs(pic) do
		check_id(v)
		wpicture(f, v)
	end
	for _,v in pairs(ani) do
		check_id(v)
		wanimation(f,v);
	end

	wshort(f,max_id)
	wshort(f,export)

	local data = table.concat(f.result)
	local sz = epconv(data)
	f.result = {}
	wchar(f,DATA)
	wlong(f,sz)
	table.insert(f.result, data)
	return table.concat(f.result)
end

local function _load(filename, func)
	memfile.result = {}
	local w,h,depth,data = func(filename)
	print("Gen image",w,h,depth)
	if depth == 15 then
		wchar(memfile, TEXTURE4)
	elseif depth ==  255 then
		wchar(memfile, TEXTURE8)
	else
		error("Unsupport depth", depth)
	end
	wshort(memfile, w)
	wshort(memfile, h)
	table.insert(memfile.result, data)
	return table.concat(memfile.result)
end

local function load_png(filename)
	return _load(filename..".png", png.read)
end

local function load_ppm(filename)
	return _load(filename, ppm)
end

local write_block

if compress == "0" then

function write_block(f, t)
	wlong(f, -#t)
	f:write(t)
end

else

function write_block(f, t)
	local c = lzma.compress(t)
	wlong(f, #c)
	f:write(c)
end

end

filename = string.match(filename, "(.*)%..*$")

local f = io.open(filename .. ".ep", "wb")
local gm_filename, gm_load = nil, nil

if model == "-ppm" then
	gm_load = load_ppm
	gm_filename = filename.."."
elseif model =="-png" then
	gm_load = load_png
	gm_filename = filename
else
	error("not match ppm or png  model.")
end

for i = 1,tex do
	local t = gm_load(gm_filename..tostring(i))
	write_block(f, t)
end

write_block(f, detail())

f:close()
