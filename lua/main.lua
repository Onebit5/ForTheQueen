-- Define the character
local player = {
    x = 200, -- Initial X position
    y = 300, -- Initial Y position
    width = 50, -- Player width
    height = 100, -- Player height
    speed = 400, -- Player speed in pixels per second
    yVelocity = 0, -- Vertical velocity
    jumpHeight = -600, -- Jump force
    gravity = 1600, -- Gravity force
    isOnGround = false -- Check if the player is on the ground
}

-- Define the ground level
local groundY = 400

-- Function to load resources
function love.load()
    love.graphics.setBackgroundColor(0.2, 0.2, 0.2) -- Set background to dark gray
end

-- Function to update game logic
function love.update(dt)
    if love.keyboard.isDown("right") then
        player.x = player.x + player.speed * dt
    elseif love.keyboard.isDown("left") then
        player.x = player.x - player.speed * dt
    end

    -- Apply gravity
    player.yVelocity = player.yVelocity + player.gravity * dt

    -- Update the player's position
    player.y = player.y + player.yVelocity * dt

    -- Check if the player is on the ground
    if player.y + player.height >= groundY then
        player.y = groundY - player.height -- Place the player on the ground
        player.yVelocity = 0 -- Stop vertical movement
        player.isOnGround = true -- Player is on the ground
    else
        player.isOnGround = false -- Player is in the air
    end
end

-- Function to handle key presses
function love.keypressed(key)
    if key == "up" and player.isOnGround then
        player.yVelocity = player.jumpHeight -- Apply jump force
        player.isOnGround = false -- Player is no longer on the ground
    end
end

-- Function to draw graphics (called every frame)
function love.draw()
    -- Draw the ground
    love.graphics.setColor(0.4, 0.4, 0.4) -- Set ground color
    love.graphics.rectangle("fill", 0, groundY, love.graphics.getWidth(), love.graphics.getHeight() - groundY)

    -- Draw the player
    love.graphics.setColor(1, 1, 1) -- Set player color
    love.graphics.rectangle("fill", player.x, player.y, player.width, player.height)
end