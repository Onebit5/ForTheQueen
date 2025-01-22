
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.HashSet;
import java.util.Set;
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

    // A set to track which keys are currently pressed
    private final Set<Integer> keysPressed = new HashSet<>();

    public ForTheQueen() {
        this.setFocusable(true);
        this.addKeyListener(this);
        this.setPreferredSize(new Dimension(1280, 720));

        // Start a game loop using a timer
        Timer timer = new Timer(16, e -> gameLoop());
        timer.start();
    }

    private void gameLoop() {
        // Handle horizontal movement based on keys pressed
        if (keysPressed.contains(KeyEvent.VK_LEFT)) {
            playerX -= MOVE_SPEED; // Move left
        } else if (keysPressed.contains(KeyEvent.VK_RIGHT)) {
            playerX += MOVE_SPEED; // Move right
        }

        // Apply gravity if the player is above the floor
        if (playerY < FLOOR_Y || verticalVelocity < 0) {
            verticalVelocity += GRAVITY; // Gravity affects the vertical velocity
            playerY += verticalVelocity; // Update player's Y position
        }

        // Ensure the player doesn't fall through the floor
        if (playerY >= FLOOR_Y) {
            playerY = FLOOR_Y;
            verticalVelocity = 0;
            isJumping = false; // Allow jumping again when on the floor
        }

        repaint();
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

        // Draw player character
        g.setColor(Color.RED);
        g.fillRect(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT);
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
