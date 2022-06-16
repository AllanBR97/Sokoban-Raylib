#include <raylib.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "Game.h"

int Game::saveMap()
{
	if (switch_mode == 0)
	{
		FILE* f = nullptr;

		fopen_s(&f, "levels\\save.txt", "w");

		if (f == NULL) return 1;

		for (int i = 0; i < count_boxes; i++)
		{
			fprintf_s(f, "B %d %d\n", (int)box[i].x, (int)box[i].y);
		}
		for (int i = 0; i < count_wall; i++)
		{
			fprintf_s(f, "W %d %d\n", (int)wall[i].x, (int)wall[i].y);
		}
		for (int i = 0; i < count_goal; i++)
		{
			fprintf_s(f, "G %d %d\n", (int)goal[i].x, (int)goal[i].y);
		}

		fprintf_s(f, "P %d %d\n", (int)player.x, (int)player.y);
		fclose(f);
	}
	else printf("Not in edit mode");
	
	return 0;
}

int Game::loadMap()
{
	FILE* f = nullptr;
	char temp[20];

	sprintf_s(temp, "levels\\%s", files[level]);
	fopen_s(&f, temp, "r");

	if (f == NULL) return 1;

	char temp_letter;
	int x = 0, y = 0;

	while (fscanf_s(f, "%c %d %d\n", &temp_letter, 1, &x, &y) != EOF)
	{
		switch (temp_letter)
		{
		case 'B':
			box[count_boxes].x = (float)x;
			box[count_boxes].y = (float)y;
			box[count_boxes].height = SPRITES_SIZE;
			box[count_boxes].width = SPRITES_SIZE;
			count_boxes++;
			break;
		case 'W':
			wall[count_wall].x = (float)x;
			wall[count_wall].y = (float)y;
			wall[count_wall].height = SPRITES_SIZE;
			wall[count_wall].width = SPRITES_SIZE;
			count_wall++;
			break;
		case 'G':
			goal[count_goal].x = (float)x;
			goal[count_goal].y = (float)y;
			goal[count_goal].height = SPRITES_SIZE;
			goal[count_goal].width = SPRITES_SIZE;
			count_goal++;
			break;
		case 'P':
			player.x = (float)x;
			player.y = (float)y;
			player.height = SPRITES_SIZE;
			player.width = SPRITES_SIZE;
			break;
		default:
			break;
		}
	}
	fclose(f);

	return 0;
}

void Game::putWall()
{
	if (switch_mode == 0)
	{
		wall[count_wall].x = editor.x;
		wall[count_wall].y = editor.y;
		count_wall++;
	}
	
}

void Game::putBox()
{
	if (switch_mode == 0)
	{
		box[count_boxes].x = editor.x;
		box[count_boxes].y = editor.y;
		count_boxes++;
	}
	
}

void Game::putGoal()
{
	if (switch_mode == 0)
	{
		goal[count_goal].x = editor.x;
		goal[count_goal].y = editor.y;
		count_goal++;
	}
}

void Game::putPlayer()
{
	if (switch_mode == 0)
	{
		player.x = editor.x;
		player.y = editor.y;
	}
	else printf("Not in edit mode\n");
}

void Game::setMode(int mode)
{
	switch_mode = mode;
}

void Game::resetModes()
{
	switch_mode = 0; // starts with editor
	switch_menu = 0;
	switch_screen = 0;
}

// Reset variables
void Game::Reset()
{
	editor.x = 0;
	editor.y = 0;
	editor.width = SPRITES_SIZE;
	editor.height = SPRITES_SIZE;

	player.width = SPRITES_SIZE;
	player.height = SPRITES_SIZE;

	moves = 0;
	counter_timer = 0;
	count_boxes = 0;
	count_goal = 0;
	count_wall = 0;
}

// Clear selected tile
void Game::clearTile()
{
	for (int i = 0; i < count_wall; i++)
	{
		if (wall[count_wall - 1].x == editor.x && wall[count_wall - 1].y == editor.y)
		{
			count_wall--;
		}
		else if (wall[i].x == editor.x && wall[i].y == editor.y)
		{
			for (int x = i; x < count_wall; x++)
			{
				wall[x].x = wall[x + 1].x;
				wall[x].y = wall[x + 1].y;
			}
			count_wall--;
		}
	}

	for (int i = 0; i < count_boxes; i++)
	{
		if (box[count_boxes - 1].x == editor.x && box[count_boxes - 1].y == editor.y)
		{
			count_boxes--;
		}
		else if (box[i].x == editor.x && box[i].y == editor.y)
		{
			for (int x = i; x < count_boxes; x++)
			{
				box[x].x = box[x + 1].x;
				box[x].y = box[x + 1].y;
			}
			count_boxes--;
		}
	}

	for (int i = 0; i < count_goal; i++)
	{
		// decrement count_goal if last goal index is in same editor position
		if (goal[count_goal - 1].x == editor.x && goal[count_goal - 1].y == editor.y)
		{
			count_goal--;
		}
		// 
		else if (goal[i].x == editor.x && goal[i].y == editor.y)
		{
			for (int x = i; x < count_goal; x++)
			{
				// copy next goal position into current index
				goal[x].x = goal[x + 1].x;
				goal[x].y = goal[x + 1].y;
			}
			count_goal--;
		}
	}
}

bool Game::isVictory()
{
	int counter = 0;
	for (int i = 0; i < count_goal; i++)
	{
		for (int x = 0; x < count_boxes; x++)
		{
			if (box[x].x == goal[i].x && box[x].y == goal[i].y)
			{
				counter++;
			}
		}
	}
	return counter == count_boxes ? true : false;
}

void Game::nextLevel()
{
	level++;
	if (level == MAX_LEVEL)
	{
		level = 0;
		Reset();
		setMode(1);
		switch_screen = SCREEN_MENU;
	}
	else
	{
		Reset();
		loadMap();
	}
	
}

// Editor move
void Game::moveUp()
{
	if (switch_mode == 0)
	{
		editor.y -= SPRITES_SIZE;
		printf("Ed: %3.f %3.f --- Is empty: %d\n", editor.x, editor.y, isEmptySpace(editor.x, editor.y));
	}
}

void Game::moveDown()
{
	if (switch_mode == 0)
	{
		editor.y += SPRITES_SIZE;
		printf("Ed: %3.f %3.f --- Is empty: %d\n", editor.x, editor.y, isEmptySpace(editor.x, editor.y));
	}
	
}

void Game::moveLeft()
{
	if (switch_mode == 0)
	{
		editor.x -= SPRITES_SIZE;
		printf("Ed: %3.f %3.f --- Is empty: %d\n", editor.x, editor.y, isEmptySpace(editor.x, editor.y));
	}
}

void Game::moveRight()
{
	if (switch_mode == 0)
	{
		editor.x += SPRITES_SIZE;
		printf("Ed: %3.f %3.f --- Is empty: %d\n", editor.x, editor.y, isEmptySpace(editor.x, editor.y));
	}
	
}

void Game::playerMoveUp()
{
	int id;
	if (switch_mode == 1)
	{
		if (!CheckColWall('U') && !CheckColBox('U', &id))
		{
			player.y -= SPRITES_SIZE;
			printf("P: %3.f %3.f\n", player.x, player.y);
			moves++;
		}
		else if (CheckColBox('U', &id) && isEmptySpace(player.x, player.y - SPRITES_SIZE * 2))
		{
			player.y = box[id].y;
			box[id].y -= SPRITES_SIZE;
			moves++;

			if (isVictory())
			{
				printf("VITORIA\n");
				nextLevel();
			}
		}
		
	}
}

void Game::playerMoveDown()
{
	int id;
	if (switch_mode == 1)
	{
		if (!CheckColWall('D') && !CheckColBox('D', &id))
		{
			player.y += SPRITES_SIZE;
			printf("P: %3.f %3.f\n", player.x, player.y);
			moves++;
		}
		else if (CheckColBox('D', &id) && isEmptySpace(player.x, player.y + SPRITES_SIZE * 2))
		{
			player.y = box[id].y;
			box[id].y += SPRITES_SIZE;
			moves++;

			if (isVictory())
			{
				printf("VITORIA\n");
				nextLevel();
			}
		}
		
	}
}

void Game::playerMoveLeft()
{
	int id;
	if (switch_mode == 1)
	{
		if (!CheckColWall('L') && !CheckColBox('L', &id))
		{
			player.x -= SPRITES_SIZE;
			printf("P: %3.f %3.f\n", player.x, player.y);
			moves++;
		}
		else if (CheckColBox('L', &id) && isEmptySpace(player.x - SPRITES_SIZE * 2, player.y))
		{
			player.x = box[id].x;
			box[id].x -= SPRITES_SIZE;
			moves++;

			if (isVictory())
			{
				printf("VITORIA\n");
				nextLevel();
			}
		}
	}
}

void Game::playerMoveRight()
{
	int id;
	if (switch_mode == 1)
	{
		if (!CheckColWall('R') && !CheckColBox('R', &id))
		{
			player.x += SPRITES_SIZE;
			printf("P: %3.f %3.f\n", player.x, player.y);
			moves++;
		}
		else if (CheckColBox('R', &id) && isEmptySpace(player.x + SPRITES_SIZE * 2, player.y))
		{
			player.x = box[id].x;
			box[id].x += SPRITES_SIZE;
			moves++;

			if (isVictory())
			{
				printf("VITORIA\n");
				nextLevel();
			}
		}
	}
}

bool Game::isEmptySpace(float x, float y)
{
	for (int i = 0; i < count_wall; i++)
	{
		if (x == wall[i].x && y == wall[i].y)
		{
			return false;
		}
	}

	for (int i = 0; i < count_boxes; i++)
	{
		if (x == box[i].x && y == box[i].y)
		{
			return false;
		}
	}
	return true;
}

bool Game::CheckColWall(char dir)
{
	switch (dir)
	{
	case 'U':
		for (int i = 0; i < count_wall; i++)
		{
			if (player.x == wall[i].x && player.y - SPRITES_SIZE == wall[i].y)
			{
				return true;
			}
		}
		break;
	case 'D':
		for (int i = 0; i < count_wall; i++)
		{
			if (player.x == wall[i].x && player.y + SPRITES_SIZE == wall[i].y)
			{
				return true;
			}
		}
		break;
	case 'L':
		for (int i = 0; i < count_wall; i++)
		{
			if (player.x - SPRITES_SIZE == wall[i].x && player.y == wall[i].y)
			{
				return true;
			}
		}
		break;
	case 'R':
		for (int i = 0; i < count_wall; i++)
		{
			if (player.x + SPRITES_SIZE == wall[i].x && player.y == wall[i].y)
			{
				return true;
			}
		}
		break;
	}
	return false;
}

bool Game::CheckColBox(char dir, int *id)
{
	switch (dir)
	{
	case 'U':
		for (int i = 0; i < count_boxes; i++)
		{
			if (player.x == box[i].x && player.y - SPRITES_SIZE == box[i].y)
			{
				*id = i;
				return true;
			}
		}
		break;
	case 'D':
		for (int i = 0; i < count_boxes; i++)
		{
			if (player.x == box[i].x && player.y + SPRITES_SIZE == box[i].y)
			{
				*id = i;
				return true;
			}
		}
		break;
	case 'L':
		for (int i = 0; i < count_boxes; i++)
		{
			if (player.x - SPRITES_SIZE == box[i].x && player.y == box[i].y)
			{
				*id = i;
				return true;
			}
		}
		break;
	case 'R':
		for (int i = 0; i < count_boxes; i++)
		{
			if (player.x + SPRITES_SIZE == box[i].x && player.y == box[i].y)
			{
				*id = i;
				return true;
			}
		}
		break;
	}
	return false;
}

