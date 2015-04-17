require "d2d.stage"

local selected = d2d.stage.getSelectedSprites()
local last = selected:get(0)
for i=1, 39 do
  local clone = last:clone()
  local offset = clone:height()
  local offx = 0
  local offy = 0
  if i % 2 == 0 then
    offx = 103
    offy = 136
  else
    offx = 0
    offy = -136
  end
  clone:move(offx, offy)
  clone:setName("item_"..tostring(i+1))
  d2d.stage.insert(clone)
  last = clone
end
