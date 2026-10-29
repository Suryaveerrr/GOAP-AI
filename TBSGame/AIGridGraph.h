#ifndef AI_GRID_GRAPH_H
#define AI_GRID_GRAPH_H

/*
CMP304/MAT501 AI Framework (2025)
*/

#pragma once
#include <vector>
#include "AIPathGraphBase.h"

/*
* AIGridGraph is a 2D tile grid presented as a graph
*
* - Each cell is a node with index (y * width + x).
* - Movement is 4-way: up, down, left, right only, diagaonal movement is not possible
*/
class AIGridGraph :
	public AIPathGraphBase
{
public:
	
	bool Init(int _width, int _height);

	// AIPathGraphBase interface
	std::vector<AIPathConnection> GetConnections(int _fromNode) const override;
	AIMath_Global::Vector2f GetNodePosition(int _node) const override;
	bool IsValidNode(int _node) const override;

	// grid function
	void SetWalkable(int _x, int _y, bool _walkable);
	bool IsWalkable(int _x, int _y) const;
	bool IsInBounds(int _x, int _y) const;

	// conversion between grid coordinates and node index
	int GridToNode(int _x, int _y) const;
	int GetNodeX(int _node) const;
	int GetNodeY(int _node) const;

	int GetWidth() const;
	int GetHeight() const;

	static constexpr float STEP_COST = 1.f;

private:
	int width = 0;
	int height = 0;
	std::vector<bool> walkable;	
};

#endif // !AI_GRID_GRAPH_H
