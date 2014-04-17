
local ex2id = {}
local s2id = {}

local _animation = animation
local idx = 0

local function animation(t)
  if t.export then
      ex2id[t.export] = t.id
  end

  _animation(t)
end

local function export2id(export)
  local id = ex2id[export]
  assert(id, export)
  return id
end

local function alloc_id(str)
  if s2id[str] then
    error("alloc2_id str: "..str.."is realloc.")
  else
    s2id[str] = 8527+idx
    idx = idx+1
    return s2id[str]
  end
end

local function get_id(str)
  if s2id[str] then
    return s2id[str]
  else
    error("the "..str.." is not id.")
  end 
end


