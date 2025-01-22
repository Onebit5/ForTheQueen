
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
    private final int PLAYER_WIDTH = 50;
    private final int PLAYER_HEIGHT = 50;
    private final int MOVE_SPEED = 20;

    private float verticalVelocity = 0; // Vertical velocity for jumping
    private final float GRAVITY = .5f; // Gravity pulling the player down
    private final float JUMP_STRENGTH = -10; // Jump strength
    private final int FLOOR_Y = 300; // Y position of the "floor"

    private boolean isJumping = false; // Boolean to know if the player is jumping or not
    private boolean isMovingLeft = false;
    private boolean isMovingRight = false;

    // A set to track which keys are currently pressed
    private final Set<Integer> keysPressed = new HashSet<>();

    // Sprite sheet and animations
    private BufferedImage spriteSheet;
    private BufferedImage[] idleFrames;
    private BufferedImage[] runningFrames;
    private int animationFrame = 0; // Current frame of animation
    private int animationSpeed = 0; // Lower = faster animation
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

        // Extract frames from the sprite sheet
        idleFrames = extractFrames(spriteSheet, 0, 1, 13, 19); // Idle animation (row 0, 4 frames)
        runningFrames = extractFrames(spriteSheet, 1, 3, 13, 19); // Running animation (row 1, 6 frames)

        // Start a game loop using a timer
        Timer timer = new Timer(16, e -> gameLoop());
        timer.start();
    }

    private void gameLoop() {
        // Handle horizontal movement
        if (keysPressed.contains(KeyEvent.VK_LEFT)) {
            playerX -= MOVE_SPEED;
            isMovingLeft = true;
        } else {
            isMovingLeft = false;
        }

        if (keysPressed.contains(KeyEvent.VK_RIGHT)) {
            playerX += MOVE_SPEED;
            isMovingRight = true;
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

        // Update animation frame
        animationCounter++;
        if (animationCounter >= animationSpeed) {
            animationCounter = 0;
            animationFrame = (animationFrame + 1) % getCurrentAnimationFrames().length;
        }

        repaint();
    }

    private BufferedImage[] extractFrames(BufferedImage spriteSheet, int row, int frameCount, int frameWidth, int frameHeight) {
        int sheetWidth = spriteSheet.getWidth();
        int sheetHeight = spriteSheet.getHeight();

        System.out.println("Sprite Sheet Dimensions: " + sheetWidth + "x" + sheetHeight);
        System.out.println("Row: " + row + ", Frame Count: " + frameCount + ", Frame Width: " + frameWidth + ", Frame Height: " + frameHeight);

        BufferedImage[] frames = new BufferedImage[frameCount];
        for (int i = 0; i < frameCount; i++) {
            int x = i * frameWidth; // X position of the frame
            int y = row * frameHeight; // Y position of the frame

            // Validate bounds
            if (x + frameWidth > sheetWidth || y + frameHeight > sheetHeight) {
                throw new IllegalArgumentException("Frame is out of sprite sheet bounds: x=" + x + ", y=" + y);
            }

            frames[i] = spriteSheet.getSubimage(x, y, frameWidth, frameHeight);
        }
        return frames;
    }

    private BufferedImage[] getCurrentAnimationFrames() {
        if (isJumping) {
            return idleFrames; // You can replace this with a jumping animation
        } else if (isMovingLeft || isMovingRight) {
            return runningFrames;
        } else {
            return idleFrames;
        }
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
        g.drawImage(currentFrame, playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, null);
    }

    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode();
        keysPressed.add(key); // Add the pressed key to the set

        // Jump
        if (key == KeyEvent.VK_SPACE && !isJumping) {
            verticalVelocity = JUMP_STRENGTH; // Apply upward velocity
            isJumping = true; // Prevent double jumps
        }

        // Repaint to update the screen
        repaint();
    }

    @Override
    public void keyReleased(KeyEvent e) {
        int key = e.getKeyCode();
        keysPressed.remove(key); // Remove the released key from the set
    }

    @Override
    public void keyTyped(KeyEvent e) {
        // Not used but required 
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
