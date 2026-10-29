#ifndef AI_PATH_GRAPH_BASE_H
#define AI_PATH_GRAPH_BASE_H

/*
CMP304/MAT501 AI Framework (2025)
*/

#pragma once
#include <vector>
#include "AIPathConnection.h"
#include "../AI_Framework/Global/AIMath_Global.h"

/*
* AIPathGraphBase is the interface a pathfinder uses to explore a graph based on the Graph class in AI for Games
*/
class AIPathGraphBase
{
public:
	// all outgoing connections from the given node
	virtual std::vector<AIPathConnection> GetConnections(int _fromNode) const = 0;

	// world/grid position of a node, used by heuristics
	virtual AIMath_Global::Vector2f GetNodePosition(int _node) const = 0;

	// true if the node exists and char can vist
	virtual bool IsValidNode(int _node) const = 0;

	virtual ~AIPathGraphBase() = default;
};

#endif // !AI_PATH_GRAPH_BASE_H
