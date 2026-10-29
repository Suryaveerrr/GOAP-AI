#include "GOAP_Reasoner.h"

#include "AIActor_Unit.h"

#include <algorithm>


GOAP_Reasoner::~GOAP_Reasoner()
{


}

// AIReasonerBase overrides

bool GOAP_Reasoner::Init(std::string _id, AIBrainBlackboardBase& _context) {
	actorBlackboard = &_context;
	reasonerID = _id;

	// obtain Math and Random singletons
	Global_Random = AIRandom_Global::getInstance();
	Global_Math = AIMath_Global::getInstance();



	return true;
}

void GOAP_Reasoner::Sense()
{
	// set the goal state - for now hard coded, but could be provided to AI Actor depending on gameplay 
	goalState.state["HasHighHealth"] = true;
	goalState.state["HasSword"] = true;
	goalState.state["AtCommander"] = true;


}

/*
printWorldState will print a world state - useful for debuggin
*/
void printWorldState(WorldState _ws)
{
	std::cout << "World State " << std::endl;
	for (auto s : _ws.state)
	{
		std::cout << "\t" << s.first << " is " << s.second << std::endl;
	}
}


// Initialisation

void GOAP_Reasoner::InitPlanningDatabase(AIActorBase& _planningContext)
{

	actor = static_cast<AIActor_Unit*>(&_planningContext);
}




/*
* DoesStateMeetGoal will return true if the provided World State meets the goalState, otherwise false
*/
bool GOAP_Reasoner::DoesStateMeetGoal(WorldState _worldState)
{

	for (auto& s : goalState.state)
	{

		if (_worldState.state[s.first] != s.second)
			return false;
	}


	return true;
}


/*

A* Functions

*/
void GOAP_Reasoner::AddToOpenList(GOAPNode& _node)
{
	open.push_back(_node);
	SortOpenList();
}


void GOAP_Reasoner::SortOpenList()
{
	std::sort(open.begin(), open.end(),
		[](const GOAPNode a, const GOAPNode b) {
			return a.IsShorterThan(b);
		});
}


int GOAP_Reasoner::GetOpenIndex(int _id)
{
	for (int i = 0; i < open.size(); i++)
	{
		if (open[i].id == _id)
		{
			return i;
		}
	}

	return -1;
}


/**
   * PopAndClose will take the first node from the open list and move it to the closed list
   *  - Will return null if no open nodes
   */
GOAPNode* GOAP_Reasoner::PopAndClose()
{

	if (open.size() > 0)
	{
		closed.push_back(open[0]);
		open.erase(open.begin());

		return &closed[closed.size() - 1];
	}

	return nullptr;
}

GOAPNode* GOAP_Reasoner::IsMemberOfClosed(WorldState _ws)
{
	for (auto& w : closed)
	{
		if (w.ws == _ws)
		{
			return &w;
		}
	}

	return nullptr;

}

GOAPNode* GOAP_Reasoner::IsMemberOfOpen(WorldState _ws)
{
	for (auto& w : open)
	{
		if (w.ws == _ws)
		{
			return &w;
		}
	}

	return nullptr;
}




/*
GetNextState will apply the provided option to the current game state
*/
WorldState GOAP_Reasoner::GetNextState(AIOptionBase* _option)
{
	
	// set actor state to the current state
	actor->SetBBWorldState(curState);

	// apply option in planining mode so that the BB will be updated with the expected impact of the action
	_option->GetOptionAction()->PerformAction(actor->GetPlanningBlackboard());

	// get actor's new state that will now be the simulated effect of selecting the provided option
	return actor->GetBBWorldState();

}


/*
CalculateHeuristic will return the esitmated cost, determined by a Heuristic, to get from the provided starting state to the provided goal state
*/
int GOAP_Reasoner::CalculateHeuristic(WorldState _now, WorldState _goal)
{
	// Check the World State's estiamted Distance Heuristic
	return _now.DistanceTo(_goal);
}


/*

GOAPPlanner will perform a GOAP algorithm using A* to produce a plan of option

If sucessful, thePlan will be populated with options ordered by steps to take
If unsuccessul, thePlan will be empty and an eror message will be logged to the console

*/

void GOAP_Reasoner::GOAPPlanner()
{

	// set the actor's isPlanning to true to ensure Actions are all simulated
	actor->isPlanning = true;

	// get the current world state - this will be the AI Actor's starting point
	actor->SetPlanningState();
	curState = actor->GetBBWorldState();


	// Check if current state meets the goal - if does, no need to plan
	bool _startMeetsGoal = DoesStateMeetGoal(curState);

	if (_startMeetsGoal)
	{
		std::cout << "GOAP WARNING : The Start State meets the Goal - no plan is required - plan will be an empty list" << std::endl;
		selectedOption = nullptr;
	}


	// clear the plan and open/closed lists

	thePlan.clear();
	open.clear();
	closed.clear();

	int lastID = 0;// node ID tracker


	// create the starting node - give ID 0, and set option to null as no option taken to get to this state
	GOAPNode _startingNode(curState, 0, CalculateHeuristic(curState, goalState), 0, 0, nullptr);

	open.push_back(_startingNode);


	while (open.size() > 0)
	{
		// get the current node
		GOAPNode* _current = PopAndClose();

		// set the current state to that of the newly popped node
		curState = _current->ws;

		// if current node meets the goal, we have found the shortest path
		if (DoesStateMeetGoal(_current->ws))
		{

			std::cout << "GOAP: AI Actor has found a plan, cost is " << _current->g << std::endl;

			// walk back from the goal node to the root, collecting each node's option
			GOAPNode* _node = _current;

			while (_node != nullptr && _node->id != 0)
			{
				thePlan.push_back(_node->option);

				// find the parent node in the closed list (or open list as a fallback)
				GOAPNode* _parent = nullptr;

				for (auto& n : closed)
				{
					if (n.id == _node->parentID)
					{
						_parent = &n;
						break;
					}
				}

				if (_parent == nullptr)
				{
					for (auto& n : open)
					{
						if (n.id == _node->parentID)
						{
							_parent = &n;
							break;
						}
					}
				}

				_node = _parent;
			}

			// the plan was collected goal-to-start, so reverse it into execution order
			std::reverse(thePlan.begin(), thePlan.end());

			for (auto* o : thePlan)
			{
				std::cout << " - Plan Option = " << o->GetOptionID() << std::endl;
			}


	







			std::cout << "GOAP: A plan has been set " << std::endl;

	
			// set the Reasoner's hasPlan flag to inform a plan as been produced
			hasPlan = true;

			// stop the algorithm, goal reached
			return;
		}

		// get all Consideration for the Option


		// Check for paths from the current node
		// This can be achived by checking which of the possible Options are valid from this game state
		for (auto& option : options)
		{

			// set the world state to the current, ready for the testing of the outcome of actions in the game world
			actor->SetBBWorldState(curState);

			// Get all the considerations for the Option, and it's associated Action
			std::vector<std::shared_ptr<AIConsiderationBase>> ruleList = option->GetOptionConsiderations();
			AIAction* action = static_cast<AIAction*>(option->GetOptionAction());


			bool conditionsMet = true;
			// check all conditions
			for (auto r : ruleList)
			{
				if (!r->Calculate(actor->GetPlanningBlackboard()))
					conditionsMet = false;

			}

			// if conditions are met, we can select this option
			if (conditionsMet)
			{
				// get the new world state achieved by applying this option to the current game state
				WorldState outcome = GetNextState(option.get());

				// check if the new game state is already known

				// check the closed list first
				GOAPNode* _c = IsMemberOfClosed(outcome);

				// if already found, skip
				if (_c != nullptr)
				{
					continue;
				}

				// check the open list next
				GOAPNode* _o = IsMemberOfOpen(outcome);

				// if _o is null, we have not seen this game state before
				if (_o == nullptr)
				{
					// Make a new node, with current as its parent, recording the cost and heuristic

					GOAPNode _found(outcome, _current->g + action->cost, CalculateHeuristic(outcome, goalState), ++lastID, _current->id, option.get());
				
					// add the new node to the open list so it will be explored
					AddToOpenList(_found);
				}
				else
				{

					// game state already a memeber of open
				    // check to see if the cost is better					
				
					if (_current->g + action->cost < _o->g)
					{
						// this path is shorter - update the node to take this route instead					
						_o->parentID = _current->id;
						_o->g = _current->g + action->cost;
						_o->h = CalculateHeuristic(outcome, goalState);
						_o->option = option.get();

						// sort to ensure ordered shortest first
						SortOpenList();
					
					}

				}
			}
		}

	}

	std::cout << "GOAP WARNING: No Path has been found - returning empty plan " << std::endl;
}

/*
Think is called on demand to choose from the possible Options that AI has.

The function should implement the logic of the AI method to choose one Option as the selectedOption.
*/
void GOAP_Reasoner::Think()
{
	// If no plan is known, Think() should implement GOAP to produce thePlan (a series of options, ordered)
	// Act() will work through this plan so long as it is valid

	if (!hasPlan)
	{

		std::cout << "GOAP: AI Actor has no plan, creating..." << std::endl;
		// call the planner to populate thePlan
		GOAPPlanner();

		if (hasPlan && thePlan.size()>0)
		{
			// set isPLanning to false to ensure future actions are not simulated, but applied to the game world
			actor->isPlanning = false;

			// start the plan running from the first index
			currentPlanIndex = 0;

			// select the first option the AI Actor should perform
			selectedOption = thePlan[currentPlanIndex];


			std::cout << "GOAP: Reasoner is starting a Plan at index " << currentPlanIndex << std::endl;
		}

	}
	
	
	// By the end of the function,  'thePlan' should be populated with options that comprise the plan
	// ( Act()  will be called automatically)

	if (hasPlan)
	{
		// update the actor's state to Executing to inform that a GOAP plan is being executed
		actor->UpdateTurnStatus(AIActor_Unit::TurnState::Executing);
	}

}



ActionStatus GOAP_Reasoner::Act()
{
	if (selectedOption == nullptr)
	{
		std::cout << "WARNING: Null Selected option, no action taken. " << std::endl;
		return ActionStatus::ACTION_FAILURE;
	}

	if (!hasPlan)
	{
		std::cout << "GOAP WARNING: AI Actor cannot act, no valid plan. " << std::endl;
	}

	// run the current option from the plan
	ActionStatus actionResult = selectedOption->GetOptionAction()->PerformAction(*actorBlackboard);

	
	if (actionResult == ActionStatus::ACTION_SUCCESS)
	{
		// Action has completed, move to the next action
		currentPlanIndex++;
		if (currentPlanIndex== thePlan.size() )
		{
			// plan complete, update status
			hasPlan = false;
			std::cout << "GOAP Plan compelete  - goal reached" << std::endl;
			actor->UpdateTurnStatus(AIActor_Unit::TurnState::GoalReached);
		
		}else
		{ 
			// update the selectedOption to the next Option
			selectedOption = thePlan[currentPlanIndex];
			std::cout << "GOAP: Actor Moving to Plan index " << currentPlanIndex << std::endl;
		}
	}
	else if (actionResult == ActionStatus::ACTION_FAILURE)
	{
		// if Action has failed, then flag for replanning as cannot continue the plan
		std::cout << "GOAP Plan has failed  - attempting to replan... " << std::endl;
		hasPlan = false;
	
	}
	else if (actionResult == ActionStatus::ACTION_RUNNING)
	{
		// action is running, no action needed
	}
	return actionResult;

}