#include "stdafx.h"
#include "Collision.h"
#include "Objects\GameModel.h"


bool Collision::AABBCollision(GameModel & A, GameModel & B)
{
	std::vector<D3DXVECTOR3> pointsA, pointsB;
	D3DXVECTOR3 centerA = A.Position(), centerB = B.Position(), projection;
	float radiusA, radiusB;

	A.Radius(radiusA);
	B.Radius(radiusB);
	projection = centerB - centerA;

	bool b = true;	//or¿œ ∂© false
	A.GetAABB(pointsA);
	B.GetAABB(pointsB);

	b &= (pointsA[0].x <= pointsB[7].x) && (pointsA[7].x >= pointsB[0].x);
	b &= (pointsA[0].z <= pointsB[7].z) && (pointsA[7].z >= pointsB[0].z);
	//b &= (pointsA[0].y <= pointsB[7].y) && (pointsA[7].y >= pointsB[0].y);

	if (b) return true;

	return false;
}
