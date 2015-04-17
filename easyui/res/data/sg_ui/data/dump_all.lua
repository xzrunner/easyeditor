local ljson = require "json"
local lfs = require "lfs"
local lpeg = require "lpeg"
local sep = string.match(package.config, "[^\n]+")


local _patt =lpeg.P("_complex.json") + lpeg.P("_anim.json")
_patt = (1 - _patt)^1 * _patt
local function _check_file(file)
    return lpeg.match( _patt, file)
end

local function gen_export_name(file)
    local handle = io.open(file, "r")
    local ret = ljson.decode(handle:read("*a"))
    handle:close()
    if ret and ret.name and ret.name ~= "" then
        return ret.name
    else
        return nil
    end
end


local function gen_name(dir)
    local map = {}
    local ret = {}
    for file in lfs.dir(dir) do
        if file ~= "." and file ~= ".." then
            local f = dir..sep..file
            local attr = lfs.attributes(f)
            if attr.mode == "file" and _check_file(f) then
                local name = gen_export_name(f)
                if name then
                    if map[name] then
                        error("file_name:"..name.." "..map[name].." | "..f)
                    end
                    ret[#ret+1] = name
                    map[name] = f
                end
            end
        end
    end
    return ret
end

local ret = gen_name("./json")
for i=1,#ret do
    print(ret[i])
end


-- print(_check_file("./json/guild_search_item_complex.json"))