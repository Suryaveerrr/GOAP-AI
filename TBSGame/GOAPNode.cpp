#include "GOAPNode.h"

GOAPNode::GOAPNode()
{
    g = 0;
    h = 0;
    id = 0;
}


GOAPNode::GOAPNode(WorldState _ws, int _g, int _h, int _id, int _parentID, AIOptionBase* _option)
{
    g = _g;
    h = _h;
    parentID = _parentID;
    ws = _ws;
    option = _option;
    id = _id;


}

/*
* the f value reutnrs the cost so far + heursitic distance to goal
*/
int GOAPNode::f()
{
	return g + h;
}

/*
* IsShorterThan will return true if the current node's f() cost is lower than the provided node's f() cost
*/
bool GOAPNode::IsShorterThan(const GOAPNode _other) const
{
    return (g + h) < (_other.g + _other.h);
}