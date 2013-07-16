
local input, output = ...

print("input:", input)
print("output:", output)

local inputf = loadfile(input)
print("inputf:", inputf)
local data = inputf()
print("data:", data)
print(data.sheet)
print(data.frameIndex)

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

out("frames = [")
for i, entry in ipairs(data.sheet.frames) do
    out("{", 1)
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
local all_ids = {}
local id_name_map = {}
for name, id in pairs(data.frameIndex) do
    table.insert(all_ids, id)
    id_name_map[id] = name
end
table.sort(all_ids)
for _, id in ipairs(all_ids) do
    local name = assert(id_name_map[id])
    out(('"%s": %d,'):format(name, id-1), 1)
end
out("}")

outputf:close()
