local tools = require 'tools'

local function _parse_line(line)
	local ret = {}
	local count, front = 1 , 1
	local i = 1
	while i<=string.len(line) do
		if string.byte(line, i) == string.byte(',') then -- find ,
			if string.byte(line, i+1) == string.byte('"') then  -- match , "...""
				if front < i-1 then
					ret[count] = string.sub(line, front, i-1)
					count = count + 1
				end
				front = i + 2
				for j=i+2, string.len(line) do
					if string.byte(line, j) == string.byte('\"') then
						ret[count] = string.sub(line, front, j-1)
						count = count + 1
						if j == string.len(line) then
							return ret
						else
							assert(string.byte(line, j+1) == string.byte(','))
							front = j + 2
							i = j
						end
						break
					end
				end
			else -- match , ...
				if front < i-1 then
					ret[count] = string.sub(line, front, i-1)
					count = count + 1
				end
				front = i +1
			end
		end
		i= i+1		
	end

	ret[count] = string.sub(line, front, -1)
	return ret
end

local texts_table = {}
local file = io.open('texts.csv', 'r')
local all_keys = _parse_line(file:lines()())

-- pass value type
file:lines()()

 -- gen language table
for i=2,#all_keys do
	texts_table[all_keys[i]] = {}
end

local _c = 3
for line in file:lines() do
	local tl = _parse_line(line)
	local tid = tl[1]
	for i=2,#tl do
		texts_table[all_keys[i]][tid] = tl[i]
	end
	_c = _c + 1
end

file:close()

-- write lua file
local fd = io.open('texts.lua', 'w')
local str = 'return '..tools.table2str(texts_table)
fd:write(str)
fd:close()


