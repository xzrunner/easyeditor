
local SRC, DST = ...
local SCALE = 0.625

local env = {}
local tex = 0
local entries = {}

function env.texture(n)
    tex = n
end

local function convert_screen(v)
    for i=1,8 do
        v[i] = v[i] * SCALE
    end
    --local x1, x2, x3, x4 = v[1], v[3], v[5], v[7]
    --local y1, y2, y3, y4 = v[2], v[4], v[6], v[8]
    --local left   = math.min(x1, x2, x3, x4)
    --local right  = math.max(x1, x2, x3, x4)
    --local top    = math.min(y1, y2, y3, y4)
    --local bottom = math.max(y1, y2, y3, y4)
    --local cx = (left + right) / 2
    --local cy = (top + bottom) / 2
    --for i = 1,7,2 do
    --    v[i] = cx + (v[i] - cx) * SCALE
    --end
    --for i = 2,8,2 do
    --    v[i] = cy + (v[i] - cy) * SCALE
    --end
end

local function convert_matrix(v)
    v[5] = v[5] * SCALE
    v[6] = v[6] * SCALE
end

function env.picture(v)
    v.__type = "pic"
    entries[#entries+1] = v

    for _, piece in ipairs(v) do
        convert_screen(piece.screen)
    end
end

function env.animation(v)
    v.__type = "ani"
    entries[#entries+1] = v

    for _, action in ipairs(v) do
        for _, frame in ipairs(action) do
            for _, part in ipairs(frame) do
                if part.mat then
                    convert_matrix(part.mat)
                end
            end
        end
    end
end

local f = assert(loadfile(SRC,"t",env))
f()

local o = io.open(DST,"wb")
local t = ''

local function w(v, ...)
    o:write(t)
    o:write(v:format(...))
    o:write("\n")
end

local function t_inc()
    t = t .. '  '
end

local function t_dec()
    t = t:sub(1, -3)
end

local function wpic_piece(v)
    w("{ tex=%d, src={ %d,%d, %d,%d, %d,%d, %d,%d }, screen={ %d,%d, %d,%d, %d,%d, %d,%d } },",
    v.tex,
    v.src[1],v.src[2],v.src[3],v.src[4],v.src[5],v.src[6],v.src[7],v.src[8],
    v.screen[1],v.screen[2],v.screen[3],v.screen[4],v.screen[5],v.screen[6],v.screen[7],v.screen[8]
    )
end

local function wpic(v)
    w("picture {")
    t_inc()
        w("id = %d,", v.id)
        for _, piece in ipairs(v) do
            wpic_piece(piece)
        end
    t_dec()
    w("}")
end

local function dump(v)
    local t = type(v)
    if t == "number" then
        return tostring(v)
    elseif t == "string" then
        return string.format("%q", v)
    elseif t == "table" then
        local data = {}
        for i, n in ipairs(v) do
            data[i] = dump(n)
        end
        return "{"..table.concat(data, ",").."}"
    else
        error(t)
    end
end

local function wani_component(v)
    local data = {}
    for k, v in pairs(v) do
        data[#data+1] = ("%s = %s"):format(k, dump(v))
    end
    w("{ %s },", table.concat(data, ","))
end

local function wframe_part(v)
    local data = {}
    for k, v in pairs(v) do
        data[#data+1] = ("%s = %s"):format(k, dump(v))
    end
    w("{ %s },", table.concat(data, ","))
end

local function wframe(v)
    w("{")
    t_inc()
        for _, f in ipairs(v) do
            wframe_part(f)
        end
    t_dec()
    w("},")
end

local function waction(v)
    w("{")
    t_inc()
        for _, f in ipairs(v) do
            wframe(f)
        end
    t_dec()
    w("},")
end

local function wani(v)
    w("animation {")
    t_inc()
        if v.export then
            w('export = "%s",', v.export)
        end

        w("id = %d,", v.id)

        w("component = {")
        t_inc()
            for _, c in ipairs(v.component) do
                wani_component(c)
            end
        t_dec()
        w("},")

        for _, a in ipairs(v) do
            waction(a)
        end
    t_dec()
    w("}")
end

w("texture(%d)", tex)

entry_func = {
    pic = wpic,
    ani = wani,
}

for _, v in ipairs(entries) do
    entry_func[v.__type](v)
end
