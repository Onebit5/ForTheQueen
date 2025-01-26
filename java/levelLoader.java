
import javax.swing.*;

public class levelLoader {

    private int currentLevel = 0; // Track the current level
    private ForTheQueen gameInstance; // Reference to the game instance

    public levelLoader(ForTheQueen gameInstance) { // Proper constructor
        this.gameInstance = gameInstance;
    }

    public void loadNextLevel() {
        currentLevel++;
        JOptionPane.showMessageDialog(null, "Level " + currentLevel + " loaded!");

        if (gameInstance != null) {
            gameInstance.loadNextLevel(); // Call the method from ForTheQueen
        } else {
            System.err.println("Game instance is null. Cannot load next level.");
        }
    }
}
