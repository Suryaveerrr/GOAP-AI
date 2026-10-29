#include "AIPathfindingList.h"

void AIPathfindingList::Add(const AIPathNodeRecord& _record)
{
	records.push_back(_record);
}

void AIPathfindingList::Remove(int _node)
{
	for (size_t i = 0; i < records.size(); i++)
	{
		if (records[i].node == _node)
		{
			records.erase(records.begin() + i);
			return;
		}
	}
}

bool AIPathfindingList::Contains(int _node) const
{
	for (const auto& record : records)
	{
		if (record.node == _node)
		{
			return true;
		}
	}
	return false;
}

AIPathNodeRecord* AIPathfindingList::Find(int _node)
{
	for (auto& record : records)
	{
		if (record.node == _node)
		{
			return &record;
		}
	}
	return nullptr;
}

AIPathNodeRecord AIPathfindingList::SmallestElement() const
{
	// ties go to the earliest-added record
	size_t smallest = 0;
	for (size_t i = 1; i < records.size(); i++)
	{
		if (records[i].estimatedTotalCost < records[smallest].estimatedTotalCost)
		{
			smallest = i;
		}
	}
	return records[smallest];
}

bool AIPathfindingList::IsEmpty() const
{
	return records.empty();
}

size_t AIPathfindingList::Size() const
{
	return records.size();
}
