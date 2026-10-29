#ifndef AI_PATH_HEURISTIC_H
#define AI_PATH_HEURISTIC_H

/*
CMP304/MAT501 AI Framework (2025)
*/

#pragma once
#include "AIPathGraphBase.h"

/*
* AIPathHeuristicBase estimates the remaining cost from a node to a fixed goal node, based on the Heuristic class in AI For Games
*/
class AIPathHeuristicBase
{
public:
	virtual bool Init(const AIPathGraphBase& _graph, int _goalNode);

	// estimated cost from _fromNode to the goal node
	virtual float Estimate(int _fromNode) const = 0;

	int GetGoalNode() const;

	virtual ~AIPathHeuristicBase() = default;

protected:
	const AIPathGraphBase* graph = nullptr;	// non-owning
	int goalNode = -1;
};


/*
* AIEuclideanHeuristic - straight-line distance to the goal.
*/
class AIEuclideanHeuristic :
	public AIPathHeuristicBase
{
public:
	float Estimate(int _fromNode) const override;
};

#endif // !AI_PATH_HEURISTIC_H
