#pragma once
#include <raylib.h>

#define SPRITES_SIZE 40
#define MAX_LEVEL 11

#define SCREEN_MENU 0
#define SCREEN_GAME 1
#define SCREEN_EDITOR 2
#define SCREEN_ABOUT 3

// Game struct
struct Game
{
	int count_boxes, count_goal, count_wall, moves;
	int switch_mode; // 0 - Editor, 1 - Player
	int switch_menu; // 0 - New game, 1 - Editor, 2 - About
	int switch_screen; // SCREEN_MENU, SCREEN_GAME, SCREEN_EDITOR, SCREEN_ABOUT
	int counter, counter_timer;

	int level = 0;
	const char *files[MAX_LEVEL] = {
		"1.1.txt",
		"1.2.txt",
		"1.3.txt",
		"2.1.txt",
		"2.2.txt",
		"2.3.txt",
		"3.1.txt",
		"3.2.txt",
		"3.3.txt",
		"4.1.txt",
		"4.2.txt",
	};

	Rectangle editor; // editor
	Rectangle player;
	Rectangle wall[100]; // wall
	Rectangle box[100]; // box to push
	Rectangle goal[100]; // goal

	int saveMap();
	int loadMap();

	void putWall();
	void putBox();
	void putGoal();
	void putPlayer();
	void setMode(int);
	void resetModes();
	void Reset();
	void clearTile();
	void nextLevel();
	bool isVictory();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void playerMoveUp();
	void playerMoveDown();
	void playerMoveLeft();
	void playerMoveRight();
	bool isEmptySpace(float x, float y);
	bool CheckColWall(char dir);
	bool CheckColBox(char dir, int *id);
};

