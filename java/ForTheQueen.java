
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

public class ForTheQueen extends JPanel implements KeyListener {

    private int playerX = 100; // Initial X position of the player
    private int playerY = 300; // Initial Y position of the player
    private final int PLAYER_WIDTH = 13;
    private final int PLAYER_HEIGHT = 19;
    private final int MOVE_SPEED = 20;

    private float verticalVelocity = 0; // Vertical velocity for jumping
    private final float GRAVITY = .5f; // Gravity pulling the player down
    private final float JUMP_STRENGTH = -10; // Jump strength
    private final int FLOOR_Y = 300; // Y position of the "floor"

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

        // Extract frames using levelManager
        idleFrames = (BufferedImage[]) levelManager.extractFrames(spriteSheet, 0, 1, 13, 19); // Idle animation
        runningFrames = (BufferedImage[]) levelManager.extractFrames(spriteSheet, 1, 3, 13, 19); // Running animation

        // Start a game loop using a timer
        Timer timer = new Timer(16, e -> gameLoop());
        timer.start();
    }

    private void gameLoop() {
        BufferedImage[] currentFrames = getCurrentAnimationFrames();

        // Handle horizontal movement
        if (keysPressed.contains(KeyEvent.VK_LEFT)) {
            playerX -= MOVE_SPEED;
            isMovingLeft = true;
            facingLeft = true;
        } else {
            isMovingLeft = false;
        }

        if (keysPressed.contains(KeyEvent.VK_RIGHT)) {
            playerX += MOVE_SPEED;
            isMovingRight = true;
            facingLeft = false;
        } else {
            isMovingRight = false;
        }

        // Apply gravity if the player is above the floor
        if (playerY < FLOOR_Y || verticalVelocity < 0) {
            verticalVelocity += GRAVITY;
            playerY += verticalVelocity;
        }

        // Ensure the player doesn't fall through the floor
        if (playerY >= FLOOR_Y) {
            playerY = FLOOR_Y;
            verticalVelocity = 0;
            isJumping = false;
        }

        // Reset animationFrame if the current animation changes
        if (animationFrame >= currentFrames.length) {
            animationFrame = 0;
        }

        // Update animation frame
        animationCounter++;
        if (animationCounter >= animationSpeed) {
            animationCounter = 0;
            animationFrame = (animationFrame + 1) % currentFrames.length;
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
        g.setColor(Color.LIGHT_GRAY);
        g.fillRect(0, 0, getWidth(), getHeight());

        // Draw floor
        g.setColor(Color.GREEN);
        g.fillRect(0, FLOOR_Y + PLAYER_HEIGHT, getWidth(), getWidth() - FLOOR_Y);

        // Draw the player sprite
        BufferedImage currentFrame = getCurrentAnimationFrames()[animationFrame];
        BufferedImage[] currentFrames = getCurrentAnimationFrames();

        if (animationFrame >= currentFrames.length) {
            animationFrame = 0;
        }

        g.drawImage(currentFrames[animationFrame], playerX, playerY, this);

        // Determine if the sprite should be flipped
        boolean flipHorizontally = (isMovingLeft || facingLeft);

        Graphics2D g2d = (Graphics2D) g;
        if (flipHorizontally) {
            g2d.drawImage(currentFrame, playerX + PLAYER_WIDTH, playerY, -PLAYER_WIDTH, PLAYER_HEIGHT, null);
        } else {
            g2d.drawImage(currentFrame, playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, null);
        }
    }

    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode();
        keysPressed.add(key);

        if (key == KeyEvent.VK_SPACE && !isJumping) {
            verticalVelocity = JUMP_STRENGTH;
            isJumping = true;
        }

        repaint();
    }

    @Override
    public void keyReleased(KeyEvent e) {
        int key = e.getKeyCode();
        keysPressed.remove(key);

        if (key == KeyEvent.VK_LEFT || key == KeyEvent.VK_RIGHT) {
            isMovingLeft = false;
            isMovingRight = false;
        }

        repaint();
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("For The Queen");
        ForTheQueen game = new ForTheQueen();

        frame.add(game);
        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}
