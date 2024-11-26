#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define ROWS 20
#define COLS 20
#define TILE_SIZE 30
#define FIRE_FRAMES 4 // Number of frames in the fire animation
#define DRAGON_FRAMES 7

// Function to add obstacles to the dungeon
void addObstacles(char dungeon[ROWS][COLS], int numObstacles, int level) {
    int placed = 0;
    while (placed < numObstacles) {
        int obsX = rand() % (ROWS - 2) + 1;  // Ensures obsX is not on the boundary (1 to ROWS-2)
        int obsY = rand() % (COLS - 2) + 1;  // Ensures obsY is not on the boundary (1 to COLS-2)

        // Ensure no obstacles are placed near player spawn or escape door
        if (dungeon[obsX][obsY] == '.' &&
    !(obsX <= 1 && obsY <= 1) &&           
    !(obsX >= ROWS - 2 && obsY >= COLS - 2)) {
    // On level 4, some obstacles become bars
    if (level == 4 && rand() % 2 == 0) { // 50% chance for bars
        dungeon[obsX][obsY] = 'B'; // 'B' represents bars
    } else {
        dungeon[obsX][obsY] = 'O'; // 'O' represents regular obstacles
    }
    placed++;
}
    }
}


// Function to add fire to the dungeon on level 3 and above
void addFire(char dungeon[ROWS][COLS], int numFire) {
    int placed = 0;

    // Define "safe zones" around the player spawn and escape door
    // Adjust these to create enough room for safe navigation
    int safeZonePlayerX[4] = {0, 0, 1, 1};  // Safe tiles around the player's spawn
    int safeZonePlayerY[4] = {0, 1, 0, 1};
    int safeZoneEscapeX[4] = {ROWS - 1, ROWS - 2, ROWS - 1, ROWS - 2};
    int safeZoneEscapeY[4] = {COLS - 1, COLS - 1, COLS - 2, COLS - 2};

    while (placed < numFire) {
        int fireX = rand() % (ROWS - 2) + 1;  // Avoid boundary (1 to ROWS-2)
        int fireY = rand() % (COLS - 2) + 1;  // Avoid boundary (1 to COLS-2)

        // Ensure fire does not spawn in safe zones or overwrite other fire
        bool isSafe = false;
        for (int i = 0; i < 4; i++) {
            if ((fireX == safeZonePlayerX[i] && fireY == safeZonePlayerY[i]) ||
                (fireX == safeZoneEscapeX[i] && fireY == safeZoneEscapeY[i])) {
                isSafe = true;
                break;
            }
        }

        if (!isSafe && dungeon[fireX][fireY] == '.') {
            dungeon[fireX][fireY] = 'F';
            placed++;
        }
    }
}


// Move dragon towards the player
void moveDragonTowardsPlayer(int *dragonX, int *dragonY, int playerX, int playerY) {
    if (abs(playerX - *dragonX) > abs(playerY - *dragonY)) {
        if (playerX > *dragonX && *dragonX < ROWS - 2) (*dragonX)++;
        else if (playerX < *dragonX && *dragonX > 0) (*dragonX)--;
    } else {
        if (playerY > *dragonY && *dragonY < COLS - 2) (*dragonY)++;
        else if (playerY < *dragonY && *dragonY > 0) (*dragonY)--;
    }
}

int main() {
    double startTime;
    const int screenWidth = COLS * TILE_SIZE;
    const int screenHeight = ROWS * TILE_SIZE;
    InitWindow(screenWidth, screenHeight, "Dragon Escape - Start Menu");
    InitAudioDevice();
    

    // Load the background image for the start menu
    Texture2D background = LoadTexture("hellbackground.png");
    Texture2D gameTitleImage = LoadTexture("game_title.png");
    
    // Calculate the position to center the title image
int titleImageWidth = 400;
int titleImageHeight = 400;
int titleImageX = (screenWidth - titleImageWidth) * 3 / 4;
int titleImageY = screenHeight / 20;  // This places it a bit lower than the top
DrawTexture(gameTitleImage, titleImageX, titleImageY, WHITE);




   
    // Game textures
    Texture2D playButton = LoadTexture("play.png");
    Texture2D quitButton = LoadTexture("quit.png");
    Texture2D gameBackground = LoadTexture("background.png");
    Texture2D playerTexture = LoadTexture("player.png");
    Texture2D obstacleTexture = LoadTexture("obstacle.png");
    Texture2D barsTexture = LoadTexture("bars.png");
    Texture2D escapeDoorTexture = LoadTexture("escapedoor.png");
    Texture2D fireTexture = LoadTexture("fire.png");
    Sound moveSound = LoadSound("movement.mp3");
    Sound doorSound = LoadSound("door_open.mp3");
    Sound dragonSound = LoadSound("dragon_roar.mp3");
    Music backgroundMusic = LoadMusicStream("background_music.mp3");
    Sound winSound = LoadSound("win_sound.mp3");
    

PlayMusicStream(backgroundMusic); // Start the music stream
    
    // Load dragon animation frames
Texture2D dragonTextures[DRAGON_FRAMES];
for (int i = 0; i < DRAGON_FRAMES; i++) {
    char filePath[30];
    sprintf(filePath, "Dragon_%d.png", i + 1); // Assuming Dragon_1.png, Dragon_2.png, etc.
    dragonTextures[i] = LoadTexture(filePath);
}
    
    int currentDragonFrame = 0;           // Current frame of the dragon animation
float dragonFrameTime = 0.0f;         // Timer to track elapsed time for frame switching
float dragonAnimationSpeed = 0.15f;   // Time in seconds between frames

    // Load fire animation frames
    Texture2D fireTextures[FIRE_FRAMES];
    for (int i = 0; i < FIRE_FRAMES; i++) {
        char filePath[30];
        sprintf(filePath, "Fire_%d.png", i + 1);  // Assuming Fire_1.png, Fire_2.png, etc.
        fireTextures[i] = LoadTexture(filePath);
    }

    int currentFireFrame = 0;
    float fireFrameTime = 0;      // Tracks elapsed time for frame switching
    float fireAnimationSpeed = 0.2f; // Time in seconds between frames

    srand(time(0));
    char dungeon[ROWS][COLS];
    
    SetTargetFPS(60);
    
     // Game state variables
    bool startGame = false;
    bool gameOver = false;
    int level = 1;


    while (!WindowShouldClose()) {
        BeginDrawing();
    ClearBackground(RAYWHITE);
        // Define button rectangles
    // Define the button bounds
    Rectangle playButtonBounds = { screenWidth / 2 - 80, screenHeight / 2 - 5, 200, 100 }; // Play button rectangle
    Rectangle quitButtonBounds = { screenWidth / 2 - 70, screenHeight / 2 + 200, 200, 100 }; // Quit button rectangle

if (!startGame) {
  

    // Draw the start menu background and title
    Rectangle sourceRec = { 0, 0, (float)background.width, (float)background.height };
    Rectangle destRec = { 0, 0, (float)screenWidth, (float)screenHeight };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, WHITE);
    DrawTexture(gameTitleImage, titleImageX, titleImageY, WHITE);


// Draw the "Play" and "Quit" buttons
DrawTexture(playButton, playButtonBounds.x, playButtonBounds.y, WHITE);
DrawTexture(quitButton, quitButtonBounds.x, quitButtonBounds.y, WHITE);

    EndDrawing();

    // Handle mouse click logic
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePosition = GetMousePosition();

        // Check if "Play" button was clicked
        if (CheckCollisionPointRec(mousePosition, playButtonBounds)) {
            startGame = true;
            startTime = GetTime(); // Store the start time
        }

        // Check if "Quit" button was clicked
        if (CheckCollisionPointRec(mousePosition, quitButtonBounds)) {
            CloseWindow(); // Close the game window
        }
    }
} else {
            // Game loop
            // Initialize dungeon with open spaces
for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
        dungeon[i][j] = '.';
    }
}

// Initialize dungeon with open spaces
for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
        dungeon[i][j] = '.';
    }
}

// Set boundaries with obstacles ('O')
for (int i = 0; i < ROWS; i++) {
    dungeon[i][0] = 'O';           // Left boundary
    dungeon[i][COLS - 1] = 'O';     // Right boundary
}
for (int j = 0; j < COLS; j++) {
    dungeon[0][j] = 'O';            // Top boundary
    dungeon[ROWS - 1][j] = 'O';     // Bottom boundary
}

// Clear player start and exit locations
dungeon[0][0] = '.';                // Player start
dungeon[0][1] = '.';                // Right side of the player start
dungeon[ROWS - 1][COLS - 1] = 'E';  // Exit
dungeon[ROWS - 2][COLS - 1] = '.';  // Upper side of the escape door

            dungeon[ROWS - 1][COLS - 1] = 'E';
            if (level > 1) addObstacles(dungeon, 25, level);
            if (level >= 3) {
                // Add fire on level 3, randomly placed
                addFire(dungeon, 20);  // Adjust the number of fire spots
            }

            int playerX = 0, playerY = 0;
            int dragonX = ROWS / 2, dragonY = COLS / 2;
            while ((dragonX == playerX && dragonY == playerY) || (dragonX >= ROWS - 2 && dragonY >= COLS - 2)) {
                dragonX = rand() % (ROWS - 1) ;
                dragonY = rand() % (COLS - 1);
            }

            while (!WindowShouldClose() && !gameOver) {
                 UpdateMusicStream(backgroundMusic);
                // Update dragon animation
dragonFrameTime += GetFrameTime();
if (dragonFrameTime >= dragonAnimationSpeed) {
    currentDragonFrame = (currentDragonFrame + 1) % DRAGON_FRAMES; // Cycle through frames
    dragonFrameTime = 0;
}
// Move dragon towards the player occasionally
if (rand() % 60 < 3) {
    moveDragonTowardsPlayer(&dragonX, &dragonY, playerX, playerY);
} 
                // Update fire animation frame
                fireFrameTime += GetFrameTime();
                if (fireFrameTime >= fireAnimationSpeed) {
                    currentFireFrame = (currentFireFrame + 1) % FIRE_FRAMES;
                    fireFrameTime = 0;
                } 
                    if (IsKeyPressed(KEY_W) && playerX > 0 && dungeon[playerX - 1][playerY] != 'O' && dungeon[playerX - 1][playerY] != 'B') {
    playerX--;
    PlaySound(moveSound);
} else if (IsKeyPressed(KEY_S) && playerX < ROWS - 1 && dungeon[playerX + 1][playerY] != 'O' && dungeon[playerX + 1][playerY] != 'B') {
    playerX++;
    PlaySound(moveSound);
} else if (IsKeyPressed(KEY_A) && playerY > 0 && dungeon[playerX][playerY - 1] != 'O' && dungeon[playerX][playerY - 1] != 'B') {
    playerY--;
    PlaySound(moveSound);
} else if (IsKeyPressed(KEY_D) && playerY < COLS - 1 && dungeon[playerX][playerY + 1] != 'O' && dungeon[playerX][playerY + 1] != 'B') {
    playerY++;
    PlaySound(moveSound);
}

                

                if ((playerX == dragonX || playerX == dragonX + 1) && (playerY == dragonY || playerY == dragonY + 1)) {
                    PlaySound(dragonSound);
                    gameOver = true;
                }
                
                if (dungeon[playerX][playerY] == 'F') {
                    PlaySound(dragonSound);
                    gameOver = true;  // End the game if player touches fire
                }

                if ((playerX == ROWS - 1 || playerX == ROWS - 2) && playerY == COLS - 1) {
                    PlaySound(doorSound);
    level++;
    if (level > 4) {
        gameOver = true;
    } else {
        break;  // Proceed to the next level if level <= 3
    }
}

                if (rand() % 100 < 3) {
                    moveDragonTowardsPlayer(&dragonX, &dragonY, playerX, playerY);
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTexture(gameBackground, 0, 0, RAYWHITE);

                for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
        int posX = j * TILE_SIZE;
        int posY = i * TILE_SIZE;

        if (i == playerX && j == playerY) {
            DrawTexture(playerTexture, posX, posY, WHITE);
        } else if (i == dragonX && j == dragonY) {
            // Draw the current dragon frame scaled to cover 4 tiles (2x2 area)
            DrawTextureEx(dragonTextures[currentDragonFrame], (Vector2){posX, posY}, 0.0f, 2.0f, WHITE);
        } else if (i == ROWS - 1 && j == COLS - 1) {
            DrawTexture(escapeDoorTexture, posX, posY, WHITE);
            DrawTexture(escapeDoorTexture, posX, posY - TILE_SIZE, WHITE);
        } else if (dungeon[i][j] == 'O') {
            DrawTexture(obstacleTexture, posX, posY, WHITE);
        } else if (dungeon[i][j] == 'B') {
            DrawTexture(barsTexture, posX, posY, WHITE); // Draw bars obstacle
        } else if (dungeon[i][j] == 'F') {
            DrawTexture(fireTextures[currentFireFrame], posX, posY, WHITE);  // Fire tile
        }
    }
}


                if (gameOver) {
    // Calculate elapsed time
    double elapsedTime = GetTime() - startTime;

    // Display elapsed time
    char timeText[50];
    sprintf(timeText, "Time Taken: %.2f seconds", elapsedTime);

    // Display "You Win!" or "Game Over!" message
    if (level > 4) {
        PlaySound(winSound);
        int winMessageWidth = MeasureText("You Win!", 30);
        DrawText("You Win!", (screenWidth - winMessageWidth) / 2, screenHeight / 2 - 60, 30, GREEN);

        // Display the elapsed time below the "You Win!" message
        int timeMessageWidth = MeasureText(timeText, 20);
        DrawText(timeText, (screenWidth - timeMessageWidth) / 2, screenHeight / 2 - 20, 20, WHITE);
    } else {
        int gameOverMessageWidth = MeasureText("Game Over!", 30);
        DrawText("Game Over!", (screenWidth - gameOverMessageWidth) / 2, screenHeight / 2 - 60, 30, ORANGE);

        // Display the elapsed time below the "Game Over!" message
        int timeMessageWidth = MeasureText(timeText, 20);
        DrawText(timeText, (screenWidth - timeMessageWidth) / 2, screenHeight / 2 - 20, 20, WHITE);
    }
}

 

            DrawText(TextFormat("Level: %d", level), 40, TILE_SIZE * 2, 20, WHITE);
                EndDrawing();
            }
        }
    }
    // Unload textures
    for (int i = 0; i < FIRE_FRAMES; i++) {
        UnloadTexture(fireTextures[i]);
    }
    for (int i = 0; i < DRAGON_FRAMES; i++) {
        UnloadTexture(dragonTextures[i]);
    }
    UnloadTexture(background);
    UnloadTexture(gameBackground);
    UnloadTexture(playerTexture);
    UnloadTexture(obstacleTexture);
    UnloadTexture(escapeDoorTexture);
    UnloadTexture(barsTexture);
    UnloadTexture(gameTitleImage);
    UnloadTexture(playButton);
    UnloadTexture(quitButton);
    UnloadSound(moveSound);
    UnloadSound(doorSound);
    UnloadSound(dragonSound);
    UnloadMusicStream(backgroundMusic);
    UnloadSound(winSound);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}
