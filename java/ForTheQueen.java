
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.*;

public class ForTheQueen extends JPanel implements KeyListener {

    private int playerX = 100; // Initial X position of the player
    private int playerY = 300; // Initial Y position of the player
    private final int PLAYER_WIDTH = 50;
    private final int PLAYER_HEIGHT = 50;
    private final int MOVE_SPEED = 20;

    public ForTheQueen() {
        this.setFocusable(true);
        this.addKeyListener(this);
        this.setPreferredSize(new Dimension(1280, 720));
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        // Background color
        g.setColor(Color.LIGHT_GRAY);
        g.fillRect(0, 0, getWidth(), getHeight());

        // Draw player character
        g.setColor(Color.RED);
        g.fillRect(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT);
    }

    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode();

        if (key == KeyEvent.VK_LEFT) {
            playerX -= MOVE_SPEED; // Move left
        } else if (key == KeyEvent.VK_RIGHT) {
            playerX += MOVE_SPEED; // Move right
        }

        // Repaint to update the screen
        repaint();
    }

    @Override
    public void keyReleased(KeyEvent e) {
        // Not used but required 
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
