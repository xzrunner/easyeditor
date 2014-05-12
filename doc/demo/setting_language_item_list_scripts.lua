require "d2d.stage"

local selected = d2d.stage.getSelectedSprites()
local last = selected:get(0)
for i=1, 40 do
  local clone = last:clone()
  local offset = clone:height()
  clone:move(0, -offset)
  d2d.stage.insert(clone)
  last = clone
end
