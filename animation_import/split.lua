local arg = ...

local anim = {}

function recursive_compare(t1,t2)
  -- Use usual comparison first.
  if t1==t2 then return true end

  if t1 == nil or t2 == nil then return false end

  -- We only support non-default behavior for tables
  if (type(t1)~="table") then return false end
  -- They better have the same metatables
  local mt1 = getmetatable(t1)
  local mt2 = getmetatable(t2)
  if( not recursive_compare(mt1,mt2) ) then return false end

  -- Check each key-value pair
  -- We have to do this both ways in case we miss some.
  -- TODO: Could probably be smarter and not check those we've 
  -- already checked though!
  for k1,v1 in pairs(t1) do
    local v2 = t2[k1]
    if( not recursive_compare(v1,v2) ) then return false end
  end
  for k2,v2 in pairs(t2) do
    local v1 = t1[k2]
    if( not recursive_compare(v1,v2) ) then return false end
  end

  return true  
end

local function load(filename)
	local f = io.open(filename, "r")

	local t = loadstring(f:read("*all"))
	anim = t()

	f:close()
end

load(arg)

for k in pairs(anim) do
--	print(k)
	local a = anim[k]

	local component = a["str_component"]
	local frame = a["frame"]

	local f = io.open(k.."_anim.json", "w")
	---
	f:write("{\n")
	f:write("\"name\":\""..k.."\",\n")
	f:write("\"fps\" : 24,\n")
	f:write("\"layer\" : \n")
	f:write("[\n")
	f:write("{\n")
	f:write("\"name\" : \"\",\n")
	f:write("\"frame\" : \n")
	f:write("[\n")

	local last = nil
	for k in pairs(frame) do

		if recursive_compare(frame[k], last) and k ~= #frame then
--		if frame[k] == last then
			-- skip
		else
	--		print("frame " .. k)
			f:write("{\n")
			f:write("\"actor\" : \n")
			f:write("[\n")

			for j in pairs(frame[k]) do
				local index = frame[k][j]

				local scale = 1
				local filename
				if type(index) == "table" then
					filename = component[index["index"]+1]
					scale = index["mat"][1] / 1024
				else
					filename = component[index+1]
				end
				
					f:write("{\n")
					f:write("\"angle\" : 0.0,\n")
					f:write("\"filepath\" : ".."\""..filename..".png\",\n")
					f:write("\"name\" : \"\",\n")
					f:write("\"position\" : {\"x\" : 0.0,\"y\" : 0.0},\n")
					f:write("\"x mirror\" : false,\n")
					f:write("\"y mirror\" : false,\n")

					f:write("\"x scale\" : "..scale..",\n")
					f:write("\"y scale\" : "..scale.."\n")

					if j == #frame[k] then
						f:write("}\n")
					else
						f:write("},\n")
					end

			end

			f:write("],\n")

			f:write("\"time\":"..k..",\n")
			f:write("\"tween\" : false\n")

			if k == #frame then
				f:write("}\n")
			else
				f:write("},\n")
			end
		end

		last = frame[k]

	end

	f:write("], \n")
	f:write("}] \n")
	f:write("}\n")
	---

	f:close()
end

--print(#anim["2003daoke1_attack1_3"]["str_component"])
