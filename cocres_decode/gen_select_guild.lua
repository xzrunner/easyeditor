local body_format = [[
{
    "name" : "select_guild",
    "redo" : null,
    "sprite" : 
    [
        
        {
            "add color" : "",
            "angle" : 0.0,
            "filepath" : "pop_action_bg_complex.json",
            "multi color" : "",
            "name" : "",
            "position" : 
            {
                "x" : 0.0,
                "y" : 0.0
            },
            "x mirror" : false,
            "x scale" : 0.8,
            "x shear" : 0.0,
            "y mirror" : false,
            "y scale" : 0.8,
            "y shear" : 0.0
        },
%s
    ]
}
]]

local frame_format = [[
        {
            "add color" : "",
            "angle" : 0.0,
            "filepath" : "guild%d_complex.json",
            "multi color" : "",
            "name" : "guild%d",
            "position" : 
            {
                "x" : %d,
                "y" : %d
            },
            "x mirror" : false,
            "x scale" : 1.0,
            "x shear" : 0.0,
            "y mirror" : false,
            "y scale" : 1.0,
            "y shear" : 0.0
        }
]]

local _x = -250
local _y = 140
local function _gen_data()
    local _str = ""
    local _count = 1
    for i=1,5 do
        local y = _y - (i-1)*65
        for j=1,10 do
            local x = _x + (j-1)*55
            _str = _str..string.format(frame_format, _count, _count, x, y)
            _count = _count + 1
            if not (i==5 and j==10) then
                _str = _str..','
            end
        end
    end
    local ret = body_format:format(_str)
    return ret
end

local function _write_file(file, str)
    local fd = io.open(file, "w")
    fd:write(str)
    fd:close()  
end

_write_file('../sg_ui/select_guild_complex.json', _gen_data())




