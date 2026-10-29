#include "AIAStarPathfinder.h"
#include "AIPathfindingList.h"
#include <algorithm>
#include <iostream>

bool AIAStarPathfinder::FindPath(const AIPathGraphBase& _graph, int _start, int _goal,
	const AIPathHeuristicBase& _heuristic, std::vector<AIPathConnection>& _outPath)
{
	_outPath.clear();
	nodesExpanded = 0;

	if (!_graph.IsValidNode(_start) || !_graph.IsValidNode(_goal))
	{
		std::cout << "WARNING [AIAStarPathfinder] : Start or goal node is out of bounds or blocked" << std::endl;
		return false;
	}

	if (_heuristic.GetGoalNode() != _goal)
	{
		std::cout << "WARNING [AIAStarPathfinder] : Heuristic goal does not match requested goal" << std::endl;
		return false;
	}

	// initialise the record for the start node
	AIPathNodeRecord startRecord;
	startRecord.node = _start;
	startRecord.costSoFar = 0.f;
	startRecord.estimatedTotalCost = _heuristic.Estimate(_start);

	AIPathfindingList open;
	AIPathfindingList closed;
	open.Add(startRecord);

	AIPathNodeRecord current;

	// iterate through processing each node
	while (!open.IsEmpty())
	{
		// find the most promising open node
		current = open.SmallestElement();

		// stop when the goal is the cheapest open node
		if (current.node == _goal)
		{
			break;
		}

		nodesExpanded++;

		for (const AIPathConnection& connection : _graph.GetConnections(current.node))
		{
			const int endNode = connection.GetToNode();
			const float endNodeCost = current.costSoFar + connection.GetCost();

			AIPathNodeRecord endNodeRecord;
			float endNodeHeuristic = 0.f;

			if (AIPathNodeRecord* closedRecord = closed.Find(endNode))
			{
				// already closed: skip unless this route is cheaper
				if (closedRecord->costSoFar <= endNodeCost)
				{
					continue;
				}

				endNodeRecord = *closedRecord;
				closed.Remove(endNode);

				// reuse the stored heuristic rather than recalculating
				endNodeHeuristic = endNodeRecord.estimatedTotalCost - endNodeRecord.costSoFar;
			}
			else if (AIPathNodeRecord* openRecord = open.Find(endNode))
			{
				// already open: skip unless this route is cheaper
				if (openRecord->costSoFar <= endNodeCost)
				{
					continue;
				}

				endNodeRecord = *openRecord;
				open.Remove(endNode);	// re-added below with updated values

				endNodeHeuristic = endNodeRecord.estimatedTotalCost - endNodeRecord.costSoFar;
			}
			else
			{
				// unvisited node: make a new record
				endNodeRecord.node = endNode;
				endNodeHeuristic = _heuristic.Estimate(endNode);
			}

			// update the record and put it on the open list
			endNodeRecord.costSoFar = endNodeCost;
			endNodeRecord.connection = connection;
			endNodeRecord.estimatedTotalCost = endNodeCost + endNodeHeuristic;
			open.Add(endNodeRecord);
		}

		// finished with this node's connections: move it to closed
		open.Remove(current.node);
		closed.Add(current);
	}

	// open list exhausted without reaching the goal: no path
	if (current.node != _goal)
	{
		return false;
	}

	// work back along the connections to the start
	while (current.node != _start)
	{
		_outPath.push_back(current.connection);

		AIPathNodeRecord* previous = closed.Find(current.connection.GetFromNode());
		if (previous == nullptr)
		{
			std::cout << "WARNING [AIAStarPathfinder] : Broken path chain during reconstruction" << std::endl;
			_outPath.clear();
			return false;
		}
		current = *previous;
	}

	std::reverse(_outPath.begin(), _outPath.end());
	return true;
}

int AIAStarPathfinder::GetNodesExpanded() const
{
	return nodesExpanded;
}

std::vector<int> AIAStarPathfinder::ConnectionsToNodes(const std::vector<AIPathConnection>& _path, int _start)
{
	std::vector<int> nodes;
	nodes.push_back(_start);

	for (const AIPathConnection& connection : _path)
	{
		nodes.push_back(connection.GetToNode());
	}

	return nodes;
}
