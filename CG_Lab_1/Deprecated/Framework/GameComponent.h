#pragma once

class Game;

class GameComponent
{
public:
	Game* game_;
public:
	virtual void Update() = 0;
	virtual void Init() = 0;
};

