#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define ROWS 20
#define COLS 20
#define TILE_SIZE 30
#define FIRE_FRAMES 4 // Number of frames in the fire animation

// Function to add obstacles to the dungeon
void addObstacles(char dungeon[ROWS][COLS], int numObstacles) {
    int placed = 0;
    while (placed < numObstacles) {
        int obsX = rand() % (ROWS - 2) + 1;  // Ensures obsX is not on the boundary (1 to ROWS-2)
        int obsY = rand() % (COLS - 2) + 1;  // Ensures obsY is not on the boundary (1 to COLS-2)

        // Ensure no obstacles are placed near player spawn or escape door
        if (dungeon[obsX][obsY] == '.' &&
            !(obsX <= 1 && obsY <= 1) &&           // Avoid player spawn area
            !(obsX >= ROWS - 2 && obsY >= COLS - 2)) {  // Avoid escape door area
            dungeon[obsX][obsY] = 'O';
            placed++;
        }
    }
}
// Function to add fire to the dungeon on level 3
void addFire(char dungeon[ROWS][COLS], int numFire) {
    int placed = 0;
    while (placed < numFire) {
        int fireX = rand() % (ROWS - 2) + 1;  // Avoid boundary (1 to ROWS-2)
        int fireY = rand() % (COLS - 2) + 1;  // Avoid boundary (1 to COLS-2)

        // Ensure fire does not spawn near player spawn or escape door
        if (dungeon[fireX][fireY] == '.' && 
            !(fireX <= 1 && fireY <= 1) &&  // Avoid player spawn
            !(fireX >= ROWS - 2 && fireY >= COLS - 2)) {  // Avoid escape door
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

    // Load the background image for the start menu
    Texture2D background = LoadTexture("hellbackground.png");
    Texture2D gameTitleImage = LoadTexture("game_title.png");
    
    // Calculate the position to center the title image
int titleImageWidth = 400;
int titleImageHeight = 400;
int titleImageX = (screenWidth - titleImageWidth) * 3 / 4;
int titleImageY = screenHeight / 20;  // This places it a bit lower than the top
DrawTexture(gameTitleImage, titleImageX, titleImageY, WHITE);


    // Calculate positions for title and start button
    int titleFontSize = 40;
    int titleWidth = MeasureText("Dragon Escape", titleFontSize);
    int titleX = (screenWidth - titleWidth) / 2;
    int titleY = screenHeight / 4;
    Rectangle startButton = { screenWidth / 2 - 100, screenHeight * 3 / 4 - 25, 200, 50 };

   
    // Game textures
    Texture2D gameBackground = LoadTexture("background.png");
    Texture2D playerTexture = LoadTexture("player.png");
    Texture2D dragonTexture = LoadTexture("dragon.png");
    Texture2D obstacleTexture = LoadTexture("obstacle.png");
    Texture2D escapeDoorTexture = LoadTexture("escapedoor.png");
    Texture2D fireTexture = LoadTexture("fire.png");
    
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
        if (!startGame) {
            // Start menu loop
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePosition = GetMousePosition();
                if (CheckCollisionPointRec(mousePosition, startButton)) {
    startGame = true;
    startTime = GetTime(); // Store the start time
}
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw start menu
            Rectangle sourceRec = { 0, 0, (float)background.width, (float)background.height };
            Rectangle destRec = { 0, 0, (float)screenWidth, (float)screenHeight };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, WHITE);
            DrawTexture(gameTitleImage, titleImageX, titleImageY, WHITE);
            DrawRectangleRec(startButton, LIGHTGRAY);
            DrawText("Start", startButton.x + startButton.width / 2 - MeasureText("Start", 20) / 2, startButton.y + startButton.height / 2 - 10, 20, DARKGRAY);

            EndDrawing();
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
            if (level > 1) addObstacles(dungeon, 25);
            if (level == 3) {
                // Add fire on level 3, randomly placed
                addFire(dungeon, 20);  // Adjust the number of fire spots
            }

            int playerX = 0, playerY = 0;
            int dragonX = rand() % (ROWS - 1), dragonY = rand() % (COLS - 1);
            while ((dragonX == playerX && dragonY == playerY) || (dragonX >= ROWS - 2 && dragonY >= COLS - 2)) {
                dragonX = rand() % (ROWS - 1);
                dragonY = rand() % (COLS - 1);
            }

            while (!WindowShouldClose() && !gameOver) {
                
                // Update fire animation frame
                fireFrameTime += GetFrameTime();
                if (fireFrameTime >= fireAnimationSpeed) {
                    currentFireFrame = (currentFireFrame + 1) % FIRE_FRAMES;
                    fireFrameTime = 0;
                } 
                    if (IsKeyPressed(KEY_W) && playerX > 0 && dungeon[playerX - 1][playerY] != 'O') playerX--;
else if (IsKeyPressed(KEY_S) && playerX < ROWS - 1 && dungeon[playerX + 1][playerY] != 'O') playerX++;
else if (IsKeyPressed(KEY_A) && playerY > 0 && dungeon[playerX][playerY - 1] != 'O') playerY--;
else if (IsKeyPressed(KEY_D) && playerY < COLS - 1 && dungeon[playerX][playerY + 1] != 'O') playerY++;

                

                if ((playerX == dragonX || playerX == dragonX + 1) && (playerY == dragonY || playerY == dragonY + 1)) {
                    gameOver = true;
                }
                
                if (dungeon[playerX][playerY] == 'F') {
                    gameOver = true;  // End the game if player touches fire
                }

                if ((playerX == ROWS - 1 || playerX == ROWS - 2) && playerY == COLS - 1) {
    level++;
    if (level > 3) {
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
            // Draw the dragon texture scaled to cover 4 tiles (2x2 area)
            DrawTextureEx(dragonTexture, (Vector2){posX, posY}, 0.0f, 2.0f, WHITE);  // Scale by 2 to cover 4 tiles
        } else if (i == ROWS - 1 && j == COLS - 1) {
            DrawTexture(escapeDoorTexture, posX, posY, WHITE);
            DrawTexture(escapeDoorTexture, posX, posY - TILE_SIZE, WHITE);
        } else if (dungeon[i][j] == 'O') {
            DrawTexture(obstacleTexture, posX, posY, WHITE);
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
    sprintf(timeText, "Time Taken: %.2f secs", elapsedTime);
    DrawText(timeText, (screenWidth - MeasureText(timeText, 20)) / 2, screenHeight / 2, 20, WHITE);
    int messageWidth;
    Rectangle restartButton = { (screenWidth / 2) - 100, screenHeight / 2 + 40, 200, 50 };
    Rectangle menuButton = { (screenWidth / 2) - 100, screenHeight / 2 + 100, 200, 50 };

    // Display "You Win!" or "Game Over!" centered on the screen
    if (level > 3) {
        // Display "You Win!" message
        messageWidth = MeasureText("You Win!", 30);
        DrawText("You Win!", (screenWidth - messageWidth) / 2, screenHeight / 2 - 40, 30, GREEN);
    } else {
        // Display "Game Over!" message
        messageWidth = MeasureText("Game Over!", 30);
        DrawText("Game Over!", (screenWidth - messageWidth) / 2, screenHeight / 2 - 40, 30, ORANGE);
    }

    // Draw buttons: Restart and Menu
    DrawRectangleRec(restartButton, LIGHTGRAY);
    DrawText("Restart", restartButton.x + (restartButton.width - MeasureText("Restart", 20)) / 2, restartButton.y + 15, 20, DARKGRAY);

    DrawRectangleRec(menuButton, LIGHTGRAY);
    DrawText("Main Menu", menuButton.x + (menuButton.width - MeasureText("Main Menu", 20)) / 2, menuButton.y + 15, 20, DARKGRAY);

    // Check for button clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePosition = GetMousePosition();

        // Check if restart button was clicked
        if (CheckCollisionPointRec(mousePosition, restartButton)) {
            // Restart the game from level 1
            gameOver = false;
            level = 1;
            playerX = playerY = 0;
            dragonX = rand() % (ROWS - 1);
            dragonY = rand() % (COLS - 1);
            startGame = true;  // Start the game loop again
        }
        // Check if menu button was clicked
        else if (CheckCollisionPointRec(mousePosition, menuButton)) {
            // Go back to landing page
            gameOver = false;
            startGame = false;  // Go back to start menu
            level = 1;
        }
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
    UnloadTexture(background);
    UnloadTexture(gameBackground);
    UnloadTexture(playerTexture);
    UnloadTexture(dragonTexture);
    UnloadTexture(obstacleTexture);
    UnloadTexture(escapeDoorTexture);
    UnloadTexture(gameTitleImage);


    CloseWindow();
    return 0;
}