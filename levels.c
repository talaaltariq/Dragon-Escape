#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

int main() {
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Select Level");

    // Load textures (make sure these images are in your project directory)
    Texture2D background = LoadTexture("hellbackground.png");  // Background image
    Texture2D levelsImage = LoadTexture("leveltitle.png"); // Title image
    Texture2D level1Image = LoadTexture("level1.png"); 
    Texture2D level2Image = LoadTexture("level2.png"); 
    Texture2D level3Image = LoadTexture("level3.png"); 
    Texture2D level4Image = LoadTexture("level4.png"); 
    Texture2D backButtonImage = LoadTexture("back.png"); // Back button image

    // Define rectangles for each level image (positions and sizes)
    Rectangle level1Bounds = { SCREEN_WIDTH / 4 - level1Image.width / 2, SCREEN_HEIGHT / 3 + 50, level1Image.width, level1Image.height };
    Rectangle level2Bounds = { SCREEN_WIDTH / 4 * 2 - level2Image.width / 2 + 150, SCREEN_HEIGHT / 3 + 50, level2Image.width, level2Image.height };
    Rectangle level3Bounds = { SCREEN_WIDTH / 4 - level3Image.width / 2, SCREEN_HEIGHT / 3 + 150, level3Image.width, level3Image.height };
    Rectangle level4Bounds = { SCREEN_WIDTH / 4 * 2 - level4Image.width / 2 + 150, SCREEN_HEIGHT / 3 + 150, level4Image.width, level4Image.height };

    // Define bounds for the back button
    Rectangle backButtonBounds = { SCREEN_WIDTH / 2 - backButtonImage.width / 2, SCREEN_HEIGHT - 100, backButtonImage.width, backButtonImage.height };

    // Game state variable
    bool levelSelected = false;
    int selectedLevel = 0;
    bool showLevelScreen = true;  // To control the level screen visibility

    SetTargetFPS(60); // Set the target FPS

    while (!WindowShouldClose()) {
        // Handle user input (check mouse clicks)
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePosition = GetMousePosition();

            // Check if any of the levels are clicked
            if (CheckCollisionPointRec(mousePosition, level1Bounds)) {
                selectedLevel = 1;
                levelSelected = true;
            }
            else if (CheckCollisionPointRec(mousePosition, level2Bounds)) {
                selectedLevel = 2;
                levelSelected = true;
            }
            else if (CheckCollisionPointRec(mousePosition, level3Bounds)) {
                selectedLevel = 3;
                levelSelected = true;
            }
            else if (CheckCollisionPointRec(mousePosition, level4Bounds)) {
                selectedLevel = 4;
                levelSelected = true;
            }

            // Check if the Back button is clicked
            if (CheckCollisionPointRec(mousePosition, backButtonBounds)) {
                showLevelScreen = false;  // Hide the level screen (go back)
            }
        }

        // Draw the screen
        BeginDrawing();

        // If level selection screen is active, draw it
        if (showLevelScreen) {
            // Draw the background image to fill the screen
            Rectangle sourceRec = { 0, 0, (float)background.width, (float)background.height };
            Rectangle destRec = { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, WHITE);

            // Draw the levels title image at the top of the screen
            DrawTexture(levelsImage, (SCREEN_WIDTH - levelsImage.width) / 2, 100, WHITE);

            // Draw the level option images
            DrawTexture(level1Image, level1Bounds.x, level1Bounds.y, WHITE);
            DrawTexture(level2Image, level2Bounds.x, level2Bounds.y, WHITE);
            DrawTexture(level3Image, level3Bounds.x, level3Bounds.y, WHITE);
            DrawTexture(level4Image, level4Bounds.x, level4Bounds.y, WHITE);

            // Draw the back button at the bottom of the screen
            DrawTexture(backButtonImage, backButtonBounds.x, backButtonBounds.y, WHITE);
        }

        EndDrawing();
    }

    // Unload textures
    UnloadTexture(background);  // Unload background texture
    UnloadTexture(levelsImage);
    UnloadTexture(level1Image);
    UnloadTexture(level2Image);
    UnloadTexture(level3Image);
    UnloadTexture(level4Image);
    UnloadTexture(backButtonImage);  // Unload back button texture

    // Close window
    CloseWindow();

    return 0;
}
