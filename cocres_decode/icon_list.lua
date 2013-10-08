
local body_format = [[
{
    "name" : "",
    "sprite" : 
    [
%s
    ]
}
]]

local icon_format = [[
        {
            "add color" : "",
            "angle" : 0.0,
            "filepath" : "character_info_icon_complex.json",
            "multi color" : "",
            "name" : "unit_slot_%d",
            "position" : 
            {
                "x" : %d,
                "y" : 0.0
            },
            "x mirror" : false,
            "x scale" : 1.0,
            "x shear" : 0.0,
            "y mirror" : false,
            "y scale" : 1.0,
            "y shear" : 0.0
        }]]


local _sf = string.format
local function _gen_icon(i, x)
    local ret = _sf(icon_format, i,  x)
    return ret
end

local function _gen()
    local ret = ""
    for i=1,10 do
        ret = ret.._gen_icon(i, (i-1)*58)
        if i ~= 10 then
            ret = ret..",\n"
        end
    end

    return _sf(body_format, ret)
end

local function _write_file(file, str)
    local fd = io.open(file, "w")
    fd:write(str)
    fd:close()  
end


_write_file("../sg_ui/troop_info_list_complex.json", _gen())
