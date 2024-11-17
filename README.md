# 2D DRAGON DUNGEON ESCAPE 
<p> The game is a 2D dungeon escape game where the player must move through a dungeon, avoid obstacles, avoid fire (in later levels), and escape before a dragon catches them. The game has multiple levels, and the objective is to reach an exit while avoiding dangers like obstacles and fire.

Basic Structure:
Start Menu:

When you launch the game, you see a start menu with a "Start" button.
Clicking the button takes you into the game loop where the actual gameplay starts.
Dungeon Setup:

The dungeon is represented as a grid (20 rows by 20 columns), where each cell can contain:
. (an empty space the player can move through),
O (an obstacle the player cannot move through),
F (fire, which the player must avoid),
E (exit door).
Game Levels:

Level 1: You start in a dungeon with some obstacles.
Level 2: More obstacles are added to make it harder.
Level 3: Fire hazards are introduced, which the player must avoid.
Player and Dragon Movement:

The player moves around using the W, A, S, D keys (up, left, down, right) through the dungeon.
The dragon is also in the dungeon and chases the player. It moves towards the player randomly at a rate of about 3% each frame, creating some tension.
Objective:

The goal is to reach the exit door (E), located at the bottom-right corner of the dungeon.
As the player moves, they avoid obstacles (O) and fire (F), and they try to escape before the dragon catches them.
Collision and Game Over:

If the player collides with an obstacle or fire, or if the dragon catches the player, the game ends with a Game Over message.
If the player reaches the exit door, they advance to the next level. After completing all the levels, the game displays a "You Win!" message.</p>