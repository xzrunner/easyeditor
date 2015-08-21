static const char* trans_old_ejoy2d_data2 = STRINGIFY(

local data = {}

local function picture(tbl)
	tbl["type"] = "picture"
	table.insert(data, tbl)
end

local function animation(tbl)
	tbl["type"] = "animation"
	table.insert(data, tbl)
end

local function texture(n)
end

local _env = _ENV
_env.picture = picture
_env.animation = animation
_env.texture = texture

function trans2table(filename)
	f = assert(loadfile(filename , "t", _env))
	f()  
	
	local max_id = 0
	for _, v in ipaires(data) do
		if v.id > max_id then
			max_id = v.id
		end
	end
	
	for _, v in ipaires(data) do
		if v.type == 'animation' then
			-- extract label
			for _, comp in ipairs(v.component) do
				if comp.id == nil then
					-- label
					if comp.font then
						max_id = max_id + 1
						local label_id = max_id
						data[#data+1] = {
							type = 'label',
							id = label_id,
							align = comp.align & 0xf,
							color = comp.color,
							size = comp.size + 8,
							width = comp.width,
							height = comp.height,
							noedge = false, -- FIXME
							space_w = 0, -- FIXME
							space_h = 0, -- FIXME
							auto_size = 0, -- FIXME							
						}
						comp.id = label_id
					end
				end
			end	
			
			-- fix color
			for _, action in ipairs(v) do
				for _, frame in ipairs(action) do
					for _, part in ipairs(frame) do
						if part.color then
							local c = part.color
							local b = (c >> 24) & 0xff
							local g = (c >> 16) & 0xff
							local r = (c >>  8) & 0xff
							local a =  c        & 0xff
							part.color = (a<<24) | (r<<16) | (g<<8) | b
						end
						if part.add then
							local c = part.add
							local a = (c >> 24) & 0xff
							local b = (c >> 16) & 0xff
							local g = (c >>  8) & 0xff
							local r =  c        & 0xff
							part.add = (a<<24) | (r<<16) | (g<<8) | b
						end
					end
				end
			end			
		end
	end
	
	return data, max_id
end

);