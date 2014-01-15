local body_format = [[
{
    "name" : "guild%d",
    "redo" : null,
    "sprite" : 
    [
        
        {
            "add color" : "",
            "angle" : 0.0,
            "filepath" : "guild_mark_%d.png",
            "multi color" : "",
            "name" : "",
            "position" : 
            {
                "x" : 0.0,
                "y" : 0.0
            },
            "x mirror" : false,
            "x scale" : 0.6000000238418579,
            "x shear" : 0.0,
            "y mirror" : false,
            "y scale" : 0.6000000238418579,
            "y shear" : 0.0
        }
    ]
}
]]

local function _write_file(file, str)
    local fd = io.open(file, "w")
    fd:write(str)
    fd:close()  
end


for i=1, 50 do
    local str = string.format(body_format, i, i)
    local file ='../sg_ui/guild'..(i)..'_complex.json'
    _write_file( file, str)
end
