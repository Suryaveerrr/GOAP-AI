#pragma once

/*
CMP304/MAT501 AI Framework (2026)
*/

#include <vector>
#include <string>
#include "../AI_Framework/framework.h"


/*
WorldState stores the games current state for the GOAP planner
State is recorded a Key Value Pairs using std::map, with a boolean value
*/
struct WorldState {
    // the map of game state
   std::map<std::string, bool> state;

   // check for equal states
   bool operator==(const  WorldState& w) const
   {

       for (auto s = w.state.begin(); s != w.state.end(); ++s)
       {
           if (!state.contains(s->first))
           {
               return false;
           }


           if (s->second != state.find(s->first)->second)
           {
               return false;
           }

       }

       return true;
   }

    /*
    * DistanceTo will calcuate the GOAP distance from this state to the provided goal state
    */
    int DistanceTo(WorldState _goalState)
    {

        /*
            TASK TODO: implement a GOAP distance calculate whose score is 1 for every key different to the goal state

            WorldState for this node is in 'state'
            WorldState for the goal node is in '_goalState.state'

            WorldState is recorded a Key Value Pairs using std::map, with a boolean value
            Access state.first() to get the Key (e.g "HasWater")
            Access state.second() to get the Value (e.g. true)

            Function should return an integer 
            
                
        */


        int result = 0;

        // score 1 for every goal key where this state differs (or lacks the key)
        for (auto& goalKey : _goalState.state)
        {
            auto it = state.find(goalKey.first);

            if (it == state.end() || it->second != goalKey.second)
            {
                result++;
            }
        }

        return result;
    }

};


/*
GOAPNode contains the data for the A* pathfinding
*/
class GOAPNode
{
public:
    GOAPNode();

    GOAPNode(WorldState _ws, int _g, int _h, int _id, int _parentID, AIOptionBase* _option);

    // ID's for the Node
     int id;
     int parentID;

    //Pathfinding vars
     int g; // The A* cost from 'start' to 'here'
     int h; // The estimated remaining cost to 'goal' form 'here'

    // This Node's world state and Option that was applied to reach this game state
     WorldState ws;
     AIOptionBase* option;

     int f(); // g + h

     bool IsShorterThan(const GOAPNode _other) const;

};

