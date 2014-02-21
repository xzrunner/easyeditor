local ej = require "ejoy2d"
local fw = require "ejoy2d.framework"
local pack = require "ejoy2d.simplepackage"

pack.load {
	pattern = fw.WorkDir..[[_tmp_ejoy2d_preview/?]],
	"tmp",
}

local obj = ej.sprite("tmp","root")
obj:ps(-100,0,0.5)

local game = {}
local screencoord = { x = 512, y = 384, scale = 1.2 }

function game.update()
end

function game.drawframe()
	obj:draw(screencoord)
end

function game.touch(what, x, y)
end

ej.start(game)


