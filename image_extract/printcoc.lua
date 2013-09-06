local M = {}

function M.print_table2(t)
	if type(t) == "table" then
		f:write("{ ")
		for k,v in pairs(t) do
			if type(k) == "string" then
				if k == "color" then
					print(k.." = 0x"..string.format("%x", v)..", ")
				else
					print(k.." = ")
					M.print_table2(v)
				end
			else
				M.print_table2(v)
			end
		end
		print("},")
	else
		if (type(t) == "string") then
			print("\""..t.."\", ")
		else
			print(t..", ")
		end
	end
end

function M.print_table(t, f)
	if type(t) == "table" then
		f:write("{ ")
		for k,v in pairs(t) do
			if type(k) == "string" then
				if k == "color" then
			--		f:write(k.." = 0x"..string.format("%x", v)..", ")
				else
					f:write(k.." = ")
					M.print_table(v,f)
				end
			else
				M.print_table(v,f)
			end
		end
		f:write("},")
	else
		if (type(t) == "string") then
			f:write("\""..t.."\", ")
		else
			f:write(t..", ")
		end
	end
end

function M.print_picture(picture, f)
	for _,v in pairs(picture) do
		f:write("picture {\n")
		f:write("\tid = "..v.id..",\n")
		for _,u in ipairs(v) do
			u.tex = 0
			f:write("\t")
			M.print_table(u, f)
			f:write("\n")
		end
		f:write("}\n")
	end	
end

function M.print_animation(animation, f)
	for _,v in pairs(animation) do
		f:write("animation {\n")
		if v["export"] then
			f:write("\texport = \""..v["export"].."\",\n")
		end
		if v["id"] then
			f:write("\tid = "..v["id"]..",\n")
		end
		if v["component"] then
			f:write("\tcomponent = {\n")
			for _,v in pairs(v["component"]) do
				f:write("\t\t")
				M.print_table(v,f)
				f:write("\n")
			end
			f:write("\t},\n")
		end
		for _,v in ipairs(v) do
			f:write("\t{\n")
			for _,v in ipairs(v) do
				f:write("\t\t")
				M.print_table(v,f)
				f:write("\n")			
			end
			f:write("\t},\n")
		end
		f:write("}\n")
	end
end

function M.printcoc(picture, animation, dstfile)
	local f = io.open(dstfile, "w")
	f:write("texture(1)\n")
	M.print_picture(picture, f)
	M.print_animation(animation, f)
	f:close()
end

return M