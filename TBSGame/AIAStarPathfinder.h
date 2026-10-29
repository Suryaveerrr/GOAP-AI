#ifndef AI_ASTAR_PATHFINDER_H
#define AI_ASTAR_PATHFINDER_H

/*
CMP304/MAT501 AI Framework (2025)
*/

#pragma once
#include <vector>
#include "AIPathGraphBase.h"
#include "AIPathHeuristic.h"

/*
* AIAStarPathfinder implements the A* algorithm from  AI for Games using AIPathfindingList for the open and closed lists.
*/
class AIAStarPathfinder
{
public:
	/*
	* Finds a path from _start to _goal.
	* _heuristic must already be initialised with the same graph and _goal.
	* On success returns true and fills _outPath with the connections from start to goal in order (empty if _start == _goal). 
	*  On failure returns false and _outPath is empty.
	*/
	bool FindPath(const AIPathGraphBase& _graph, int _start, int _goal,
		const AIPathHeuristicBase& _heuristic, std::vector<AIPathConnection>& _outPath);

	// number of nodes expanded by the last FindPath call 
	int GetNodesExpanded() const;

	// converts a connection path into the list of nodes visited, including _start.
	static std::vector<int> ConnectionsToNodes(const std::vector<AIPathConnection>& _path, int _start);

private:
	int nodesExpanded = 0;
};

#endif // !AI_ASTAR_PATHFINDER_H
