local _sf = string.format

local char_info = {
    {name="char_info_daoke", "6175-tongyong-1.png", "6163-tongyong-1.png", "6163-tongyong-2.png", "6163-tongyong-3.png"},
    {name="char_info_shenshe", "6175-tongyong-1.png", "6164-tongyong-1.png", "6164-tongyong-2.png", "6164-tongyong-3.png", "6164-tongyong-4.png"},
    {name="cha_info_mojin", "6175-tongyong-1.png", "6165-tongyong-1.PNG", "6165-tongyong-2.PNG", "6165-tongyong-3.PNG", "6165-tongyong-4.PNG"},
    {name="char_info_jiabing", "6175-tongyong-1.png", "6166-tongyong-1.PNG", "6166-tongyong-2.PNG", "6166-tongyong-3.PNG"},
    {name="char_info_sishi", "6175-tongyong-1.png", "6167-tongyong-1.PNG", "6167-tongyong-2.PNG", "6167-tongyong-3.PNG", "6167-tongyong-4.PNG"},
    {name="char_info_tianzai", "6168-tongyong-1.PNG", "6168-tongyong-2.PNG", "6168-tongyong-3.PNG", "6168-tongyong-4.PNG", "6168-tongyong-5.PNG"},
    {name="char_info_daoshi", "6175-tongyong-1.png", "6169-tongyong-1.PNG", "6169-tongyong-2.PNG",},
    {name="char_info_shennv", "6170-tongyong-1.PNG", "6170-tongyong-2.PNG", "6170-tongyong-3.PNG"},
    {name="char_info_dapeng", "6171-tongyong-1.PNG", "6171-tongyong-2.PNG", "6171-tongyong-3.PNG"},
    {name="char_info_xueying", "6196-tongyong-1.PNG", "6196-tongyong-2.PNG", "6196-tongyong-3.PNG", "6196-tongyong-4.PNG"},
    {name="char_info_baji", "6175-tongyong-1.png", "6173-tongyong-1.PNG", "6173-tongyong-2.PNG", "6173-tongyong-3.PNG", "6173-tongyong-4.PNG", "6173-tongyong-5.PNG", "6173-tongyong-6.PNG", "6173-tongyong-7.PNG"}, 
    {name="char_info_yingyan", "6175-tongyong-1.png", "6174-tongyong-1.PNG", "6174-tongyong-2.PNG", "6174-tongyong-3.PNG", "6174-tongyong-4.PNG", "6174-tongyong-5.PNG"},
}



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
            "name" : "",
            "position" : 
            {
                "x" : 0.0,
                "y" : 0.0
            },
            "x mirror" : false,
            "x scale" : 0.7,
            "x shear" : 0.0,
            "y mirror" : false,
            "y scale" : 0.7,
            "y shear" : 0.0
        }]]


local ani_body_format = [[
{
    "fps" : 30,
    "layer" : 
    [
        
        {
            "frame" : 
            [
%s
            ],
            "name" : "Layer0"
        }
    ],
    "name" : "char_info"
}
]]


local ani_frame_format = [[
                {
                    "actor" : 
                    [
                        
                        {
                            "add color" : "",
                            "angle" : 0.0,
                            "filepath" : "%s_complex.json",
                            "multi color" : "",
                            "name" : "",
                            "position" : 
                            {
                                "x" : 0.0,
                                "y" : 0.0
                            },
                            "x mirror" : false,
                            "x scale" : 1.0,
                            "x shear" : 0.0,
                            "y mirror" : false,
                            "y scale" : 1.0,
                            "y shear" : 0.0
                        }
                    ],
                    "id" : 0,
                    "time" : %d,
                    "tween" : false
                }]]



local function _gen(name, info)
    local ret = ""
    for i=1,#info do
        ret = ret.._sf(sprit_format, info[i])
        if i ~= #info then
            ret = ret..",\n"
        end    
    end

    return _sf(body_format, name, ret)
end

local function _write_file(file, str)
    local fd = io.open(file, "w")
    fd:write(str)
    fd:close()  
end




-- gen complex file
local function _gen_com()
    for i=1,#char_info do
        local name, info = char_info[i].name, char_info[i]
        local ret = _gen(name, info)
        _write_file(_sf("../sg_ui/%s_complex.json", name), ret)
    end
end


-- gen animation file
local function _gen_ani()
    local ret = ""
    for i=1,#char_info do
        local name = char_info[i].name
        ret = ret.._sf(ani_frame_format, name, i)
        if i ~= #char_info then
            ret = ret..",\n"
        end
    end
    local str = _sf(ani_body_format, ret)
    _write_file("../sg_ui/char_info_anim.json", str) 
end


_gen_com()

_gen_ani()





