#pragma once

class GameModel;

class Collision
{
public:
	Collision() {};
	~Collision() {};

	static bool AABBCollision(GameModel& A, GameModel& B);

private:


};