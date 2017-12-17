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
	brd ( gfx ),
	snake(brd,rng),
	xAppleDist( 0, brd.GetWidth() -1 ),
	yAppleDist(0, brd.GetHeight() - 1),
	apple(brd, {xAppleDist(rng),yAppleDist(rng) }, rng)
	
{
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
	snake.Update();
	snake.Control(wnd);
	snake.testCollision(true);
	if (snake.testCollision(brd))
	{
		snake.Respawn(true);
	}
	if (snake.testCollision(apple))
	{
		snake.Grow();
		apple.Respawn();
	}
}

void Game::ComposeFrame()
{
	brd.DrawBorder();
	apple.Draw();
	snake.Draw();
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
