local p = require("printcoc")

local src, packer, dst, filter = ...

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

f = assert(loadfile(src, "t", { picture = picture, animation = animation, texture = texture}))
f()

local pat = "(%S+)%s+(%S+)%s+(%S+)%s+"

f = assert(io.open(packer, "r"))
line = f:read("*line")
while line do
	--print(line)
	if string.find(line, "filename") and string.find(line, filter) then
		local i, j = string.find(line, "%d+")
		local id = tonumber(string.sub(line, i, j))
		local index = tonumber(string.match(line, "%d+", j+1))
--		print(id, index)

		line = f:read("*line")
		local x = tonumber(string.match(line, "%d+", string.find(line, "x")))
		local y = tonumber(string.match(line, "%d+", string.find(line, "y")))
		local w = tonumber(string.match(line, "%d+", string.find(line, "w")))
		local h = tonumber(string.match(line, "%d+", string.find(line, "h")))
--		print(x, y, w, h)

		local left = x
		local right = x + w
		local up = y
		local down = y + h

		for _,v in pairs(pics) do
			if v.id == id then
				local src = v[index].src

				-- print("from")
				-- p.print_table2(src)

				local min = math.min(src[1], src[3], src[5])
				local max = math.max(src[1], src[3], src[5])
				local c = (min+max)*0.5
				for i = 1, 8, 2 do
					if src[i] < c then
						src[i] = left
					elseif src[i] > c then
						src[i] = right
					else
						assert(nil)
					end
				end

				local min = math.min(src[2], src[4], src[6])
				local max = math.max(src[2], src[4], src[6])
				local c = (min+max)*0.5
				for i = 2, 8, 2 do
					if src[i] < c then
						src[i] = up
					elseif src[i] > c then
						src[i] = down
					else
						assert(nil)
					end
				end

				-- print("to")
				-- p.print_table2(src)

			end
		end
	end

	line = f:read("*line")
end
f:close()


p.printcoc(pics, anis, dst)

