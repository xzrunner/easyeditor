local README = [[
lua package_ui.lua  <pattern> [target]
    <pattern>:
        -load: 仅仅是进行打包生成epp epd文件
        -pack: 生成贴图以及打包成epp epd文件
    [target]:
        common: 默认为此选项, 仅仅是打包common目录下的png 和json
        360: qihoo target
        efun: efun target
]]

local texture_pack = require "texture_pack"
local json = require "ljson"

local join = texture_pack.join
local del_dir = texture_pack.del_dir
local del_file = texture_pack.del_file
local mkdir = texture_pack.mkdir
local tmp_dir = texture_pack.tmp_dir
local run_cmd = texture_pack.run_cmd
local check_have_file = texture_pack.check_have_file


local pattern, target = ...
local target = target or "common"

local epbin = "..\\tools\\lua\\epbin.lua"
local lua = "..\\tools\\lua\\lua52.exe"
local python = "..\\tools\\python27\\python.exe"

local _path_target = {
    ["common"] = "..\\sg_ui\\common",
    ["360"] = "..\\sg_ui\\360",
    ["efun"] = "..\\sg_ui\\efun"
}


--  创建打包环境 type: json/png/all  默认为all
local function _get_target_env(target, type)
    type = type or "all"
    local common_path = _path_target.common
    local target_path = _path_target[target]
    assert(target_path)

    local c_png = common_path.."\\png"
    local c_json = common_path.."\\json"
    local t_png = target_path.."\\png"
    local t_json = target_path.."\\json"

    local ret = {
        png_path = c_png,
        json_path = c_json,
        target_path = "ui_output\\"..target
    }

    -- 检查target文件中是否有png
    if common_path~=target_path and (type=="all" or type=="png") and check_have_file(t_png) then
        tmp_dir('tmp_png')
        run_cmd( ('COPY /Y %s\\*.png "tmp_png\\" '):format(c_png))
        run_cmd( ('COPY /Y %s\\*.png "tmp_png\\" '):format(t_png))
        ret.png_path = 'tmp_png'
    end

    -- 检查target文件是否有json
    if common_path~=target_path and (type=="all" or type=="json") and check_have_file(t_json) then
        tmp_dir('tmp_json')
        run_cmd( ('COPY /Y %s\\*.json "tmp_json\\" '):format(c_json))
        run_cmd( ('COPY /Y %s\\*.json "tmp_json\\" '):format(t_json))
        ret.json_path = 'tmp_json'
    end

    return ret
end

-- 清理打包环境
local function _clean_target_env(target)
    del_dir('tmp_png')
    del_dir('tmp_json')
end

local function _json2table(file)
    local handle = io.open(file)
    local str = handle:read("*a")
    handle:close()
    local success, table =  json.decode(str)
    assert(success)
    return table
end

---------------------- pack ----------------------
local function _gen_pack(target_env)
    local target_path = target_env.target_path
    local png_path = target_env.png_path
    mkdir(target_path)
    return texture_pack.pack(png_path, target_path.."\\ui2")
end

-- 获得五边形能力图的6450-gaoguang-1.png 所在贴图位置
local function _get_pentagon_src(json_file)
    local data = _json2table(json_file).frames
    for i=1,#data do
        if data[i].filename == "6450-gaoguang-1.png" then
            local frame = data[i].frame
            return {
                frame.x+2, 
                frame.y+frame.h-2, 
                frame.x+frame.w-2,
                frame.y+frame.h-2,
                frame.x+frame.w-2,
                frame.y+2,
                frame.x+2,
                frame.y+2
            }
        end
    end
    return false
end

local function _get_pentagon(target_env)
    for i=1,2 do
        local path = target_env.target_path.."\\ui2"..(i)..".json"
        local src = _get_pentagon_src(path)
        if src then
            local src_pos = (('"%d,%d, %d,%d, %d,%d, %d,%d"'):format(
                src[1], src[2],
                src[3], src[4],
                src[5], src[6],
                src[7], src[8]))
            return src_pos, tostring(i-1)
        end
    end
    assert(false)
end

-- 生成角色和英雄的五边形信息
local function _gen_pentagon(target_env)
    local src_pos, tex = _get_pentagon(target_env)
    
    -- gen char
    run_cmd(join(
        python, 
        "../pentagon_import/gen_pentagon.py",
        src_pos, tex, 
        "../pentagon_import/character.csv ../pentagon_import/pt_char.lua 19",
        "10527"))
    
    -- gen hero
    run_cmd(join(
        python,
        "../pentagon_import/gen_pentagon.py",
        src_pos, tex,
        "../pentagon_import/hero.csv ../pentagon_import/pt_hero.lua 7",
        "11527"))
end

-- 合并五边形信息和
local function _gen_merge(target_env)
    local ui2_ori_lua = target_env.target_path.."\\ui2.ori.lua"
    run_cmd("type ..\\pentagon_import\\pt_char.lua >> "..ui2_ori_lua)
    run_cmd("type ..\\pentagon_import\\pt_hero.lua >> "..ui2_ori_lua)
end

local function _gen_load(target_env)
    local target_path = target_env.target_path
    local json_path = target_env.json_path
    mkdir(target_path)
    
    local cmd = join(
        "cocpackage_load.exe",
        json_path, 
        target_path.."\\ui2",
        target_path.."\\ui2.ori.lua"
    )
    run_cmd(cmd)
end

--- 生成EP 文件
local function _gen_ep(target_env)
    local ui2_conver = ".\\ui_output\\ui2.conver.lua"
    local ui2_manual = ".\\ui_output\\ui2.manual.lua"
    local ui2_ori_lua = target_env.target_path.."\\ui2.ori.lua"
    local ui2_lua = target_env.target_path.."\\ui2.lua"

    local ui2 = target_env.target_path.."\\ui2"
    local t_ios = target_env.target_path.."\\ui2.ios"
    local t_android = target_env.target_path.."\\ui2.android"
    
    del_file(ui2_lua)
    
    run_cmd(("type %s >> %s"):format(ui2_conver, ui2_lua))
    run_cmd(("type %s >> %s"):format(ui2_ori_lua, ui2_lua))
    run_cmd(("type %s >> %s"):format(ui2_manual, ui2_lua))
    
    print("---------gen ios ep file-------------")
    run_cmd(("%s %s -pd -png8 %s %s.ios"):format(lua, epbin, ui2_lua, ui2))
    print("---------gen android ep file---------")
    -- fixed me!!!
    run_cmd(("COPY /Y .\\ui_output\\*.pkm %s"):format(target_env.target_path))
    run_cmd(("%s %s -pd -pkmc %s %s.android"):format(lua, epbin, ui2_lua, ui2))
end





----------------------run --------------------------
if not _path_target[target] then
    print(README)
    return
end

if pattern == "-load" then
    local env = _get_target_env(target, "json")
    _gen_pentagon(env)
    _gen_load(env)
    _gen_merge(env)
    _gen_ep(env)
    _clean_target_env(env)
    return
elseif pattern == "-pack" then
    local env = _get_target_env(target)
    _gen_pack(env)
    _gen_pentagon(env)
    _gen_load(env)
    _gen_merge(env)
    _gen_ep(env)
    _clean_target_env(env)
    return
end

print(README)