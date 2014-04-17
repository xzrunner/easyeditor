local M = {}

local function _print_lua_table (lua_table, indent, func)
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
			_print_lua_table(v, indent + 1, func)
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


function M.table2str(lua_table)
	local ret = '{\n'
	_print_lua_table(lua_table, 1, function (str)
		ret = ret..str..'\n'
	end)
	return ret..'\n}\n'
end

return M