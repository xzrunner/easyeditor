
local M = {}

local body_format = [[
{
    "name" : "%s",
    "sprite" : 
    [
%s
    ]
}
]]


local sprit_format = [[
        {
            "add color" : "",
            "angle" : 0.0,
            "filepath" : "%s",
            "multi color" : "",
            "name" : "%s",
            "position" : 
            {
                "x" : %d,
                "y" : %d
            },
            "x mirror" : false,
            "x scale" : 1,
            "x shear" : 0.0,
            "y mirror" : false,
            "y scale" : 1,
            "y shear" : 0.0
        }]]


local function _gen_sprite(count, prefix_name, complex_file, x_space, y_space)
    local ret = ""
    for i=1, count do
        ret = ret..string.format(sprit_format, complex_file, prefix_name..(i), (i-1)*x_space, -(i-1)*y_space)
        if i~=count then ret = ret.."," end
    end
    return ret
end

function M.gen(export_name, count, prefix_name, complex_file, x_space, y_space)
    return string.format(body_format, export_name, _gen_sprite(count, prefix_name, complex_file, x_space, y_space))
end

function M.writefile(file, str)
    local fd = io.open(file, "w")
    fd:write(str)
    fd:close()  
end

return M

