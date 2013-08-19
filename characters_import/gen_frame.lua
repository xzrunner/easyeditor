
 -- pass pic
function picture(t)
	return nil
end

function texture(n)
	return nil
end


local ani_table =  {}

local  function t2str(t)
	local str = ''
	for i=1,#t do
		if type(t[i]) == 'table' then
			str = string.format('%s%d', str, t[i].index or -1)
			str = string.format('%s%x', str, t[i].color or 0xff)
			str = string.format('%s%d', str, t[i].add or -1)
			if t[i].mat then
				str = string.format('%s%d%d%d%d', str, t[i].mat[1],t[i].mat[2],t[i].mat[3],t[i].mat[4])
			end
		else
			str = string.format('%s%d',str,t[i])
		end
	end

	return str
end


local function cmp_t(t1, t2)
	if type(t1) == type(t2) then
		assert(type(t1) == 'table')
		if t2str(t1) == t2str(t2) then return true end
	end
	return false
end

local function merge(frame_list)
	local ret, idx, count = {}, 0, 1
	local tmp = nil

	for i=1,#frame_list do
		if not cmp_t(tmp, frame_list[i]) then
			idx = idx + 1
			count = 1
			tmp = frame_list[i]
		else
			count  = count + 1
		end
		ret[idx] = count
	end

	return ret
end

-- gen animation
function animation(t)
	if t.export then
		-- print('merge animation: '..t.export)
		assert(ani_table[t.export] == nil)
		ani_table[t.export] = merge(t[1])
	end
end

function print_lua_table (lua_table, indent, func)
	func = func or print
    indent = indent or 0
	for k, v in pairs(lua_table) do
		if type(k) == "string" then
			k = string.format("%q", k)
		end
		local szSuffix = ""
		if type(v) == "table" then
			szSuffix = "{"
		end
		local szPrefix = string.rep("    ", indent)
		formatting = szPrefix.."["..k.."]".." = "..szSuffix
		if type(v) == "table" then
			func(formatting)
			print_lua_table(v, indent + 1, func)
			func(szPrefix.."},")
		else
			local szValue = ""
			if type(v) == "string" then
				szValue = string.format("%q", v)
			else
				szValue = tostring(v)
			end
			func(formatting..szValue..",")
		end
	end
end


require 'characters_sc'


local  ct = {
	{name = '2001dashi', m_name = 'worker', lv_count = 1, action = {'run', 'build1', 'build2', 'idle1', 'idle2', 'idle3'}},
	-- {name = '2002jumin', lv_count = 1, action  = {'stand', 'run'}},
	--	{name = '2011dapeng', m_name = 'dragon3', lv_count = 3, action = {'stand', 'run', 'attack1','attack2', 'cheer', }},
	{name = '2003daoke', m_name ='barbarian6', lv_count = 6, action = {'stand', 'run', 'attack1','attack2',  'cheer', }},
	{name = '2004shenshe', m_name = 'archer6', lv_count = 6,  action = {'stand', 'run', 'attack1','attack2', 'cheer', }},
	{name = '2005mojin', m_name = 'goblin3', lv_count = 5,  action = {'stand', 'run', 'attack1','attack2', 'cheer', }},
	{name = '2006jiabing', m_name = 'giant6', lv_count = 6, action = {'stand', 'run', 'attack1','attack2', 'cheer', }, shadow_scale = 2},
	{name = '2007sishi', m_name = 'skeleton3', lv_count = 5, action = {'stand', 'run', 'attack1','attack2', 'cheer', }},
	{name = '2008tianzai', m_name = 'balloon3',lv_count = 6, action = {'stand', 'run', 'attack1','attack2', 'cheer', }},
	{name = '2009daoshi', m_name = 'wizard3', lv_count = 5, action = {'stand', 'run', 'attack1','attack2', 'cheer', }},
	{name = '2010shennv', m_name = 'healer2', lv_count = 3, action = {'stand', 'run', 'attack1','attack2', 'cheer', }},
	{name = '2012bingyong', m_name = 'pekka2', lv_count = 2, action = {'stand', 'run', 'attack1','attack2', 'cheer', }},
	{name = '2013baji', m_name = 'barbKing_lvl3', lv_count = 1, action = {'stand', 'run', 'attack1','attack2', 'cheer', 'reset', 'stunned'}, shadow_scale = 2},
	{name = '2014yingyan', m_name = 'archerQueen_lvl3', lv_count = 1, action = {'stand', 'run', 'attack1','attack2', 'cheer', 'reset', 'stunned'}, shadow_scale = 2},
}


local function  gen_char2_map()
	local ret = { print_lua_table = print_lua_table}
	for i,v in ipairs(ct) do
		for j=1,v.lv_count do
			for k, action in ipairs(v.action) do
				for n=1,3 do
					local mt_name = string.format('%s_%s_%d', v.m_name, action, 1)
					local t_name = string.format('%s%d_%s_%d', v.name, j, action, n)

					if ani_table[mt_name] then
						ani_table[mt_name].shadow_scale = v.shadow_scale
					end
					ret[t_name] = ani_table[mt_name]
				end
			end
		end
	end
	return ret
end

return gen_char2_map()
