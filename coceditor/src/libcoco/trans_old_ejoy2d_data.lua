static const char* trans_old_ejoy2d_data = STRINGIFY(

local all = {}

local function picture(tbl)
  tbl["type"] = "picture"
  table.insert(all, tbl)
end

local function animation(tbl)
  tbl["type"] = "animation"
  table.insert(all, tbl)
end

local function texture(n)
end

local _env = _ENV
_env.picture = picture
_env.animation = animation
_env.texture = texture

function trans2table(filename)
  f = assert(loadfile(filename , "t", _env))
  f()
  return all
end

);