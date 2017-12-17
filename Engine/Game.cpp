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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	rng( std::random_device()() ),
	frame_time(std::chrono::steady_clock::now()),
	brd ( gfx, rng ),
	snake(brd,rng)
{
	brd.SpawnObjects(Board::Cell::Apple, amount_apples);
	brd.SpawnObjects(Board::Cell::Obstacle, amount_obstacles);
	brd.SpawnObjects(Board::Cell::Poison, amount_poison);
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
	if (!gameOver)
	{
		snake.Control(wnd);
		snake.Update();

		if (!snake.onBoard())
		{
			gameOver = true;
		}
		else
		{
			switch (snake.testCollision())
			{
			case Board::Cell::Apple:
				snake.Grow();
				score++;
				brd.RespawnObject(snake.GetCurrentLocation());
				break;
			case Board::Cell::Obstacle:
				gameOver = true;
				break;
			case Board::Cell::Poison:
				snake.SpeedUp(poison_acceleration_ratio);
				brd.RespawnObject(snake.GetCurrentLocation());
				break;
			case Board::Cell::Snake:
				snake.EatYourself();
				break;
			}
			snake.Pass();
		}
	}
	else if (wnd.kbd.KeyIsPressed('R'))
	{
		score = 0;
		gameOver = false;
		snake.Respawn();
		brd.Respawn();
	}
}

void Game::ComposeFrame()
{
	brd.DrawBorder();
	brd.Draw();
	PrintScore(40,0);
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

void Game::PrintScore(const int x, const int y)
{
	int i = 0;
	int buff = score;
	if (buff == 0)
	{
		gfx.PrintDigit(buff, x + i, y + 10, 4, { 255,255,255 });
	}
	while ((buff != 0))
	{
		i -= 15;;
		gfx.PrintDigit(buff % 10, x + i, y + 10, 4, { 255,255,255 });
		buff /= 10;
	}
}
