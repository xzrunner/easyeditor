local filename, filter = ...

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

f = assert(loadfile(filename, "t", { picture = picture, animation = animation, texture = texture}))
f()

f = assert(loadfile(filter))
local filelist = f()

local cache_ani = {}
for _,v in pairs(filelist) do
	local find = false
	for _,u in pairs(anis) do
		if u.export and u.export == v then
			find = true
			table.insert(cache_ani, u)
			break;
		end
	end
	if not find then
		print("id", v)
	end
	assert(find == true)
end

local out = io.open(filter..".txt", "w")
local out_quads = io.open(filter..".quads.txt", "w")

local function print_table(t)
	if type(t) == "table" then
		out:write("{ ")
		for k,v in pairs(t) do
			if type(k) == "string" then
				if k == "color" then
					out:write(k.." = 0x"..string.format("%x", v)..", ")
				else
					out:write(k.." = ")
					print_table(v)
				end
			else
				print_table(v)
			end
		end
		out:write("},")
	else
		if (type(t) == "string") then
			out:write("\""..t.."\", ")
		else
			out:write(t..", ")
		end
	end
end

local queue = { ptr = 0, size = 0, data = {} }
-- init queue
for k,v in pairs(cache_ani) do
	queue.data[k] = v
end
queue.ptr = 1
queue.size = #cache_ani

local pic_cache = {}

local function print_ani(ani)
	if ani["export"] then
		out:write("\texport = \""..ani["export"].."\",\n")
	end
	if ani["id"] then
		out:write("\tid = "..ani["id"]..",\n")
	end
	if ani["component"] then
		out:write("\tcomponent = {\n")
		for _,v in pairs(ani["component"]) do
			out:write("\t\t")
			print_table(v)
			--find in anim
			for _,u in pairs(anis) do
				if u.id == v.id then
					queue.size = queue.size + 1
					queue.data[queue.size] = u
				end
			end
			--find in picture
			for _,u in pairs(pics) do
				if u.id == v.id then
					pic_cache[u.id] = u
				end
			end
			out:write("\n")
		end
		out:write("\t},\n")
	end
	for _,v in ipairs(ani) do
		out:write("\t{\n")
		for _,v in ipairs(v) do
			out:write("\t\t")
			print_table(v)
			out:write("\n")			
		end
		out:write("\t},\n")
	end
end

-- print all ani
local printed = {}
while queue.ptr <= queue.size do
	local ani = queue.data[queue.ptr]
	if not printed[ani.id] then
		printed[ani.id] = true

		out:write("animation {\n")
--		print("ptr", queue.ptr)
		print_ani(ani)
		out:write("}\n")
	end
	
	queue.ptr = queue.ptr + 1
end

-- print all pic
for _,v in pairs(pic_cache) do
	out:write("picture {\n")
	out:write("\tid = "..v.id..",\n")
	for _,u in ipairs(v) do
		out:write("\t")
		print_table(u)
		out:write("\n")

		out_quads:write(filename.."_"..v.id.."_".._.." "..u.tex.." ")
		for _,v in pairs(u.src) do
			out_quads:write(v.." ")
		end
		out_quads:write("\n")
	end
	out:write("}\n")
end

out:close()
out_quads:close()
