
local body_format = [[
{
    "name" : "",
    "sprite" : 
    [
%s
    ]
}
]]

local br_icon_format = [[
        {
            "add color" : "",
            "angle" : 0.0,
            "filepath" : "br_unit_complex.json",
            "multi color" : "",
            "name" : "slot_%d",
            "position" : 
            {
                "x" : %d,
                "y" : 0.0
            },
            "x mirror" : false,
            "x scale" : 0.3000000119209290,
            "x shear" : 0.0,
            "y mirror" : false,
            "y scale" : 0.3000000119209290,
            "y shear" : 0.0
        }]]

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
local function _gen_icon(format, i, x)
    local ret = _sf(format, i,  x)
    return ret
end

local function _gen(format, len)
    local ret = ""
    for i=1,len do
        ret = ret.._gen_icon(format, i, (i-1)*58)
        if i ~= len then
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


-- _write_file("../sg_ui/troop_info_list_complex.json", _gen(icon_format, 10))

_write_file("../sg_ui/br_troop_complex.json", _gen(br_icon_format, 15))
