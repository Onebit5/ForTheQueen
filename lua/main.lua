-- main.lua

-- Define the character
local player = {
    x = 200, -- Initial X position
    y = 300, -- Initial Y position
    width = 50, -- Player width
    height = 100, -- Player height
    speed = 200 -- Player speed in pixels per second
}

-- Function to load resources
function love.load()
    love.graphics.setBackgroundColor(0.2, 0.2, 0.2) -- Set background to dark gray
end

-- Function to update game logic
function love.update(dt)
    if love.keyboard.isDown("right") then
        player.x = player.x + player.speed * dt
    elseif love.keyboard.isDown("left") then
        player.y = player.y + player.speed * dt
    end
end

-- Function to draw graphics
function love.draw()
    love.graphics.setColor(1, 1, 1) -- Set color to white
    love.graphics.rectangle("fill", player.x, player.y, player.width, player.height)
end