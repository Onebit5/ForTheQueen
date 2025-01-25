
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.ArrayList;
import java.util.List;
import javax.imageio.ImageIO;
import org.json.JSONArray;
import org.json.JSONObject;

public class levelManager {

    private JSONObject map; // Holds the tile map data
    private List<Rectangle> collisionBoxes; // Stores collision objects
    private Image tilesetImage; // The tileset image
    private int tileWidth, tileHeight, mapWidth, mapHeight, tilesetColumns;

    public levelManager(String mapPath, String tilesetPath) {
        collisionBoxes = new ArrayList<>();
        loadMap(mapPath);
        loadTileset(tilesetPath);
    }

    // Load the tileset image
    private void loadTileset(String tilesetPath) {
        try {
            BufferedImage tilesetBufferedImage = ImageIO.read(new File(tilesetPath));
            tilesetImage = tilesetBufferedImage;
            // Calculate the number of columns in the tileset
            tilesetColumns = tilesetBufferedImage.getWidth() / tileWidth;
        } catch (IOException e) {
            System.err.println("Failed to load tileset: " + e.getMessage());
            System.exit(1);
        }
    }

    // Load the map JSON file
    private void loadMap(String mapPath) {
        try (BufferedReader reader = new BufferedReader(new FileReader(mapPath))) {
            StringBuilder jsonBuilder = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                jsonBuilder.append(line);
            }

            // Parse the JSON data
            map = new JSONObject(jsonBuilder.toString());
            tileWidth = map.getInt("tilewidth");
            tileHeight = map.getInt("tileheight");
            mapWidth = map.getInt("width");
            mapHeight = map.getInt("height");

            // Load collision boxes
            JSONArray layers = map.getJSONArray("layers");
            for (int i = 0; i < layers.length(); i++) {
                JSONObject layerObject = layers.getJSONObject(i);
                if (layerObject.getString("type").equals("tilelayer")) {
                    JSONArray data = layerObject.getJSONArray("data");
                    int tileCount = 0;
                    for (int y = 0; y < mapHeight; y++) {
                        for (int x = 0; x < mapWidth; x++) {
                            int tileId = data.getInt(tileCount++);
                            if (tileId == 17 || tileId == 1) {
                                // Add a collider for the tile
                                collisionBoxes.add(new Rectangle(x * tileWidth, y * tileHeight, tileWidth, tileHeight));
                            }
                        }
                    }
                } else if (layerObject.getString("type").equals("objectgroup")) {
                    JSONArray objects = layerObject.getJSONArray("objects");
                    for (int j = 0; j < objects.length(); j++) {
                        JSONObject object = objects.getJSONObject(j);
                        int x = object.getInt("x");
                        int y = object.getInt("y");
                        int width = object.getInt("width");
                        int height = object.getInt("height");
                        collisionBoxes.add(new Rectangle(x, y, width, height));
                    }
                }
            }
        } catch (Exception e) {
            System.err.println("Failed to load map: " + e.getMessage());
        }
    }

    // Render map
    public void render(Graphics g) {
        JSONArray layers = map.getJSONArray("layers");
        for (int i = 0; i < layers.length(); i++) {
            JSONObject layerObject = layers.getJSONObject(i);
            if (layerObject.getString("type").equals("tilelayer")) {
                JSONArray data = layerObject.getJSONArray("data");
                int tileCount = 0;
                for (int y = 0; y < mapHeight; y++) {
                    for (int x = 0; x < mapWidth; x++) {
                        int tileId = data.getInt(tileCount++);
                        if (tileId != 0) {
                            // Calculate tile position in the tileset
                            int tileX = (tileId - 1) % tilesetColumns * tileWidth;
                            int tileY = (tileId - 1) / tilesetColumns * tileHeight;

                            // Render tile to the screen
                            g.drawImage(tilesetImage,
                                    x * tileWidth, y * tileHeight,
                                    x * tileWidth + tileWidth, y * tileHeight + tileHeight,
                                    tileX, tileY,
                                    tileX + tileWidth, tileY + tileHeight, null);

                            // Add a collider if the tileID matches one of the IDs to add a collision
                            if (tileId == 1 || tileId == 17) {
                                Rectangle collider = new Rectangle(x * tileWidth, y * tileHeight, tileWidth, tileHeight);
                                collisionBoxes.add(collider);
                            }
                        }
                    }
                }
            }
        }

        // Uncoment to show the map collisions
        /* g.setColor(new Color(255, 0, 0, 10));
        for (Rectangle box : collisionBoxes) {
            g.fillRect(box.x, box.y, box.width, box.height);
        }*/
    }

    // Check collision with player
    public boolean checkCollision(Rectangle player) {
        for (Rectangle box : collisionBoxes) {
            if (player.intersects(box)) {
                return true;
            }
        }
        return false;
    }

    public static BufferedImage[] extractFrames(BufferedImage spriteSheet, int row, int frameCount, int frameWidth, int frameHeight) {
        BufferedImage[] frames = new BufferedImage[frameCount];
        for (int i = 0; i < frameCount; i++) {
            frames[i] = spriteSheet.getSubimage(i * frameWidth, row * frameHeight, frameWidth, frameHeight);
        }
        return frames;
    }
}
