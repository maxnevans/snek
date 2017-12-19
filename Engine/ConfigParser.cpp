#include "ConfigParser.h"
#include <string>
#include <fstream>

ConfigParser::ConfigParser(const std::string & filename)
{
	std::ifstream file(filename);
	if (file)
	{
		for (std::string line; std::getline(file, line); )
		{
			if (line.empty())
			{
				continue;
			}
			else if (line == "[GAME]")
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					if (buffer == "[END]")
					{
						break;
					}
					else if (buffer.empty())
					{
						continue;
					}
					else if ((buffer[0] == '/') && (buffer[1] == '/'))
					{
						continue;
					}
					else
					{
						parseGameData(buffer);
					}
				}
			}
			else if (line == "[SNAKE]")
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					if (buffer == "[END]")
					{
						break;
					}
					else if (buffer.empty())
					{
						continue;
					}
					else if ((buffer[0] == '/') && (buffer[1] == '/'))
					{
						continue;
					}
					else
					{
						parseSnakeData(buffer);
					}
				}
			}
			else if (line == "[BOARD]")
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					if (buffer == "[END]")
					{
						break;
					}
					else if (buffer.empty())
					{
						continue;
					}
					else if ((buffer[0] == '/') && (buffer[1] == '/'))
					{
						continue;
					}
					else
					{
						parseBoardData(buffer);
					}
				}
			}
		}
	}
	else
	{
		throw std::runtime_error("Failed to open config file!");
	}
}

Snake::StartData ConfigParser::getSnakeData() const
{
	return _snake_data;
}

Board::StartData ConfigParser::getBoardData() const
{
	return _brd_data;
}

StartData ConfigParser::getGameData() const
{
	return _gm_data;
}

void ConfigParser::parseGameData(const std::string& buffer)
{
	std::string var_name = getVarNameFromString(buffer);
	std::vector<std::string> data_from_string = getDataFromString(buffer);

	if (var_name == "amount_apples")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_gm_data.amount_apples = std::stoi(data_from_string.front());
	}
	else if (var_name == "amount_obstacles")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_gm_data.amount_obstacles = std::stoi(data_from_string.front());
	}
	else if (var_name == "amount_poison")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_gm_data.amount_poison = std::stoi(data_from_string.front());
	}
	else if (var_name == "poison_acceleration_ratio")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_gm_data.poison_acceleration_ratio = std::stof(data_from_string.front());
	}
	else if (var_name == "score_color")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 3)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_gm_data.score_color.SetR(std::stoi(data_from_string[0]));
		_gm_data.score_color.SetG(std::stoi(data_from_string[1]));
		_gm_data.score_color.SetB(std::stoi(data_from_string[2]));
	}
	else if (var_name == "score")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_gm_data.score = std::stoi(data_from_string.front());
	}
}

void ConfigParser::parseSnakeData(const std::string& buffer)
{
	std::string var_name = getVarNameFromString(buffer);
	std::vector<std::string> data_from_string = getDataFromString(buffer);

	if (var_name == "start_location")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 2)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_snake_data.location.x = std::stoi(data_from_string[0]);
		_snake_data.location.y = std::stoi(data_from_string[1]);
	}
	else if (var_name == "start_direction")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		std::string direction_def = data_from_string.front();


		if (direction_def == "up")
		{
			_snake_data.direction = Snake::Direction::Up;
		}
		else if (direction_def == "down")
		{
			_snake_data.direction = Snake::Direction::Down;
		}
		else if (direction_def == "left")
		{
			_snake_data.direction = Snake::Direction::Left;
		}
		else if (direction_def == "right")
		{
			_snake_data.direction = Snake::Direction::Right;

		}
	}
	else if (var_name == "start_size")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_snake_data.size = std::stoi(data_from_string.front());
	}
	else if (var_name == "start_speed")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_snake_data.speed = std::stof(data_from_string.front());
	}
	else if (var_name == "head_color")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 3)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_snake_data.head_color.SetR(std::stoi(data_from_string[0]));
		_snake_data.head_color.SetG(std::stoi(data_from_string[1]));
		_snake_data.head_color.SetB(std::stoi(data_from_string[2]));
	}
	else if (var_name == "tail_size")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_snake_data.tail_draw_size_padding = std::stoi(data_from_string.front());
	}
	else if (var_name == "head_size")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_snake_data.head_draw_size_padding = std::stoi(data_from_string.front());
	}
}

void ConfigParser::parseBoardData(const std::string & buffer)
{
	std::string var_name = getVarNameFromString(buffer);
	std::vector<std::string> data_from_string = getDataFromString(buffer);

	if (var_name == "width")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.width = std::stoi(data_from_string.front());
	}
	else if (var_name == "height")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.height = std::stoi(data_from_string.front());
	}
	else if (var_name == "border_color ")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 3)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.borderColor.SetR(std::stoi(data_from_string[0]));
		_brd_data.borderColor.SetG(std::stoi(data_from_string[1]));
		_brd_data.borderColor.SetB(std::stoi(data_from_string[2]));
	}
	else if (var_name == "border_padding")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.borderPadding = std::stoi(data_from_string.front());
	}
	else if (var_name == "border_thickness")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.borderThickness = std::stoi(data_from_string.front());
	}
	else if (var_name == "cell_size")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.dim = std::stoi(data_from_string.front());
	}
	else if (var_name == "apple_color")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 3)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.contentsColors[0].SetR(std::stoi(data_from_string[0]));
		_brd_data.contentsColors[0].SetG(std::stoi(data_from_string[1]));
		_brd_data.contentsColors[0].SetB(std::stoi(data_from_string[2]));
	}
	else if (var_name == "obstacle_color")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 3)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.contentsColors[1].SetR(std::stoi(data_from_string[0]));
		_brd_data.contentsColors[1].SetG(std::stoi(data_from_string[1]));
		_brd_data.contentsColors[1].SetB(std::stoi(data_from_string[2]));
	}
	else if (var_name == "poison_color")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 3)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.contentsColors[2].SetR(std::stoi(data_from_string[0]));
		_brd_data.contentsColors[2].SetG(std::stoi(data_from_string[1]));
		_brd_data.contentsColors[2].SetB(std::stoi(data_from_string[2]));
	}
	else if (var_name == "apple_size")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.contentsPadding[0] = std::stoi(data_from_string.front());
	}
	else if (var_name == "obstacle_size")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.contentsPadding[1] = std::stoi(data_from_string.front());
	}
	else if (var_name == "poison_size")
	{
		if (data_from_string.size() < 1)
		{
			throw std::runtime_error("No data for " + var_name);
		}
		else if (data_from_string.size() > 1)
		{
			throw std::runtime_error("Too much data for " + var_name);
		}
		_brd_data.contentsPadding[2] = std::stoi(data_from_string.front());
	}
}

std::string ConfigParser::getVarNameFromString(const std::string& string) const
{
	std::string buffer;
	for (int i = 0; (i < string.size()) && (string[i] != '=') && (string[i] != ' '); i++)
	{
		buffer.push_back(string[i]);
	}
	return buffer;
}

std::vector<std::string> ConfigParser::getDataFromString(const std::string& string) const
{
	std::string buffer = string;
	std::vector<std::string> _rev_backData;
	for (int i = static_cast<int>(string.size()) - 1; i >= 0 && string[i] == ' '; i--)
	{
		buffer.erase(i, 1);
	}
	std::string _temp;
	for (int i = static_cast<int>(buffer.size()) - 1; i >= 0 && buffer[i] != '='; i--)
	{
		if (buffer[i] == ' ')
		{
			std::string _temp_reverse;
			for (int j = static_cast<int>(_temp.size()) - 1; j >= 0; j--)
			{
				_temp_reverse.push_back(_temp[j]);
			}
			_rev_backData.push_back(_temp_reverse);
			_temp.clear();
			continue;
		}
		_temp.push_back(buffer[i]);
	}
	std::vector<std::string> backData;
	for (int i = static_cast<int>(_rev_backData.size()) - 1; i >= 0; i--)
	{
		backData.emplace_back(_rev_backData[i]);
	}
	return backData;
}