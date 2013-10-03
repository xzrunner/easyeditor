local target, tex, x, y = ...

tex = tonumber(tex)
x = tonumber(x)
y = tonumber(y)

if not (target and tex and x and y) then
    print("parameters: target tex x y")
    os.exit(1)
end

local function test_screen(v)
    local x1, x2, x3, x4 = v[1], v[3], v[5], v[7]
    local y1, y2, y3, y4 = v[2], v[4], v[6], v[8]
    local left   = math.min(x1, x2, x3, x4)
    local right  = math.max(x1, x2, x3, x4)
    local top    = math.min(y1, y2, y3, y4)
    local bottom = math.max(y1, y2, y3, y4)
    return left <=x and x <= right and top <=y and y <= bottom
end

local function test(v)
    if v.tex ~= tex then
        return false
    end

    return test_screen(v.src)
end

local env = {}
local entries = {}

function env.texture(n)
end

function env.picture(v)
    v.__type = "pic"
    entries[#entries+1] = v

    for i, piece in ipairs(v) do
        if test(piece) then
            print("-------------------- FOUND")
            print("id = "..(v.id))
            print("index = "..i)
        end
    end
end

function env.animation(v)
    v.__type = "ani"
    entries[#entries+1] = v
end

local f = assert(loadfile(target,"t",env))
f()

