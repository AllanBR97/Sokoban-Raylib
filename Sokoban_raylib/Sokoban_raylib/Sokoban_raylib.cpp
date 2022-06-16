#include <iostream>
#include <raylib.h>
#include "Game.h"

void DrawObjects(Game* game)
{
	for (int x = 0; x < game->count_wall; x++)
	{
		DrawRectangle((int)game->wall[x].x, (int)game->wall[x].y, SPRITES_SIZE, SPRITES_SIZE, DARKBROWN);
	}

	for (int x = 0; x < game->count_boxes; x++)
	{
		DrawRectangle((int)game->box[x].x, (int)game->box[x].y, SPRITES_SIZE, SPRITES_SIZE, BLUE);
	}

	for (int x = 0; x < game->count_goal; x++)
	{
		DrawRectangleLines((int)game->goal[x].x, (int)game->goal[x].y, SPRITES_SIZE, SPRITES_SIZE, GOLD);
	}
}

void DrawAbout()
{
	DrawText("Help", 200, 100, 20, GOLD);
	DrawText("Arrows keys: control player and editor\nQ key: go back to menu", 200, 150, 20, GOLD);

	DrawText("Editor mode", 200, 250, 30, BLUE);
	DrawText("W key: put wall\nB key: put box\nG key: put goal\nP key: put player\n\
	Space key: switch to player/editor in editor mode", 200, 300, 20, GOLD);
}

void DrawHUD(Game* g)
{
	char temp[20];
	sprintf_s(temp, "Moves: %d", g->moves);
	DrawText(temp, 525, 0, 30, RED);

	char name[20];
	strcpy_s(name, g->files[g->level]);

	name[strlen(name) - 4] = '\0';
	sprintf_s(temp, "Level: %s", name);
	DrawText(temp, 525, 50, 30, RED);

	sprintf_s(temp, "Level time: %d", g->counter_timer);
	DrawText(temp, 525, 100, 30, RED);

	sprintf_s(temp, "Time: %d", g->counter);
	DrawText(temp, 525, 150, 30, RED);
}

void DrawStartHUD(Game* g)
{
	Color color;
	DrawText("Sokoban", 250, 160, 60, RED);
	g->switch_menu == 0 ? color = GOLD : color = RED;
	DrawText("New game", 250, 260, 30, color);
	g->switch_menu == 1 ? color = GOLD : color = RED;
	DrawText("Editor mode", 250, 290, 30, color);
	g->switch_menu == 2 ? color = GOLD : color = RED;
	DrawText("About", 250, 320, 30, color);
	DrawText("A game made in C with Raylib by Marmota 2022", 180, 450, 20, GOLD);
}

int main()
{
	InitWindow(800, 520, "Sokoban");
	Game* game = new Game;
	Rectangle rect{ 0, 0, 520, 520 }; // gameplay box
	int fps = 0;

	SetTargetFPS(60);

	game->Reset();
	game->resetModes();

	while (!WindowShouldClose())
	{
		// Update
		if (game->switch_screen == SCREEN_MENU)
		{
			if (IsKeyPressed(KEY_UP))
			{
				game->switch_menu--;
				if (game->switch_menu < 0)
				{
					game->switch_menu = 2;
				}
			}
			else if (IsKeyPressed(KEY_DOWN))
			{
				game->switch_menu++;
				if (game->switch_menu > 2)
				{
					game->switch_menu = 0;
				}
			}
			else if (IsKeyPressed(KEY_ENTER))
			{

				if (game->switch_menu == 0)
				{
					game->switch_screen = SCREEN_GAME;
					game->setMode(1);
					game->counter = 0;
					game->counter_timer = 0;
					game->level = 0;
					game->loadMap();
				}
				else if (game->switch_menu == 1)
				{
					game->switch_screen = SCREEN_EDITOR;
					game->Reset();
					game->counter = 0;
					game->counter_timer = 0;
					game->level = 0;
					game->setMode(0);
				}
				else if (game->switch_menu == 2)
				{
					game->switch_screen = SCREEN_ABOUT;
				}
			}
			BeginDrawing();
			ClearBackground(BLACK);
			DrawStartHUD(game);
			EndDrawing();
		}
		if (game->switch_screen == SCREEN_GAME)
		{
			fps++;
			if (fps == 60)
			{
				fps = 0;
				game->counter++;
				game->counter_timer++;
			}
			if (IsKeyPressed(KEY_UP))
			{
				game->playerMoveUp();
			}
			else if (IsKeyPressed(KEY_DOWN))
			{
				game->playerMoveDown();
			}
			else if (IsKeyPressed(KEY_LEFT))
			{
				game->playerMoveLeft();
			}
			else if (IsKeyPressed(KEY_RIGHT))
			{
				game->playerMoveRight();
			}
			else if (IsKeyPressed(KEY_R))
			{
				game->Reset();

				if (game->loadMap())
				{
					printf("Error load map\n");
					return 1;
				}
				printf("Map loaded\n");
			}
			else if (IsKeyPressed(KEY_Q))
			{
				game->Reset();
				game->switch_screen = SCREEN_MENU;
			}

			// Draw
			BeginDrawing();

			ClearBackground(BLACK);
			DrawHUD(game);
			DrawObjects(game);
			DrawRectangleLinesEx(rect, 2.f, RED);
			DrawRectangleRec(game->player, GREEN);

			EndDrawing();
		}
		else if (game->switch_screen == SCREEN_EDITOR)
		{
			if (IsKeyPressed(KEY_UP))
			{
				game->switch_mode == 0 ? game->moveUp() : game->playerMoveUp();
			}
			else if (IsKeyPressed(KEY_DOWN))
			{
				game->switch_mode == 0 ? game->moveDown() : game->playerMoveDown();
			}
			else if (IsKeyPressed(KEY_LEFT))
			{
				game->switch_mode == 0 ? game->moveLeft() : game->playerMoveLeft();
			}
			else if (IsKeyPressed(KEY_RIGHT))
			{
				game->switch_mode == 0 ? game->moveRight() : game->playerMoveRight();
			}
			else if (IsKeyPressed(KEY_W))
			{
				game->putWall();
			}
			else if (IsKeyPressed(KEY_B))
			{
				game->putBox();
			}
			else if (IsKeyPressed(KEY_G))
			{
				game->putGoal();
			}
			else if (IsKeyPressed(KEY_P))
			{
				game->putPlayer();
			}
			else if (IsKeyPressed(KEY_S))
			{
				if (game->saveMap())
				{
					printf("Error save map\n");
					return 1;
				}
				printf("Map saved\n");
			}
			else if (IsKeyPressed(KEY_SPACE))
			{
				game->switch_mode = game->switch_mode == 1 ? 0 : 1;
			}
			else if (IsKeyPressed(KEY_A)) // Clear all
			{
				game->Reset();
			}
			else if (IsKeyPressed(KEY_C)) // Clear selected tile
			{
				game->clearTile();
			}
			else if (IsKeyPressed(KEY_Q))
			{
				game->Reset();
				game->switch_screen = SCREEN_MENU;
			}

			BeginDrawing();

			ClearBackground(BLACK);
			DrawHUD(game);
			DrawObjects(game);
			DrawRectangleLinesEx(rect, 2.f, RED);
			DrawRectangleRec(game->player, GREEN);
			DrawRectangleLinesEx(game->editor, 1.f, GREEN);

			EndDrawing();
		}
		else if (game->switch_screen == SCREEN_ABOUT)
		{
			if (IsKeyPressed(KEY_Q))
			{
				game->switch_screen = SCREEN_MENU;
			}

			BeginDrawing();
			ClearBackground(BLACK);
			DrawAbout();
			EndDrawing();
		}

	}

	CloseWindow();

	delete game;
	return 0;
}