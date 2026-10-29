#pragma once
#include "../../AI_Framework/framework.h"

#include "GOAPNode.h"
#include "AIActor_Unit.h"

class GOAP_Reasoner :
    public AIReasonerBase
{
public:

 
    ~GOAP_Reasoner();

    // AIReasonerBase overrides
    bool Init(std::string _id, AIBrainBlackboardBase& _context) override;
    void Sense() override;
    void Think() override;
    ActionStatus Act() override;

    // GOAP function
    bool DoesStateMeetGoal(WorldState _worldState);
    void InitPlanningDatabase(AIActorBase& _planningContext);

private:
    //Global Access
    AIRandom_Global* Global_Random;
    AIMath_Global* Global_Math;

    //thePlan will store the Options produced by the GOAP planner, which the AIActor will use when execuring the plan
    std::vector<AIOptionBase*> thePlan;
    // the currnet plan index being performed
    int currentPlanIndex = 0;

    bool hasPlan = false;
   

    // tracking
    WorldState goalState;
    WorldState curState;

    // A* open and close lists
    std::vector<GOAPNode> open;
    std::vector<GOAPNode> closed;

    // for GOAP ,we need a link to the Actor for the planning state so we can simulate the outcomes of Actions the Actor will take
    AIActor_Unit* actor;

    // A* functions
    WorldState GetNextState(AIOptionBase* _option);
    GOAPNode* IsMemberOfClosed(WorldState _ws);
    GOAPNode* IsMemberOfOpen(WorldState _ws);
    void AddToOpenList(GOAPNode& _node);
    void SortOpenList();
    int GetOpenIndex(int _id);
    GOAPNode* PopAndClose();

    // GOAP functions
    void GOAPPlanner();
    int CalculateHeuristic(WorldState _now, WorldState _goal);




};

