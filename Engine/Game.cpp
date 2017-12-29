/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Colors.h"
#include "Snake.h"
#include <assert.h>
#include "Surface.h"
#include "Rect.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	rng( std::random_device()() ),
	frame_time(std::chrono::steady_clock::now()),
	config_txt(config_file_name),
	start_data(config_txt.getGameData()),
	brd ( gfx, rng, config_txt.getBoardData()),
	snake(brd,rng, config_txt.getSnakeData()),
	font("Inconsolata-ASCII-BOLD.bmp"),
	incon(&font, gfx),
	start_button_color(0x10222B),
	score_button_color(0x10222B)
{
	brd.SpawnObjects(Board::Cell::Apple, start_data.amount_apples);
	brd.SpawnObjects(Board::Cell::Obstacle, start_data.amount_obstacles);
	brd.SpawnObjects(Board::Cell::Poison, start_data.amount_poison);
	font.SetChroma(Colors::White);
	font.SetSubstituteColor(Colors::Gray);
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (!start)
	{
		//Start button hover
		if ((wnd.mouse.GetPosX() > 310) &&
			(wnd.mouse.GetPosX() < 310 + 36 * 5) &&
			(wnd.mouse.GetPosY() > 220) &&
			(wnd.mouse.GetPosY() < 220 + 72))
		{
			if (wnd.mouse.LeftIsPressed())
			{
				start = true;
			}
			start_button_color = 0x29576E;
		}
		else
		{
			start_button_color = 0x10222B;
		}


		//Score button hover
		if ((wnd.mouse.GetPosX() > 310) &&
			(wnd.mouse.GetPosX() < 310 + 36 * 5) &&
			(wnd.mouse.GetPosY() > 350) &&
			(wnd.mouse.GetPosY() < 350 + 72))
		{
			score_button_color = 0x29576E;
		}
		else
		{
			score_button_color = 0x10222B;
		}

	}
	else
	{
		if (!gameOver)
		{
			if (wnd.kbd.KeyIsPressed('W'))
			{
				snake.MoveByDelta(Snake::Direction::Up);
			}
			else if (wnd.kbd.KeyIsPressed('S'))
			{
				snake.MoveByDelta(Snake::Direction::Down);
			}
			else if (wnd.kbd.KeyIsPressed('A'))
			{
				snake.MoveByDelta(Snake::Direction::Left);
			}
			else if (wnd.kbd.KeyIsPressed('D'))
			{
				snake.MoveByDelta(Snake::Direction::Right);
			}
			snake.UpdateDelta();
			if (snake.onBoard())
			{
				switch (brd.testLocation(snake.GetNextLocation()))
				{
				case Board::Cell::Apple:
					snake.Grow();
					start_data.score++;
					brd.RespawnObject(snake.GetNextLocation());
					break;
				case Board::Cell::Poison:
					snake.SpeedUp(start_data.poison_acceleration_ratio);
					brd.RespawnObject(snake.GetNextLocation());
					break;
				case Board::Cell::Obstacle:
					gameOver = true;
					break;
				}

				if (snake.isOnTheTail())
				{
					start_data.score = snake.EatYourself();
				}

				if (!gameOver)
				{
					snake.Update();
				}
			}
			else
			{
				gameOver = true;
			}
		}
		else if (wnd.kbd.KeyIsPressed('R'))
		{
			start_data.score = 0;
			gameOver = false;
			snake.Respawn();
			brd.Respawn();
			start = false;
		}
	}
}

void Game::ComposeFrame()
{
	if (!start)
	{
		gfx.DrawRect(0, 0, Graphics::ScreenWidth - 1, Graphics::ScreenHeight - 1, 0x95AB63);
		gfx.DrawRect( 310, 220, 36*5, 72, start_button_color);
		gfx.DrawRect( 310, 350, 36 * 5, 72, score_button_color);
		incon.print("Start", { 310, 220 }, 0xE77F24, 10);
		incon.print("Score", { 310, 350 }, 0xE77F24, 10);
	}
	else
	{
		brd.Draw();
		brd.DrawBorder();
		snake.Draw();
		gfx.PrintInt(start_data.score, 0, 0, start_data.score_color, 4, 3);
	}
	ShowFPS();
}

void Game::ShowFPS()
{
	std::chrono::duration<float> _frame_time = std::chrono::steady_clock::now() - frame_time;
	int time_frame = int(_frame_time.count()*1000);
	int fps = int(1 / _frame_time.count());
	frame_time = std::chrono::steady_clock::now();
	int i = 0;
	while ((fps != 0) && (i > -790))
	{
		i -= 15;;
		gfx.PrintDigit(fps % 10, 790 + i, 10, 4, { 255,146,50 });
		fps /= 10;
	}
	i = 0;
	while ((time_frame != 0) && (i > -790))
	{
		i -= 8;;
		gfx.PrintDigit(time_frame % 10, 790 + i, 40, 2, Colors::Green);
		time_frame /= 10;
	}
}
