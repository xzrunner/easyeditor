-- 生成json格式文件，及图片位置信息

json = require('json')

local srcfile, name, tmpdir = ...

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

f = assert(loadfile(srcfile, "t", { picture = picture, animation = animation, texture = texture}))
f()

local function print_pic(pic)
	local fout = io.open(tmpdir..'/'..pic.id.."_pic.tmp", "w")
	fout:write(json.encode(pic))
	fout:close()
end

local function print_ani(ani)
	filepath = tmpdir..'/'..ani.id
	if #ani[1] > 1 then
		filepath = filepath.."_ani.tmp"
	else
		filepath = filepath.."_cmp.tmp"
	end
	local fout = io.open(filepath, "w")
	for _,v in pairs(ani.component) do
		print_fromid(v.id)
	end
	fout:write(json.encode(ani))
	fout:close()
end

function print_fromid(id)
	for _,v in pairs(pics) do
		if v.id == id then
			print_pic(v)
			return
		end
	end
	for _,v in pairs(anis) do
		if v.id == id then
			print_ani(v)
			return
		end
	end
	assert(0)
end

for _,v in pairs(anis) do
	if name == v.export then
		print_ani(v)
	end
end