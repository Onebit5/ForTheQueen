-- Define the character
local player = {
    x = 162, -- Initial X position
    y = 572, -- Initial Y position
    width = 13, -- Player width
    height = 19, -- Player height
    speed = 400, -- Player speed in pixels per second
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
    frameDuration = 0.1 -- Time between frames
}

-- Define the level structure
local level = {
    image = nil, -- Placeholder for the level image
    platforms = {}, -- List of platform collision boxes
}

-- Function to load resources
function love.load()
    -- Load the sprite sheet
    player.spriteSheet = love.graphics.newImage("knight.png")

    loadLevel("Level0.png", {
        {x = 150, y = 593, width = 990, height = 10, type = "Rigidbody"},
        {x = 150, y = 529, width = 825, height = 14, type = "Rigidbody"},
        {x = 150, y = 0, width = 10, height = 640, type = "Rigidbody"},
        {x = 1120, y = 0, width = 10, height = 640, type = "Rigidbody"},
        {x = 150, y = 466, width = 123, height = 14, type = "Rigidbody"},
        {x = 304, y = 466, width = 223, height = 14, type = "Rigidbody"},
        {x = 498, y = 433, width = 30, height = 40, type = "Rigidbody"},
        {x = 545, y = 402, width = 60, height = 77, type = "Rigidbody"},
        {x = 657, y = 370, width = 93, height = 109, type = "Rigidbody"},
        {x = 747, y = 370, width = 450, height = 14, type = "Rigidbody"},
        {x = 657, y = 275, width = 31, height = 14, type = "Rigidbody"},
        {x = 447, y = 275, width = 81, height = 14, type = "Rigidbody"},
        {x = 769, y = 275, width = 13, height = 14, type = "Rigidbody"},
        {x = 863, y = 275, width = 31, height = 14, type = "Rigidbody"},
        {x = 943, y = 275, width = 31, height = 14, type = "Rigidbody"},
        {x = 594, y = 259, width = 13, height = 14, type = "Rigidbody"},
        {x = 992, y = 306, width = 81, height = 14, type = "Rigidbody"},
        {x = 481, y = 178, width = 13, height = 14, type = "Rigidbody"},
        {x = 481, y = 66, width = 13, height = 14, type = "Rigidbody"},
        {x = 498, y = 83, width = 13, height = 14, type = "Rigidbody"},
        {x = 514, y = 128, width = 13, height = 14, type = "Rigidbody"},
        {x = 528, y = 162, width = 13, height = 14, type = "Rigidbody"},
        {x = 545, y = 145, width = 13, height = 14, type = "Rigidbody"},
    })

    -- Define animations
    local frameWidth = 13
    local frameHeight = 19
    
    -- Idle animation
    player.animations.idle = {
        love.graphics.newQuad(0, 0, frameWidth, frameHeight, player.spriteSheet:getDimensions())
    }

    -- Running animation
    player.animations.running = {}
    for i = 1, 2 do
        player.animations.running[i] = love.graphics.newQuad((i - 1) * frameWidth, frameHeight, frameWidth, frameHeight, player.spriteSheet:getDimensions())
    end

    player.collider = {
        x = player.x,
        y = player.y,
        width = player.width,
        height = player.height
    }

    love.graphics.setBackgroundColor(0.2, 0.6, 1, 0.4) -- Set background to dark gray
end

-- Check for collisions between the player and platforms
function checkCollision(a, b)
    return a.x < b.x + b.width and
           a.x + a.width > b.x and
           a.y < b.y + b.height and
           a.y + a.height > b.y
end

-- Resolve collisions
function resolveCollisions()
    player.isOnGround = false -- Reset ground state each frame

    for _, platform in ipairs(level.platforms) do
        if platform.type == "Rigidbody" and checkCollision(player.collider, platform) then
            -- Calculate overlaps
            local overlapTop = (player.collider.y + player.collider.height) - platform.y
            local overlapBottom = platform.y + platform.height - player.collider.y
            local overlapLeft = (player.collider.x + player.collider.width) - platform.x
            local overlapRight = platform.x + platform.width - player.collider.x

            -- Determine the smallest overlap
            local smallestOverlap = math.min(overlapTop, overlapBottom, overlapLeft, overlapRight)

            -- Resolve collision based on direction
            if smallestOverlap == overlapTop and player.yVelocity > 0 then
                -- Colliding from above
                player.y = platform.y - player.height
                player.yVelocity = 0
                player.isOnGround = true
            elseif smallestOverlap == overlapBottom and player.yVelocity < 0 then
                -- Colliding from below
                player.y = platform.y + platform.height
                player.yVelocity = 0
            elseif smallestOverlap == overlapLeft then
                -- Colliding from the left
                player.x = platform.x - player.width
            elseif smallestOverlap == overlapRight then
                -- Colliding from the right
                player.x = platform.x + platform.width
            end
        end
    end
end

-- Load the level
function loadLevel(imagePath, platforms)
    level.image = love.graphics.newImage(imagePath)
    level.platforms = platforms
end

-- Update game logic
function love.update(dt)
    -- Apply gravity
    player.yVelocity = player.yVelocity + player.gravity * dt

    -- Update the player's position
    player.y = player.y + player.yVelocity * dt

    -- Sync collider with player position
    player.collider = {
        x = player.x,
        y = player.y,
        width = player.width,
        height = player.height
    }

    -- Resolve collisions
    resolveCollisions()

    -- Player movement
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

    -- Update animation frame
    if player.state == "running" then
        player.animationTimer = player.animationTimer + dt
        if player.animationTimer >= player.frameDuration then
            player.animationTimer = 0
            player.currentFrame = player.currentFrame % #player.animations.running + 1
        end
    else
        player.currentFrame = 1
    end
end

-- Handle key presses
function love.keypressed(key)
    if key == "space" and player.isOnGround then
        player.yVelocity = player.jumpHeight -- Apply jump force
        player.isOnGround = false
    end
end

-- Draw everything
function love.draw()
    -- Draw the level
    love.graphics.setColor(1, 1, 1)
    love.graphics.draw(level.image, 0, 0)

    -- Draw the player
    local animation = player.animations[player.state]
    local frame = animation[player.currentFrame]
    if player.direction == "right" then
        love.graphics.draw(player.spriteSheet, frame, player.x, player.y)
    else
        love.graphics.draw(player.spriteSheet, frame, player.x + player.width, player.y, 0, -1, 1)
    end

    -- Debug: Draw platforms
    for _, platform in ipairs(level.platforms) do
        love.graphics.setColor(1, 0, 0, 0.5)
        love.graphics.rectangle("fill", platform.x, platform.y, platform.width, platform.height)
    end

    -- Debug: Draw player collider
    love.graphics.setColor(0, 0, 1, 0.5)
    love.graphics.rectangle("fill", player.collider.x, player.collider.y, player.collider.width, player.collider.height)
end