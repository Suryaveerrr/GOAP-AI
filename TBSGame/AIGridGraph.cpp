#include "AIGridGraph.h"
#include <iostream>

bool AIGridGraph::Init(int _width, int _height)
{
	if (_width <= 0 || _height <= 0)
	{
		std::cout << "WARNING [AIGridGraph] : Invalid grid size " << _width << " x " << _height << std::endl;
		return false;
	}

	width = _width;
	height = _height;
	walkable.assign(width * height, true);
	return true;
}

std::vector<AIPathConnection> AIGridGraph::GetConnections(int _fromNode) const
{
	std::vector<AIPathConnection> connections;

	if (!IsValidNode(_fromNode))
	{
		return connections;
	}

	const int x = GetNodeX(_fromNode);
	const int y = GetNodeY(_fromNode);

	// 4-way neighbours, in order: up, down, left, right
	const int offsetX[4] = { 0, 0, -1, 1 };
	const int offsetY[4] = { -1, 1, 0, 0 };

	for (int i = 0; i < 4; i++)
	{
		const int nx = x + offsetX[i];
		const int ny = y + offsetY[i];

		if (IsInBounds(nx, ny) && IsWalkable(nx, ny))
		{
			connections.push_back(AIPathConnection(_fromNode, GridToNode(nx, ny), STEP_COST));
		}
	}

	return connections;
}

AIMath_Global::Vector2f AIGridGraph::GetNodePosition(int _node) const
{
	return AIMath_Global::Vector2f((float)GetNodeX(_node), (float)GetNodeY(_node));
}

bool AIGridGraph::IsValidNode(int _node) const
{
	if (_node < 0 || _node >= width * height)
	{
		return false;
	}

	return walkable[_node];
}

void AIGridGraph::SetWalkable(int _x, int _y, bool _walkable)
{
	if (!IsInBounds(_x, _y))
	{
		std::cout << "WARNING [AIGridGraph] : SetWalkable out of bounds (" << _x << ", " << _y << ")" << std::endl;
		return;
	}

	walkable[GridToNode(_x, _y)] = _walkable;
}

bool AIGridGraph::IsWalkable(int _x, int _y) const
{
	return IsInBounds(_x, _y) && walkable[GridToNode(_x, _y)];
}

bool AIGridGraph::IsInBounds(int _x, int _y) const
{
	return _x >= 0 && _x < width && _y >= 0 && _y < height;
}

int AIGridGraph::GridToNode(int _x, int _y) const
{
	return _y * width + _x;
}

int AIGridGraph::GetNodeX(int _node) const
{
	return _node % width;
}

int AIGridGraph::GetNodeY(int _node) const
{
	return _node / width;
}

int AIGridGraph::GetWidth() const
{
	return width;
}

int AIGridGraph::GetHeight() const
{
	return height;
}
