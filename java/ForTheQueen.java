
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.HashSet;
import java.util.Set;
import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class ForTheQueen extends JPanel implements KeyListener {

    private levelManager lvlManager;

    private int playerX = 200; // Initial X position of the player
    private int playerY = 500; // Initial Y position of the player
    private final int PLAYER_WIDTH = 13;
    private final int PLAYER_HEIGHT = 19;
    private int MOVE_SPEED = 13;

    private float verticalVelocity = 0; // Vertical velocity for jumping
    private final float GRAVITY = .5f; // Gravity pulling the player down
    private float JUMP_STRENGTH = -10; // Jump strength

    private boolean isJumping = false; // Boolean to know if the player is jumping or not
    private boolean isMovingLeft = false;
    private boolean isMovingRight = false;
    private boolean facingLeft = false;

    // A set to track which keys are currently pressed
    private final Set<Integer> keysPressed = new HashSet<>();

    // Sprite sheet and animations
    private BufferedImage spriteSheet;
    private BufferedImage[] idleFrames;
    private BufferedImage[] runningFrames;
    private int animationFrame = 0; // Current frame of animation
    private int animationSpeed = 10; // Lower = faster animation
    private int animationCounter = 0; // Timer for switching frames

    // Time variables
    private int totalTime = 10; // Total time in seconds
    private int timeLeft = totalTime; // Time left in seconds
    private JSlider timeSlider;
    private JLabel timeLabel;

    // Collider to load next level
    private Rectangle colliderToNextLevel;
    private levelLoader levelLoader; // Reference to levelLoader

    public ForTheQueen() {
        this.setFocusable(true);
        this.addKeyListener(this);
        this.setPreferredSize(new Dimension(1280, 720));

        // Load the sprite sheet
        try {
            spriteSheet = ImageIO.read(new File("knight.png"));
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }

        // Load the level
        lvlManager = new levelManager("Level0.json", "../common/sprites/world_tileset.png");

        // Extract frames using levelManager
        idleFrames = levelManager.extractFrames(spriteSheet, 0, 1, 13, 19); // Idle animation
        runningFrames = levelManager.extractFrames(spriteSheet, 1, 3, 13, 19); // Running animation

        // Initialize the UI components
        initUIComponents();

        // Collider to next level
        colliderToNextLevel = new Rectangle(1100, 92, 20, 20);

        // Initialize the level loader
        levelLoader = new levelLoader(this);

        // Start a game loop using a timer
        Timer timer = new Timer(16, e -> gameLoop());
        timer.start();

        startCountdownTimer();
    }

    private void gameLoop() {
        BufferedImage[] currentFrames = getCurrentAnimationFrames();

        // Create a rectangle representing the player's current position
        Rectangle playerRect = new Rectangle(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT);

        // Horizontal movement
        if (keysPressed.contains(KeyEvent.VK_LEFT)) {
            playerRect.x -= MOVE_SPEED; // Predict next position
            Rectangle collisionBox = lvlManager.checkCollision(playerRect);

            if (collisionBox != null) {
                levelManager.CollisionType collisionType = lvlManager.getCollisionType(collisionBox);

                // Stop movement if STOP_HORIZONTAL is detected
                if (collisionType != levelManager.CollisionType.STOP_HORIZONTAL) {
                    playerX -= MOVE_SPEED; // Allow movement
                }
            } else {
                playerX -= MOVE_SPEED; // No collision, allow movement
            }

            isMovingLeft = true;
            facingLeft = true;
        } else {
            isMovingLeft = false;
        }

        if (keysPressed.contains(KeyEvent.VK_RIGHT)) {
            playerRect.x += MOVE_SPEED; // Predict next position
            Rectangle collisionBox = lvlManager.checkCollision(playerRect);

            if (collisionBox != null) {
                levelManager.CollisionType collisionType = lvlManager.getCollisionType(collisionBox);

                // Stop movement if STOP_HORIZONTAL is detected
                if (collisionType != levelManager.CollisionType.STOP_HORIZONTAL) {
                    playerX += MOVE_SPEED; // Allow movement
                }
            } else {
                playerX += MOVE_SPEED; // No collision, allow movement
            }

            isMovingRight = true;
            facingLeft = false;
        } else {
            isMovingRight = false;
        }

        // Gravity and vertical movement
        verticalVelocity += GRAVITY;
        playerRect.y += verticalVelocity; // Predict next position

        Rectangle collisionBox = lvlManager.checkCollision(playerRect);
        if (collisionBox != null) {
            levelManager.CollisionType collisionType = lvlManager.getCollisionType(collisionBox);

            if (collisionType == levelManager.CollisionType.SOLID) {
                if (verticalVelocity > 0) { // Falling onto the ground
                    playerY = collisionBox.y - PLAYER_HEIGHT;
                    verticalVelocity = 0;
                    isJumping = false;
                } else if (verticalVelocity < 0) { // Hitting a ceiling
                    playerY = collisionBox.y + collisionBox.height;
                    verticalVelocity = 0;
                }
            } else if (collisionType == levelManager.CollisionType.STOP_HORIZONTAL) {
                // Prevent vertical adjustment for horizontal collisions
                if (verticalVelocity > 0) { // Falling
                    verticalVelocity = 0; // Reset vertical velocity
                }
            }
        } else {
            // No collision, apply gravity
            playerY += verticalVelocity;
        }

        // Jumping logic: Only jump if touching a SOLID tile
        if (keysPressed.contains(KeyEvent.VK_SPACE) && !isJumping) {
            playerRect.y += 1; // Check for ground below
            Rectangle groundBox = lvlManager.checkCollision(playerRect);
            if (groundBox != null && lvlManager.getCollisionType(groundBox) == levelManager.CollisionType.SOLID) {
                verticalVelocity = JUMP_STRENGTH; // Jump up
                isJumping = true;
            }
        }

        // Update animation
        if (animationFrame >= currentFrames.length) {
            animationFrame = 0;
        }
        animationCounter++;
        if (animationCounter >= animationSpeed) {
            animationCounter = 0;
            animationFrame = (animationFrame + 1) % currentFrames.length;
        }

        // Check if the player completed the level
        if (playerRect.intersects(colliderToNextLevel)) {
            levelLoader.loadNextLevel();
        }

        repaint();
    }

    private BufferedImage[] getCurrentAnimationFrames() {
        if (isJumping) {
            if (isMovingLeft || isMovingRight) {
                return runningFrames.length > 0 ? runningFrames : idleFrames; // Fallback to idle if empty
            } else {
                return idleFrames;
            }
        }

        if (isMovingLeft || isMovingRight) {
            return runningFrames.length > 0 ? runningFrames : idleFrames; // Fallback to idle if empty
        }

        return idleFrames.length > 0 ? idleFrames : new BufferedImage[1]; // Ensure non-empty return
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        // Background color
        g.setColor(new Color(0, 0, 222, 100));
        g.fillRect(0, 0, getWidth(), getHeight());

        // Draw the player sprite
        BufferedImage currentFrame = getCurrentAnimationFrames()[animationFrame];
        BufferedImage[] currentFrames = getCurrentAnimationFrames();

        if (animationFrame >= currentFrames.length) {
            animationFrame = 0; // Reset to the first frame
        }

        lvlManager.render(g);

        g.drawImage(currentFrames[animationFrame], playerX, playerY, this);

        // Determine if the sprite should be flipped
        boolean flipHorizontally = (isMovingLeft || facingLeft);

        // Draw player sprite
        Graphics2D g2d = (Graphics2D) g;
        if (flipHorizontally) {
            g2d.drawImage(currentFrame, playerX + PLAYER_WIDTH, playerY, -PLAYER_WIDTH, PLAYER_HEIGHT, null);
        } else {
            g2d.drawImage(currentFrame, playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, null);
        }

        // Draw collider to next level
        g.setColor(Color.BLACK);
        g.fillRect(colliderToNextLevel.x, colliderToNextLevel.y, colliderToNextLevel.width, colliderToNextLevel.height);
    }

    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode();
        keysPressed.add(key); // Add the pressed key to the set

        if (key == KeyEvent.VK_SPACE && !isJumping) {
            verticalVelocity = JUMP_STRENGTH; // Apply upwards veloity
            isJumping = true; // Prevents double jump
        }

        // Resets the level
        if (key == KeyEvent.VK_R) {
            resetLevel();
        }

        repaint();
    }

    @Override
    public void keyReleased(KeyEvent e) {
        int key = e.getKeyCode();
        keysPressed.remove(key); // Remove the released key from the set

        // Stop movement if LEFT or RIGHT key is released
        if (key == KeyEvent.VK_LEFT || key == KeyEvent.VK_RIGHT) {
            isMovingLeft = false;
            isMovingRight = false;
        }

        repaint();
    }

    @Override
    public void keyTyped(KeyEvent e) {
        // DO NOT REMOVE THIS
    }

    public static void main(String[] args) {
        new ForTheQueen();
    }

    private void initUIComponents() {
        // Create a slider for the timer
        timeSlider = new JSlider(0, totalTime, totalTime);
        timeSlider.setEnabled(false); // Disable user interactions
        timeSlider.setMajorTickSpacing(10);
        timeSlider.setMinorTickSpacing(1);
        timeSlider.setPaintTicks(true);
        timeSlider.setPaintLabels(true);

        // Create a label to display the time left
        timeLabel = new JLabel("Time left: " + totalTime + "s");
        timeLabel.setFont(new Font("Comic Sans", Font.BOLD, 16));

        // Add slider and label to a panel
        JPanel timerPanel = new JPanel(new BorderLayout());
        timerPanel.add(timeLabel, BorderLayout.WEST);
        timerPanel.add(timeSlider, BorderLayout.CENTER);

        // Add the panel to the top of the main window
        JFrame frame = new JFrame("For The Queen!");
        frame.setLayout(new BorderLayout());
        frame.add(this, BorderLayout.CENTER);
        frame.add(timerPanel, BorderLayout.NORTH);

        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    private void startCountdownTimer() {
        Timer countdownTimer = new Timer(1000, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (timeLeft > 0) {
                    timeLeft--; // Decrement the time left
                    timeSlider.setValue(timeLeft); // Update the slider
                    timeLabel.setText("Time Left: " + timeLeft + "s"); // Update the label
                } else {
                    ((Timer) e.getSource()).stop(); // Stop the timer when time runs out
                    onTimeOut();
                }
            }
        });
        countdownTimer.start();
    }

    private void onTimeOut() {
        // Handle what happens when the time runs out
        JOptionPane.showMessageDialog(this, "Time's up! Game Over.");
        resetLevel(); // Rresets the level
    }

    public void resetLevel() {
        playerX = 200;
        playerY = 500;
        timeLeft = totalTime;
    }

    // Reset input states
    private void resetInputState(KeyEvent e) {
        // Set to false all bools
        isMovingLeft = false;
        isMovingRight = false;
        isJumping = false;

        int key = e.getKeyCode();
        keysPressed.remove(key);
    }

    public void loadNextLevel() {
        // Load the level
        lvlManager = new levelManager("Level1.json", "../common/sprites/world_tileset.png");

        // Collider to next level
        colliderToNextLevel = new Rectangle(600, 200, 20, 20);

        // Sets the new total time to complete the level
        totalTime = 20;
        timeSlider.setMaximum(totalTime);
        timeSlider.setValue(timeLeft);

        // DO NOT REMOVE THIS, THIS ENSURE TO NOT SAVE THE MOVE SPEED/JUMP SPEED WHEN LOADS THE NEXT LEVEL
        MOVE_SPEED = 0;
        JUMP_STRENGTH = 0;
        MOVE_SPEED = 13;
        JUMP_STRENGTH = -10;

        // Resets level
        resetLevel();
    }
}
