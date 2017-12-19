#pragma once
#include "Snake.h"
#include "StartData.h"

class ConfigParser
{
public:
	ConfigParser(const std::string& filename);
	Snake::StartData getSnakeData() const;
	Board::StartData getBoardData() const;
	StartData        getGameData() const;
private:
	void parseGameData(const std::string& buffer);
	void parseSnakeData(const std::string& buffer);
	void parseBoardData(const std::string& buffer);
	std::string getVarNameFromString(const std::string& string) const;
	std::vector<std::string> getDataFromString(const std::string& string) const;
private:
	Snake::StartData _snake_data;
	Board::StartData _brd_data;
	StartData _gm_data;
};