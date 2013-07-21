
local args = {...}
assert(#args >= 2)

local output = args[1]
print("output:", output)

local frames = {}
local name_history = {}
local id_history = {}
local all_ids = {}
local id_name_map = {}

local nf = 0
local frame_base = 0
local ni = 0
local ntex = #args - 1

for i=2, #args do
    local input = args[i]
    print("input:", input)

    local inputf = loadfile(input)
    print("inputf:", inputf)
    local data = inputf()
    print("data:", data)
    print(data.sheet)
    print(data.frameIndex)

    for _, entry in ipairs(data.sheet.frames) do
        entry.tex = i-2 -- base-0
        nf = nf + 1; frames[nf] = entry
    end

    for name, pack_id in pairs(data.frameIndex) do
        local id = frame_base + pack_id
        -- print("name:", name)
        -- print("id:", frame_base, pack_id, id)
        assert(not name_history[name])
        assert(not id_history[id])

        name_history[name] = true
        id_history[id] = true

        ni = ni+1; all_ids[ni] = id
        id_name_map[id] = name
    end

    frame_base = nf
    print("frame_base:", frame_base)
end

local outputf = io.open(output, "w")
assert(outputf)

local function out(s, indent)
    indent = indent or 0
    local prefix = string.rep("\t", indent)
    outputf:write(prefix..s.."\n")
end

local function out_field(t, k, indent)
    out(('"%s": %d,'):format(k, t[k]), indent)
end

out("tex_count = "..tostring(ntex))
out("")
out("frames = [")
for _, entry in ipairs(frames) do
    out("{", 1)
    out_field(entry, "tex", 2)
    out_field(entry, "x", 2)
    out_field(entry, "y", 2)
    out_field(entry, "width", 2)
    out_field(entry, "height", 2)
    out_field(entry, "sourceX", 2)
    out_field(entry, "sourceY", 2)
    out_field(entry, "sourceWidth", 2)
    out_field(entry, "sourceHeight", 2)
    out("},", 1)
end
out("]")
out("")
out("indexes = {")
table.sort(all_ids)
for _, id in ipairs(all_ids) do
    local name = assert(id_name_map[id])
    out(('"%s": %d,'):format(name, id-1), 1)
end
out("}")

outputf:close()
