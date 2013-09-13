
print("checker.lua 1")

local TARGET = "found_gem_fx"
local SOURCE = "misc"

local LUA_FILE = SOURCE..".lua"

local function png_file(i)
    return SOURCE..(i+1)..".png"
end

local all = {}

local function _picture(t)
    assert(t.id)
    assert(not all[t.id], t.id)
    all[t.id] = t
    t.__type = 'pic'
end

local function _animation(t)
    assert(t.id)
    assert(not all[t.id], t.id)
    all[t.id] = t
    t.__type = 'ani'
end

local env = {
    texture = function() end,
    picture = _picture,
    animation = _animation,
}

local f = assert(loadfile(LUA_FILE,'t',env), LUA_FILE)
f()

local function extract_pic(t)
    --print("pic:", t.id)
    for i, v in ipairs(t) do
        local src_filename = SOURCE..(v.tex+1)..".png"
        local dst_filename = "output\\"..t.id.."-"..i..".png"
        --print(src_filename, v.tex, unpack(v.src))
        local cmd = ("cut.py %s %s %s"):format(
            src_filename, dst_filename,
            table.concat(v.src, " "))
        print("cmd:", cmd)
        os.execute(cmd)
    end
end

local function extract_ani(t)
    print(t.export)

    for _, c in ipairs(t.component) do
        if c.id then
            --print(c.id)

            local sub = all[c.id]
            if sub.__type == 'ani' then
                extract_ani(sub)
            else
                assert(sub.__type == 'pic')
                extract_pic(sub)
            end
        end
    end
end

for id, t in pairs(all) do
    if t.export == TARGET then
        extract_ani(t)
    end
end
