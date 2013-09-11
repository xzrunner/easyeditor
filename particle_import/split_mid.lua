-- 根据filter的name列表从原始数据中提取出编辑器用的格式的文件
-- filename: 原始数据文件buildings.sc.lua
-- filter: 要提取的anim的export名称列表
-- 输出到./middle/中

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

local buffer = {}
for _,v in pairs(filelist) do
	local find = false
	for _,u in pairs(anis) do
		if v == u.export then
			find = true
			table.insert(buffer, u)
			break
		end
	end
	assert(find)
end

function recursive_compare(t1,t2)
  -- Use usual comparison first.
  if t1==t2 then return true end

  if t1 == nil or t2 == nil then return false end

  -- We only support non-default behavior for tables
  if (type(t1)~="table" and type(t2)=="table") then return false end
  if (type(t1)=="table" and type(t2)~="table") then return false end
  if (type(t1)~="table" and type(t2)~="table" and t1~=t2) then return false end
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


function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

local function cross_product(v)
	return (v[3]-v[1])*(v[6]-v[4]) - (v[4]-v[2])*(v[5]-v[3])
end

-- 0 3
-- 1 2
local function quad_type(x, y, quad)
	local cx = (quad[1]+quad[3]+quad[5]+quad[7])/4
	local cy = (quad[2]+quad[4]+quad[6]+quad[8])/4
	if x < cx and y < cy then
		return 0
	elseif x < cx and y > cy then
		return 1
	elseif x > cx and y > cy then
		return 2
	elseif x > cx and y < cy then
		return 3
	else
		return -1
	end
end

-- to complex
local function write_pic(pic)

	local out = io.open("middle/"..pic.id.."_complex.json", "w")
	out:write("{\n")
	out:write("\t\"name\" : \"\",\n")
	out:write("\t\"sprite\" : \n")
	out:write("\t[\n")

	for i,v in ipairs(pic) do
		out:write("\t\t{\n")

		local name
		if #pic == 1 then
			name = pic.id
		else
			name = pic.id.."_"..i
		end
		name = name..".png"

		out:write("\t\t\t\"filepath\" : ".."\"../pictures/"..name.."\",\n")

		local src1 = quad_type(v.src[1], v.src[2], v.src)
		local src2 = quad_type(v.src[3], v.src[4], v.src)
		local src3 = quad_type(v.src[5], v.src[6], v.src)
		local src4 = quad_type(v.src[7], v.src[8], v.src)
		local screen1 = quad_type(v.screen[1], v.screen[2], v.screen)
		local screen2 = quad_type(v.screen[3], v.screen[4], v.screen)
		local screen3 = quad_type(v.screen[5], v.screen[6], v.screen)
		local screen4 = quad_type(v.screen[7], v.screen[8], v.screen)
		
		local ox, oy
		ox = (v.screen[1]+v.screen[3]+v.screen[5]+v.screen[7])*0.5*0.5*0.1
		oy = (v.screen[2]+v.screen[4]+v.screen[6]+v.screen[8])*0.5*0.5*0.1

		local t0, t1, t2, t3
		if src4 == 0 then
			t0, t1, t2, t3 = screen4, screen3, screen2, screen1
		elseif src3 == 0 then
			t0, t1, t2, t3 = screen3, screen2, screen1, screen4
		elseif src2 == 0 then
			t0, t1, t2, t3 = screen2, screen1, screen4, screen3
		elseif src1 == 0 then
			t0, t1, t2, t3 = screen1, screen4, screen3, screen2
		else
			t0, t1, t2, t3 = screen1, screen4, screen3, screen2
		end

		local xmirror = false
		local ymirror = false
		local angle = 0
		local rotate = false
		if t0 == 1 and t1 == 2 and t2 == 3 and t3 == 0 then
			angle = math.pi*0.5
			rotate = true
		elseif t0 == 3 and t1 == 0 and t2 == 1 and t3 == 2 then
			angle = -math.pi*0.5
			rotate = true
		elseif t0 == 2 and t1 == 3 and t2 == 0 and t3 == 1 then
			angle = math.pi
		elseif t0 == 3 and t1 == 2 and t2 == 1 and t3 == 0 then
			xmirror = true
		elseif t0 == 2 and t1 == 1 and t2 == 0 and t3 == 3 then	
			xmirror = true
			angle = -math.pi*0.5
			rotate = true
		elseif t0 == 1 and t1 == 0 and t2 == 3 and t3 == 2 then	
			ymirror = true
		elseif t0 == 0 and t1 == 3 and t2 == 2 and t3 == 1 then		
			xmirror = true
			angle = math.pi*0.5
			rotate = true
		end

		local sx, sy
		local src_x = math.max(math.abs(v.src[1]-v.src[3]), math.abs(v.src[1]-v.src[7]))
		local src_y = math.max(math.abs(v.src[2]-v.src[4]), math.abs(v.src[2]-v.src[8]))
		local screen_x = math.max(math.abs(v.screen[1]-v.screen[3]), math.abs(v.screen[1]-v.screen[7])) * 0.1
		local screen_y = math.max(math.abs(v.screen[2]-v.screen[4]), math.abs(v.screen[2]-v.screen[8])) * 0.1
		if rotate then
			sx = screen_y/src_x
			sy = screen_x/src_y			
		else
			sx = screen_x/src_x
			sy = screen_y/src_y
		end

		-- if pic.id == 2046 then
		-- 	print("++++++++++++++++++++")
		-- 	print(src1, src2, src3, src4)
		-- 	print(screen1, screen2, screen3, screen4)
		-- 	print("scale", sx, sy)	
		-- 	print(src_x, src_y)
		-- 	print(screen_x, screen_y)
		-- 	print(rotate)
		-- end

		out:write("\t\t\t\"angle\" : "..angle..",\n")
		out:write("\t\t\t\"x scale\" : "..sx..",\n")
		out:write("\t\t\t\"y scale\" : "..sy..",\n")
		out:write("\t\t\t\"x shear\" : 0,\n")
		out:write("\t\t\t\"y shear\" : 0,\n")		
		if xmirror then
			out:write("\t\t\t\"x mirror\" : true,\n")
		else
			out:write("\t\t\t\"x mirror\" : false,\n")
		end
		if ymirror then
			out:write("\t\t\t\"y mirror\" : true,\n")
		else
			out:write("\t\t\t\"y mirror\" : false,\n")
		end
		out:write("\t\t\t\"position\" : \n")
		out:write("\t\t\t{\n")
		out:write("\t\t\t\t\"x\" : "..ox..",\n")
		out:write("\t\t\t\t\"y\" : "..oy.."\n")
		out:write("\t\t\t}\n")

		-- local xmirror, ymirror
		-- if cross_product(v.src) * cross_product(v.screen) < 0 then
		-- 	if v.src[1] == v.src[3] then
		-- 		assert(v.screen[1] == v.screen[3]) -- no rotate
		-- 		xmirror = (v.src[3]-v.src[5])*((v.screen[3]-v.screen[5]) < 0
		-- 		ymirror = (v.src[2]-v.src[4])*((v.screen[2]-v.screen[4]) < 0
		-- 	else
		-- 		assert(v.src[1] == v.src[7] and v.screen[1] == v.screen[7]) -- no rotate
		-- 		xmirror = (v.src[1]-v.src[3])*((v.screen[1]-v.screen[3]) < 0
		-- 		ymirror = (v.src[4]-v.src[6])*((v.screen[4]-v.screen[6]) < 0
		-- 	end
		-- 	assert((xmirror and not ymirror) or (not xmirror and ymirror)) -- xmirro or ymirror
		-- else
		-- 	xmirror = false
		-- 	ymirror = false
		-- end

		-- local angle
		-- if v.src[1] == v.src[3] then
		-- 	if v.screen[1] == v.screen[3] then
		-- 		if (v.src[2]-v.src[4])*(v.screen[2]-v.screen[4]) < 0 then
		-- 			angle = math.pi
		-- 		else
		-- 			angle = 0
		-- 	else
		-- 		if v.src[4] > v.src[2] then
		-- 			if v.screen[3] > v.screen[1] then
		-- 				angle = -math.pi * 0.5
		-- 			else
		-- 				angle = -math.pi * 1.5
		-- 			end
		-- 		else

		-- 		end
		-- 	end
		-- else
		-- 	assert(v.src[1] == v.src[7])
		-- end
		-- -- suppose mirror and rotate not used together
		-- if xmirror or ymirror then
		-- 	assert(angle == 0)
		-- end

		-- local sx, sy


		if i == #pic then
			out:write("\t\t}\n")
		else
			out:write("\t\t},\n")
		end
	end

	out:write("\t]\n")
	out:write("}\n")
	out:close()	
end

local function parser_mat(anim, t, mat)

	t.xmirror = false
	t.ymirror = false

	t.ox = mat[5] / 16
	t.oy = -mat[6] / 16

	local angle1 = math.atan(-mat[3]/mat[1])
	local angle2 = math.atan(mat[2]/mat[4])
	if math.abs(angle1-angle2) < 0.00001 then
		-- print("angle", angle1, angle2)
		-- print("angle not same", anim.id)
		-- print(mat[1], mat[2], mat[3], mat[4], mat[5], mat[6])

		-- no shear
		if angle1 == 0 then
			t.sx = mat[1]/1024/math.cos(angle1)
			t.sy = mat[4]/1024/math.cos(angle1)
		else
			t.sx = -mat[3]/1024/math.sin(angle1)
			t.sy = mat[2]/1024/math.sin(angle1)
		end
		t.kx = 0
		t.ky = 0
		t.angle = angle1

		assert(math.abs(mat[1] - math.cos(t.angle)*t.sx*1024) < 0.00001)
		assert(math.abs(mat[2] - math.sin(t.angle)*t.sy*1024) < 0.00001)
		assert(math.abs(mat[3] + math.sin(t.angle)*t.sx*1024) < 0.00001)
		assert(math.abs(mat[4] - math.cos(t.angle)*t.sy*1024) < 0.00001)		
	else
		-- no rotate
		t.sx = mat[1]/1024
		t.sy = mat[4]/1024
		t.kx = mat[3]/mat[4]
		t.ky = mat[2]/mat[1]
		t.angle = 0		
	end
end

local function isanim(id)
	local anim = nil
	for _,v in pairs(anis) do
		if v.id == id then
			anim = v
			break
		end
	end
	if not anim then
		local pic = nil
		for _,v in pairs(pics) do
			if v.id == id then
				pic = v
				break
			end
		end

		if not pic then
			print("error id", id)
			assert(false)
		else
			return false
		end
	end
	local frames = anim[1]
	return #frames > 1
end

-- to animation
local function write_anim(anim, name)

	local f = io.open("middle/"..name.."_anim.json", "w")
	---
	f:write("{\n")
	f:write("\t\"name\":\""..name.."\",\n")
	f:write("\t\"fps\" : 30,\n")
	f:write("\t\"layer\" : \n")
	f:write("\t[\n")
	f:write("\t\t{\n")
	f:write("\t\t\t\"name\" : \"\",\n")
	f:write("\t\t\t\"frame\" : \n")
	f:write("\t\t\t[\n")

	local last = nil
	assert(#anim == 1)
	local frames = anim[1]
	for k,frame in ipairs(frames) do

		if recursive_compare(frame, last) and k ~= #frames then

		else
			f:write("\t\t\t\t{\n")
			f:write("\t\t\t\t\t\"actor\" : \n")
			f:write("\t\t\t\t\t[\n")

			for j,sprite in ipairs(frame) do

				local name
				if type(sprite) == "table" then
					name = anim.component[sprite.index+1].id
				else
					name = anim.component[sprite+1].id
				end
				if isanim(name) then
					name = name.."_anim.json"
				else
					name = name.."_complex.json"
				end

				local ox, oy
				local angle
				local sx, sy
				local kx, ky
				local xmirror, ymirror
				if type(sprite) == "table" and sprite.mat then
					local t = {}
					parser_mat(anim, t, sprite.mat)
					ox = t.ox
					oy = t.oy
					angle = t.angle
					sx = t.sx
					sy = t.sy
					kx = t.kx
					ky = t.ky
					xmirror = t.xmirror
					ymirror = t.ymirror
				else
					ox = 0
					oy = 0
					angle = 0
					sx = 1
					sy = 1
					kx = 0
					ky = 0
					xmirror = false
					ymirror = false
				end

				local tab = "\t\t\t\t\t\t"
				f:write(tab.."{\n")
				f:write(tab.."\t\"angle\" : "..angle..",\n")
				f:write(tab.."\t\"filepath\" : ".."\""..name.."\",\n")
				f:write(tab.."\t\"name\" : \"\",\n")
				f:write(tab.."\t\"position\" : {\"x\" : "..ox..",\"y\" : "..oy.."},\n")
				f:write(tab.."\t\"x mirror\" : false,\n")
				f:write(tab.."\t\"y mirror\" : false,\n")
				f:write(tab.."\t\"x scale\" : "..sx..",\n")
				f:write(tab.."\t\"y scale\" : "..sy..",\n")
				f:write(tab.."\t\"x shear\" : "..kx..",\n")
				f:write(tab.."\t\"y shear\" : "..ky.."\n")

				if j == #frame then
					f:write(tab.."}\n")
				else
					f:write(tab.."},\n")
				end
			end

			f:write("\t\t\t\t\t],\n")

			f:write("\t\t\t\t\t\"time\":"..k..",\n")
			f:write("\t\t\t\t\t\"tween\" : false\n")

			if k == #frames then
				f:write("\t\t\t\t}\n")
			else
				f:write("\t\t\t\t},\n")
			end
		end

		last = frame

	end

	f:write("\t\t\t]\n")
	f:write("\t\t}\n")
	f:write("\t]\n")
	f:write("}\n")
	---

	f:close()	
end

local function write_complex(anim, name, components, sprites)

	local out = io.open("middle/"..name.."_complex.json", "w")
	out:write("{\n")
	out:write("\t\"name\" : \""..name.."\",\n")
	out:write("\t\"sprite\" : \n")
	out:write("\t[\n")

	for i,v in ipairs(sprites) do
		out:write("\t\t{\n")

		local name
		if type(v) == "table" then
			name = components[v.index+1].id
		else
			name = components[v+1].id
		end
		if isanim(name) then
			name = name.."_anim.json"
		else
			name = name.."_complex.json"
		end

		local ox, oy
		local angle
		local sx, sy
		local kx, ky
		local xmirror, ymirror
		if type(v) == "table" and v.mat then
			local t = {}
			parser_mat(anim, t, v.mat)
			ox = t.ox
			oy = t.oy
			angle = t.angle
			sx = t.sx
			sy = t.sy
			kx = t.kx
			ky = t.ky
			xmirror = t.xmirror
			ymirror = t.ymirror
		else
			ox = 0
			oy = 0
			angle = 0
			sx = 1
			sy = 1
			kx = 0
			ky = 0
			xmirror = false
			ymirror = false
		end

		--

		out:write("\t\t\t\"filepath\" : \""..name.."\",\n")
		out:write("\t\t\t\"angle\" : "..angle..",\n")
		out:write("\t\t\t\"x scale\" : "..sx..",\n")
		out:write("\t\t\t\"y scale\" : "..sy..",\n")
		out:write("\t\t\t\"x shear\" : "..kx..",\n")
		out:write("\t\t\t\"y shear\" : "..ky..",\n")		
		if xmirror then
			out:write("\t\t\t\"x mirror\" : true,\n")
		else
			out:write("\t\t\t\"x mirror\" : false,\n")
		end
		if ymirror then
			out:write("\t\t\t\"y mirror\" : true,\n")
		else
			out:write("\t\t\t\"y mirror\" : false,\n")
		end
		out:write("\t\t\t\"position\" : \n")
		out:write("\t\t\t{\n")
		out:write("\t\t\t\t\"x\" : "..ox..",\n")
		out:write("\t\t\t\t\"y\" : "..oy.."\n")
		out:write("\t\t\t}\n")

		if i == #sprites then
			out:write("\t\t}\n")
		else
			out:write("\t\t},\n")
		end
	end

	out:write("\t]\n")
	out:write("}\n")
	out:close()		
end

local function write_anim2(anim)
	-- children
	local component = anim.component
	for _,v in pairs(component) do
		local id = v.id
		local find = false
		for _,v in pairs(pics) do
			if v.id == id then
				write_pic(v)
				find = true
				break
			end
		end
		if not find then
			for _,v in pairs(anis) do
				if v.id == id then
					write_anim2(v)
					find = true
					break
				end
			end
		end
		assert(find)
	end

	-- 
	local frames = anim[1]
	local name
	if anim.export then name = anim.export else name = anim.id end	
	if #frames == 1 then
		write_complex(anim, name, component, frames[1])
	else
		write_anim(anim, name)
	end
end

local out_pic = io.open("pictures.txt", "w")

for _,v in pairs(buffer) do
	-- if #v.component == 1 and #v == 1 then
	-- 	out_pic:write(v.export.." "..v.component[1].id.."\n")
	-- else
	-- 	write_anim(v.export, v)
	-- end

	write_anim2(v)
end

out_pic:close()

---- 打印pic中src和screen的grid类型，人工判断每种情况的处理方式
-- local types = {}
-- for _,pic in pairs(pics) do
-- 	for _,v in ipairs(pic) do
-- 		local src1 = quad_type(v.src[1], v.src[2], v.src)
-- 		local src2 = quad_type(v.src[3], v.src[4], v.src)
-- 		local src3 = quad_type(v.src[5], v.src[6], v.src)
-- 		local src4 = quad_type(v.src[7], v.src[8], v.src)
-- 		local screen1 = quad_type(v.screen[1], v.screen[2], v.screen)
-- 		local screen2 = quad_type(v.screen[3], v.screen[4], v.screen)
-- 		local screen3 = quad_type(v.screen[5], v.screen[6], v.screen)
-- 		local screen4 = quad_type(v.screen[7], v.screen[8], v.screen)

-- 		local item = {}
-- 		if src4 == 0 then
-- 			item = {src4, src3, src2, src1, screen4, screen3, screen2, screen1}
-- 		elseif src3 == 0 then
-- 			item = {src3, src2, src1, src4, screen3, screen2, screen1, screen4}
-- 		elseif src2 == 0 then
-- 			item = {src2, src1, src4, src3, screen2, screen1, screen4, screen3}
-- 		else
-- 			if (src1 ~= 0) then
-- 				print("??", src1, src2, src3, src4)
-- 			end
-- --			assert(src1 == 0)
-- 			item = {src1, src4, src3, src2, screen1, screen4, screen3, screen2}
-- 		end

-- 	--	local item = {src1, src2, src3, src4, screen1, screen2, screen3, screen4}
-- 		local find = false
-- 		for _,v in pairs(types) do
-- 			find = true
-- 			for i=1,8 do
-- 				if v[i] ~= item[i] then
-- 					find = false
-- 					break
-- 				end
-- 			end
-- 			if find then break end
-- 		end
-- 		if not find then table.insert(types, item) end

-- --		print(src1, src2, src3, src4)
-- --		print(screen1, screen2, screen3, screen4)
-- 	end
-- end
-- for _,v in pairs(types) do
-- 	print("+++++++++++++++++++++++++")
-- 	print(v[1], v[2], v[3], v[4])	
-- 	print(v[5], v[6], v[7], v[8])
-- end