local READ_ME = [[
	usepage:
		lua epbin.lua <gen_model> <model> <filename>
	gen_model:
		-ep : 生成单个的ep文件
		-pd : 生成pic 和animation分离的两个文件(xx.epp: 图片打包文件 xx.epd animation打包文件)
	model: 
		-ppm:       使用的贴图为ppm文件  
		-png8:      使用的贴图为png8文件
		-png4:      使用的贴图为png4文件
		-pvr:       使用的贴图文件为pvr压缩文件
		-ktx:		使用的贴图文件为ktx压缩文件
	filename:
		导出的lua文件名

	PS:
		贴图的文件命名方式为filename<1..8>.<ext> 其中filename为lua的filename相同(无后缀名)
]]

local bit32 = require "bit32"
local ppm = require "ppm"
local epconv = require "epconv"
local lzma = require "lzma"
local pvr = require "pvr"
local ktx = require "ktx"

local gen_model, model, filename, compress = ...
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

local _env = _ENV
_env.picture = picture
_env.animation = animation
_env.texture = texture
f = assert(loadfile (filename , "t", _env))
f()

local ANIMATION = 0
local PICTURE = 1
local CLIPUI = 2

local TEXTURE4 = 0
local TEXTURE8 = 1
local DATA = 2
local PVRTC = 3
local KTX = 4

local COMPONENT = 0
local SWITCH = 1
local LABEL = 2
local MOUNT = 3

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

local function waction(f,ani,t)
	local ncomp = #ani.component
	local ani_id = assert(ani.id)

	wstring(f, t.action)
	wshort(f, #t)
	for _,frame in ipairs(t) do
		wshort(f,#frame)
		for _,v in ipairs(frame) do
			if type(v) == "number" then
				assert(0 <= v and (v % 1) == 0 and v < ncomp, ani_id)
				wchar(f, FID)
				wshort(f, v)
			else
				local i = v.index
				assert(0 <= i and (i % 1) == 0 and i < ncomp, ani_id)

				if v.mat then
					assert(not (v.scale or v.trans))
				elseif v.scale or v.trans then
					local m = {1024,0,0,1024,0,0}
					if v.scale then
						local sx, sy = v.scale[1], v.scale[2]
						m[1] = m[1] * sx
						m[2] = m[2] * sy
						m[3] = m[3] * sx
						m[4] = m[4] * sy
						m[5] = m[5] * sx
						m[6] = m[6] * sy
					end
					if v.trans then
						local tx, ty = v.trans[1], v.trans[2]
						m[5] = m[5] + tx
						m[6] = m[6] + ty
					end
					v.mat = m
				end

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
				wshort(f, i)
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
	assert(t.id)
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
			if v.font then
				wchar(f,LABEL)
				wlabel(f,v)
			elseif v.name then
				wchar(f, MOUNT)
				wstring(f, v.name)
			end
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
		waction(f,t,v)
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


local function load_pvr(filename)
	memfile.result = {}
	local w,h,internal_format,data_table = pvr.load(filename..".pvr")
	print("Gen pvr image",w,h,internal_format)
	wchar(memfile, PVRTC)
	assert(internal_format == 4 or internal_format == 2)
	wchar(memfile, internal_format)
	wshort(memfile, w)
	wshort(memfile, h)
	for i=1,#data_table do
		wlong(memfile, string.len(data_table[i]))
		table.insert(memfile.result, data_table[i])
	end
	return table.concat(memfile.result)
end

local function load_ktx(filename)
	print("load_ktx :" .. filename..".ktx")
	memfile.reault = {}
	local w,h,data = ktx.read(filename..".ktx")
	print("Gen ktx image",w,h)
	wchar(memfile, KTX)
	wchar(memfile, w)
	wchar(memfile, h)
	table.insert(memfile.result, data)
	return table.concat(memfile.result)
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
	local png = require "png"
	return _load(filename..".png", function (name)
		return png.read(name, model)
		end)
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

local gm_filename, gm_load = nil, nil

if model == "-ppm" then
	gm_load = load_ppm
	gm_filename = filename.."."
elseif model =="-png8"  or model=="-png4" then
	gm_load = load_png
	gm_filename = filename
elseif model =="-pvr" then
	gm_load = load_pvr
	gm_filename = filename
elseif model == "-ktx" then
	gm_load = load_ktx
	gm_filename = filename
else
	print(READ_ME)
	error("not match ppm or png  model.")
end

-- gen pic data
local function gen_epp(f_epp)
	for i = 1,tex do
		local t = gm_load(gm_filename..tostring(i))
		write_block(f_epp, t)
	end
end

-- gen animation data
local function gen_epd(f_epd)
	write_block(f_epd, detail())
end


if gen_model == "-ep" then
	local f = io.open(filename.. ".ep", "wb")
	gen_epp(f)
	gen_epd(f)
	f:close()
elseif gen_model == "-pd" then
	local f_epp = io.open(filename .. ".epp", "wb")
	local f_epd = io.open(filename..".epd", "wb")
	gen_epp(f_epp)
	gen_epd(f_epd)
	f_epp:close()
	f_epd:close()
else
	print(READ_ME)
	error("not match ep or pd model.")
end


