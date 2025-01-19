-- Define the character
local player = {
    x = 200, -- Initial X position
    y = 300, -- Initial Y position
    width = 13, -- Player width
    height = 19, -- Player height
    speed = 1, -- Player speed in pixels per second
    yVelocity = 0, -- Vertical velocity
    jumpHeight = -600, -- Jump force
    gravity = 1600, -- Gravity force
    isOnGround = false, -- Check if the player is on the ground
    state = "idle", -- Player state
    direction = "right", -- Player direction
    animationTimer = 0, -- Timer for animations
    currentFrame = 1, -- Current animation frame
    spriteSheet = nil, -- Placeholder for the sprite sheet
    animations = {}, -- Placeholder for animations
    frameDuration = 0.5 -- Time between frames   
}

-- Define the ground level
local groundY = 400

-- Function to load resources
function love.load()
    -- Load the sprite sheet
    player.spriteSheet = love.graphics.newImage("knight.png")

    -- Define animations
    local frameWidth = 13
    local frameHeight = 19
    
    -- Idle animation
    player.animations.idle = {
        love.graphics.newQuad(0, 0, frameWidth, frameHeight, player.spriteSheet:getDimensions())
    }

    -- Running animation
    player.animations.running = {}
    for i = 1, 4 do
        player.animations.running[i] = love.graphics.newQuad((i - 1) * frameWidth, frameHeight, frameWidth, frameHeight, player.spriteSheet:getDimensions())
    end

    love.graphics.setBackgroundColor(0.2, 0.2, 0.2) -- Set background to dark gray
end

-- Function to update game logic
function love.update(dt)
    if love.keyboard.isDown("right") then
        player.x = player.x + player.speed * dt
        player.state = "running"
        player.direction = "right"
    elseif love.keyboard.isDown("left") then
        player.x = player.x - player.speed * dt
        player.state = "running"
        player.direction = "left"
    else
        player.state = "idle"
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

    -- Update animation frame for running
    if player.state == "running" then
        player.animationTimer = player.animationTimer + dt
        if player.animationTimer >= player.frameDuration then
            player.animationTimer = 0
            player.currentFrame = player.currentFrame % #player.animations.running + 1
        end
    else
        player.currentFrame = 1 -- Reset to the first frame for idle
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
    love.graphics.setColor(1, 1, 1) -- Reset color to white
    local animation = player.animations[player.state]
    local frame = animation[player.currentFrame]
    if player.direction == "right" then
        love.graphics.draw(player.spriteSheet, frame, player.x, player.y)
    else
        love.graphics.draw(player.spriteSheet, frame, player.x + player.width, player.y, 0, -1, 1)
    end
end