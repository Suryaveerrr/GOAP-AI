#ifndef AI_PATH_NODE_RECORD_H
#define AI_PATH_NODE_RECORD_H

/*
CMP304/MAT501 AI Framework (2025)
*/

#pragma once
#include "AIPathConnection.h"

/*
* AIPathNodeRecord holds A*'s data for a node, based on the NodeRecord structure in AI for Games 
*
*   f = estimatedTotalCost = costSoFar (g) + heuristic estimate (h)
*/
struct AIPathNodeRecord
{
	int node = -1;					// node this record describes
	AIPathConnection connection;	// connection used to reach this node (unset for the start)
	float costSoFar = 0.f;			// g: cost of the best path found so far from the start
	float estimatedTotalCost = 0.f;	// f: costSoFar + heuristic estimate to the goal
};

#endif // !AI_PATH_NODE_RECORD_H
