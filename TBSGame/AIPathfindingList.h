#ifndef AI_PATHFINDING_LIST_H
#define AI_PATHFINDING_LIST_H

/*
CMP304/MAT501 AI Framework (2025)
*/

#pragma once
#include <vector>
#include "AIPathNodeRecord.h"

/*
* AIPathfindingList is used for both the open and closed lists in A*.
* Based on the PathfindingList structure AI for Games (Ch. 4),
*
*/
class AIPathfindingList
{
public:
	void Add(const AIPathNodeRecord& _record);
	void Remove(int _node);
	bool Contains(int _node) const;

	// returns nullptr if the node is not in the list.
	AIPathNodeRecord* Find(int _node);

	// record with the lowest estimatedTotalCost. The list should not be empty.
	AIPathNodeRecord SmallestElement() const;

	bool IsEmpty() const;
	size_t Size() const;

private:
	std::vector<AIPathNodeRecord> records;
};

#endif // !AI_PATHFINDING_LIST_H
