#include "AIPathHeuristic.h"

bool AIPathHeuristicBase::Init(const AIPathGraphBase& _graph, int _goalNode)
{
	graph = &_graph;
	goalNode = _goalNode;
	return true;
}

int AIPathHeuristicBase::GetGoalNode() const
{
	return goalNode;
}

float AIEuclideanHeuristic::Estimate(int _fromNode) const
{
	return AIMath_Global::getInstance()->CalcDistance(graph->GetNodePosition(_fromNode), graph->GetNodePosition(goalNode));
}
