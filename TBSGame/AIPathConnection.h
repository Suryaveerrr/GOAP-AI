#ifndef AI_PATH_CONNECTION_H
#define AI_PATH_CONNECTION_H

/*
CMP304/MAT501 AI Framework (2025)
*/

#pragma once

/*
* AIPathConnection is a pathfinding graph AI for Games Pathfinding example data structure
*
* Nodes are identified by an integer index. 
*/
class AIPathConnection
{
public:
	AIPathConnection() = default;
	AIPathConnection(int _fromNode, int _toNode, float _cost)
		: fromNode(_fromNode), toNode(_toNode), cost(_cost) {}

	//  cost of travelling along this connection
	float GetCost() const { return cost; }

	// node this connection came from
	int GetFromNode() const { return fromNode; }

	// node this connection leads to
	int GetToNode() const { return toNode; }

private:
	int fromNode = -1;	// use -1 where no node, such as the start node
	int toNode = -1;
	float cost = 0.f;
};

#endif // !AI_PATH_CONNECTION_H
