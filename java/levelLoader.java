
import javax.swing.*;

public class levelLoader {

    private int currentLevel = 0; // Track the current level
    private ForTheQueen gameInstance; // Reference to the game instance

    public void levelLoader(ForTheQueen gameInstance) {
        this.gameInstance = gameInstance;
    }

    public void loadNextLevel() {
        currentLevel++;
        JOptionPane.showMessageDialog(null, "Level " + currentLevel + " loaded!");

        gameInstance.loadNextLevel();
    }
}
