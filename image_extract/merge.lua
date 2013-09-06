local libprint = require("printcoc")

local file1, file2, output = ...

-- file1

local pic1 = {}
local ani1 = {}
local tex1 = 0

local function picture(tbl)
	table.insert(pic1, tbl)
end

local function animation(tbl)
	table.insert(ani1, tbl)
end

local function texture(n)
	tex1 = n
end

f = assert(loadfile(file1, "t", { picture = picture, animation = animation, texture = texture}))
f()

-- file2

local pic2 = {}
local ani2 = {}
local tex2 = 0

local function picture(tbl)
	table.insert(pic2, tbl)
end

local function animation(tbl)
	table.insert(ani2, tbl)
end

local function texture(n)
	tex2 = n
end

f = assert(loadfile(file2, "t", { picture = picture, animation = animation, texture = texture}))
f()

local function idused(t, id)
	for _,v in pairs(t) do
		if v.id == id then
			return true
		end
	end
	return false
end

local function trans(pic, ani, from, to)
	for _,v in pairs(pic) do
		if v.id == from then
			v.id = to
		end
	end
	for _,v in pairs(ani) do
		if v.id == from then
			v.id = to
		end
		if v.component then
			for _,v in pairs(v.component) do
				if v.id == from then
					v.id = to
					print("in compoent")
				end
			end
		end
	end
end

-- main
local id = 0
for _,v in pairs(pic2) do
	while idused(pic1, v.id) or idused(ani1, v.id) or idused(ani2, v.id) do
		print("from: "..v.id.."to: "..id)
		trans(pic2, ani2, v.id, id)
		id = id + 1
	end
end
for _,v in pairs(ani2) do
	while idused(pic1, v.id) or idused(ani1, v.id) or idused(pic2, v.id) do
		print("from: "..v.id.."to: "..id)
		trans(pic2, ani2, v.id, id)
		id = id + 1
	end
end

libprint.printcoc(pic1, ani1, output.."1")
libprint.printcoc(pic2, ani2, output.."2")
