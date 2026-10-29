#include "AIActor_Unit.h"
#include "AIBrain_TBS.h"
#include "GameController.h"
#include <iostream>

AIActor_Unit::AIActor_Unit()
{
	maxHealth = 0;

	gridPosX = 0;
	gridPosY = 0;

	charClass = CharClass::Fighter;


}


AIActor_Unit::AIActor_Unit(CharClass _class)
{
	maxHealth = classMaxHealth[_class];
	
	charClass = _class;

	gridPosX = 0;
	gridPosY = 0;


}

AIActor_Unit::AIActor_Unit(int _maxHealth, int _moveSpeed)
{
	maxHealth = _maxHealth;
	moveSpeed = _moveSpeed;

	charClass = CharClass::Fighter;


	gridPosX = 0;
	gridPosY = 0;


}

AIActor_Unit::~AIActor_Unit()
{

}



// **  AIActorBase overrides **



bool AIActor_Unit::Init(std::string _id, AIConstructorBase& _constructor)
{
	actorID = _id;

	// create new brain
	brain = std::make_unique<AIBrain_TBS>();

	brain->Init(_constructor, *this);

	planningBlackboard.Init(*this);

	isActive = true;
	
	return true;
}

AIBrainBlackboard& AIActor_Unit::GetPlanningBlackboard()
{
	return planningBlackboard;
}


// **  Initialisation **


void AIActor_Unit::InitWorldState()
{
	
	for (int i = 0; i < GameStateKeys.size(); i++)
	{
		planningBlackboard.AddNewValue<bool>(GameStateKeys[i], StartingState[i]);
		AddBBValue<bool>(GameStateKeys[i], StartingState[i]);
	}
}

void AIActor_Unit::SetPlanningState()
{

	for(auto s: GameStateKeys)
		planningBlackboard.AddNewValue<bool>(s, GetBBValue<bool>(s));

}

std::vector<std::string> AIActor_Unit::GetStatusKeys()
{
	return GameStateKeys;
}


/*
GetBBWorldState will return a WorldState relfecting the Actors current Blackboard
*/
WorldState AIActor_Unit::GetBBWorldState()
{
	WorldState _ws;

	if (isPlanning)
	{
		for (auto s : GameStateKeys)
			_ws.state[s] = planningBlackboard.GetValue<bool>(s);

	}
	else
	{
		for (auto s : GameStateKeys)
			_ws.state[s] = GetBBValue<bool>(s);


	}


	return _ws;
}

/*
SetBBWorldState will set the keys on the Planning blackboard to match the provided World State
*/
void AIActor_Unit::SetBBWorldState(WorldState _ws)
{
	for(auto s: _ws.state)
		planningBlackboard.EditValue(s.first, s.second);
}	


void AIActor_Unit::Spawn(int _x, int _y)
{
	gridPosX = _x;
	gridPosY = _y;
}


void AIActor_Unit::InitChar(bool _isActive, CharClass _class, int _team)
{

	isActive = _isActive;
	charClass = _class;
	teamID = _team;


	AddBBValue("Health", 2);

}


void AIActor_Unit::UpdateTurnStatus(TurnState _turnState)
{

	turnState = _turnState;

	gameController->UpdateCharStatus(turnState);
}



/*
* SetGameContext will set the game controller context
*
 - AI decision making data should be on the blackboard, not accessed via the game controller

*/
void AIActor_Unit::SetGameContext(GameController& _gameController) {

	gameController = &_gameController;
}



/*
InitPathfinding will create the AIGridGraph for pathfinding based on the game's map size
*/
void AIActor_Unit::InitPathfinding()
{
	if (gameController == nullptr)
		return;

	mapGridGraph.Init(gameController->mapWidth, gameController->mapHeight);


	//TASK TODO - Add below any non-walkable map tiles using mapGridGraph.SetWalkable()
		// the Shop covers x 6-8, y 1-3
	for (int x = 6; x <= 8; x++)
	{
		for (int y = 1; y <= 3; y++)
		{
			mapGridGraph.SetWalkable(x, y, false);
		}
	}

	// crates
	mapGridGraph.SetWalkable(1, 4, false);
	mapGridGraph.SetWalkable(2, 4, false);
	mapGridGraph.SetWalkable(14, 2, false);
	mapGridGraph.SetWalkable(16, 2, false);

	
}



/*
TestPathfinding can be used to manaually force a path for testing purposes
*/
void AIActor_Unit::TestPathfinding()
{
	//e.g. Pathfind(0, 0, 5, 5);
}



//  ** Game Loop **


/*
* Update will cause the AI Actor to use the brain to make a decision
*/
void AIActor_Unit::Update(float _dt)
{
	brain->Update();
}







// ** Game Rules and Logic **


/*
* unsed, but can be used for initialisation
*/
void AIActor_Unit::PreTransition()
{

}

bool AIActor_Unit::TakePathStep()
{
	Move(pathSteps[pathStep]);

	pathStep++;

	if (pathStep >= pathSteps.size())
		return true;
	else
		return false;
	
}

void AIActor_Unit::Move(int _gridIndex)
{
	gridPosX = mapGridGraph.GetNodeX(_gridIndex);
	gridPosY = mapGridGraph.GetNodeY(_gridIndex);

	//std::cout << "Moved to " << gridPosX << " / " << gridPosY << std::endl;

}


void AIActor_Unit::Move(int _x, int _y)
{
	gridPosX = _x;
	gridPosY = _y;
}


/*
* UpdateHealth will update the health value on the blackboard by the specified amount
*/
void AIActor_Unit::UpdateHealth(int _change)
{

	int _curHealth = GetBBValue<int>("Health");

	int _newHealth = _curHealth +_change;

	if (_newHealth >= maxHealth)
		_newHealth = maxHealth;

	if (_newHealth < 0)
		_newHealth = 0;

	EditBBValue("Health", _newHealth);
}


void AIActor_Unit::ClearAtLocations()
{

	if (isPlanning)
	{
		planningBlackboard.EditValue("AtBush", false);
		planningBlackboard.EditValue("AtForest", false);
		planningBlackboard.EditValue("AtTrader", false);
		planningBlackboard.EditValue("AtStore", false);
		planningBlackboard.EditValue("AtWell", false);
		planningBlackboard.EditValue("AtForge", false);
		planningBlackboard.EditValue("AtCommander", false);
	}
	else
	{
		EditBBValue("AtBush", false);
		EditBBValue("AtForest", false);
		EditBBValue("AtTrader", false);
		EditBBValue("AtStore", false);
		EditBBValue("AtWell", false);
		EditBBValue("AtForge", false);
		EditBBValue("AtCommander", false);
	}

}


// ** AI Actions **
// These functions will cause a change in game state
// During Planning, the planningBlackboard should be instantly updated with the outcome of the Action
// During Gameplay, the usual actor blackboard should be updated once Action is complete
// Any Action that returns  ActionStatus::ACTION_FAILURE will cause a GOAP plan to fail



ActionStatus AIActor_Unit::FindTrader()
{
	if (isPlanning)
	{
		ClearAtLocations();
		planningBlackboard.EditValue("AtTrader", true);

		return ActionStatus::ACTION_SUCCESS;
	}




	if (moveState == MoveState::Idle)
	{
		std::cout << "Actor is Finding Trader" << std::endl;
		currentAction = "Finding Trader";
		Pathfind(gridPosX, gridPosY, 15, 3);
		moveState = MoveState::Pathing;
		ClearAtLocations();
	}
	else if (moveState == MoveState::Pathing)
	{
		bool completedPath = TakePathStep();

		if (completedPath)
			moveState = MoveState::Complete;

	}
	else if (moveState == MoveState::Complete)
	{
		EditBBValue("AtTrader", true);
		return ActionStatus::ACTION_SUCCESS;
	}

	return ActionStatus::ACTION_RUNNING;
}




ActionStatus AIActor_Unit::FindWater()
{
	if (isPlanning)
	{
		ClearAtLocations();
		planningBlackboard.EditValue("AtWell", true);

		return ActionStatus::ACTION_SUCCESS;
	}


	if (moveState == MoveState::Idle)
	{
		std::cout << "Actor is Searching for Water" << std::endl;
		currentAction = "Finding Water";
		Pathfind(gridPosX, gridPosY, 1, 7);
		moveState = MoveState::Pathing;
		ClearAtLocations();

	}
	else if (moveState == MoveState::Pathing)
	{
		bool completedPath = TakePathStep();

		if (completedPath)
			moveState = MoveState::Complete;

	}
	else if (moveState == MoveState::Complete)
	{
		EditBBValue("AtWell", true);
		return ActionStatus::ACTION_SUCCESS;
	}

	return ActionStatus::ACTION_RUNNING;
}



ActionStatus AIActor_Unit::FindBerries()
{
	if (isPlanning)
	{
		ClearAtLocations();
		planningBlackboard.EditValue("AtBush", true);

		return ActionStatus::ACTION_SUCCESS;
	}



	if (moveState == MoveState::Idle)
	{
		std::cout << "Actor is Finding Berries" << std::endl;
		currentAction = "Finding Berries";
		Pathfind(gridPosX, gridPosY, 3, 3);
		moveState = MoveState::Pathing;
		ClearAtLocations();
	}
	else if (moveState == MoveState::Pathing)
	{
		bool completedPath = TakePathStep();

		if (completedPath)
			moveState = MoveState::Complete;

	}
	else if (moveState == MoveState::Complete)
	{
		moveState = MoveState::Idle;
		EditBBValue("AtBush", true);
		return ActionStatus::ACTION_SUCCESS;
	}

	return ActionStatus::ACTION_RUNNING;
}


ActionStatus AIActor_Unit::FindForest()
{
	if (isPlanning)
	{
		ClearAtLocations();
		planningBlackboard.EditValue("AtForest", true);

		return ActionStatus::ACTION_SUCCESS;
	}


	if (moveState == MoveState::Idle)
	{
		std::cout << "Actor is Finding Forest" << std::endl;
		currentAction = "Finding Forest";
		Pathfind(gridPosX, gridPosY, 13, 8);
		moveState = MoveState::Pathing;
		ClearAtLocations();
	}
	else if (moveState == MoveState::Pathing)
	{
		bool completedPath = TakePathStep();

		if (completedPath)
			moveState = MoveState::Complete;

	}
	else if (moveState == MoveState::Complete)
	{
		moveState = MoveState::Idle;
		EditBBValue("AtForest", true);
		return ActionStatus::ACTION_SUCCESS;
	}

	return ActionStatus::ACTION_RUNNING;

	
}



ActionStatus AIActor_Unit::FindStore()
{
	if (isPlanning)
	{
		ClearAtLocations();
		planningBlackboard.EditValue("AtStore", true);

		return ActionStatus::ACTION_SUCCESS;
	}


	if (moveState == MoveState::Idle)
	{
		std::cout << "Actor is Finding Store" << std::endl;
		currentAction = "Finding Store";
		Pathfind(gridPosX, gridPosY, 7, 4);
		moveState = MoveState::Pathing;
		ClearAtLocations();
	}
	else if (moveState == MoveState::Pathing)
	{
		bool completedPath = TakePathStep();

		if (completedPath)
			moveState = MoveState::Complete;

	}
	else if (moveState == MoveState::Complete)
	{
		moveState = MoveState::Idle;
		EditBBValue("AtStore", true);
		return ActionStatus::ACTION_SUCCESS;
	}

	return ActionStatus::ACTION_RUNNING;


}



ActionStatus AIActor_Unit::FindCommander()
{
	if (isPlanning)
	{
		ClearAtLocations();
		planningBlackboard.EditValue("AtCommander", true);

		return ActionStatus::ACTION_SUCCESS;
	}


	if (moveState == MoveState::Idle)
	{
		std::cout << "Actor is Finding Commander" << std::endl;
		currentAction = "Finding Commander";
		Pathfind(gridPosX, gridPosY, 18, 8);
		moveState = MoveState::Pathing;
		ClearAtLocations();
	}
	else if (moveState == MoveState::Pathing)
	{
		bool completedPath = TakePathStep();

		if (completedPath)
			moveState = MoveState::Complete;

	}
	else if (moveState == MoveState::Complete)
	{
		moveState = MoveState::Idle;
		EditBBValue("AtCommander", true);
		return ActionStatus::ACTION_SUCCESS;
	}

	return ActionStatus::ACTION_RUNNING;


}




ActionStatus AIActor_Unit::FindForge()
{
	if (isPlanning)
	{
		ClearAtLocations();
		planningBlackboard.EditValue("AtForge", true);

		return ActionStatus::ACTION_SUCCESS;
	}


	if (moveState == MoveState::Idle)
	{
		std::cout << "Actor is Finding Forge" << std::endl;
		currentAction = "Finding Forge";
		Pathfind(gridPosX, gridPosY, 6, 7);
		moveState = MoveState::Pathing;
		ClearAtLocations();
	}
	else if (moveState == MoveState::Pathing)
	{
		bool completedPath = TakePathStep();

		if (completedPath)
			moveState = MoveState::Complete;

	}
	else if (moveState == MoveState::Complete)
	{
		moveState = MoveState::Idle;
		EditBBValue("AtForge", true);
		return ActionStatus::ACTION_SUCCESS;
	}

	return ActionStatus::ACTION_RUNNING;


}





ActionStatus AIActor_Unit::UseWell()
{
	if (isPlanning)
	{
		planningBlackboard.EditValue("HasWater", true);

		return ActionStatus::ACTION_SUCCESS;
	}

	std::cout << "Actor is Using Well" << std::endl;
	currentAction = "Using Well";
	EditBBValue("HasWater", true);
	moveState = MoveState::Idle;

	return ActionStatus::ACTION_SUCCESS;
}




ActionStatus AIActor_Unit::CraftPotion()
{
	if (isPlanning)
	{
		planningBlackboard.EditValue("HasBerries", false);
		planningBlackboard.EditValue("HasWater", false);
		planningBlackboard.EditValue("HasPotion", true);

		return ActionStatus::ACTION_SUCCESS;
	}

	std::cout << "Actor is Crafting Potion" << std::endl;
	currentAction = "Crafting Potion";

	EditBBValue("HasBerries", false);
	EditBBValue("HasWater", false);
	EditBBValue("HasPotion", true);
	moveState = MoveState::Idle;

	return ActionStatus::ACTION_SUCCESS;
}



ActionStatus AIActor_Unit::DrinkPotion()
{
	if (isPlanning)
	{
		planningBlackboard.EditValue("HasHighHealth", true);
		planningBlackboard.EditValue("HasPotion", false);

		return ActionStatus::ACTION_SUCCESS;
	}


	std::cout << "Actor is Drinking Potion" << std::endl;
	currentAction = "Drinking Potion";
	UpdateHealth(20);
	EditBBValue("HasHighHealth", true);
	EditBBValue("HasPotion", false);
	moveState = MoveState::Idle;

	return ActionStatus::ACTION_SUCCESS;
}

ActionStatus AIActor_Unit::SellMushrooms()
{
	if (isPlanning)
	{
		planningBlackboard.EditValue("HasMushrooms", false);
		planningBlackboard.EditValue("HasMoney", true);

		return ActionStatus::ACTION_SUCCESS;
	}


	std::cout << "Actor is Selling Mushrooms" << std::endl;
	currentAction = "Selling Mushroom";
	EditBBValue("HasMushrooms", false);
	EditBBValue("HasMoney", true);
	moveState = MoveState::Idle;

	return ActionStatus::ACTION_SUCCESS;
}

ActionStatus AIActor_Unit::BuyPotion()
{
	if (isPlanning)
	{
		planningBlackboard.EditValue("HasPotion", true);
		planningBlackboard.EditValue("HasMoney", false);

		return ActionStatus::ACTION_SUCCESS;
	}

	std::cout << "Actor is Buying Potion" << std::endl;
	currentAction = "Buying Potion";
	EditBBValue("HasPotion", true);
	EditBBValue("HasMoney", false);

	moveState = MoveState::Idle;



	return ActionStatus::ACTION_SUCCESS;
}


ActionStatus AIActor_Unit::PickBerries()
{
	if (isPlanning)
	{
		planningBlackboard.EditValue("HasBerries", true);

		return ActionStatus::ACTION_SUCCESS;
	}

	std::cout << "Actor is Picking Berries" << std::endl;
	currentAction = "Picking Berries";
	EditBBValue("HasBerries", true);

	moveState = MoveState::Idle;



	return ActionStatus::ACTION_SUCCESS;
}


ActionStatus AIActor_Unit::PickMushrooms()
{
	if (isPlanning)
	{
		planningBlackboard.EditValue("HasMushrooms", true);

		return ActionStatus::ACTION_SUCCESS;
	}

	std::cout << "Actor is Picking Mushrooms" << std::endl;
	currentAction = "Picking Mushrooms";
	EditBBValue("HasMushrooms", true);

	moveState = MoveState::Idle;



	return ActionStatus::ACTION_SUCCESS;
}

ActionStatus AIActor_Unit::UseForge()
{
	if (isPlanning)
	{
		planningBlackboard.EditValue("HasSword", true);

		return ActionStatus::ACTION_SUCCESS;
	}

	std::cout << "Actor is Repairing Sword" << std::endl;
	currentAction = "Repairing Sword";
	EditBBValue("HasSword", true);

	moveState = MoveState::Idle;



	return ActionStatus::ACTION_SUCCESS;
}


ActionStatus AIActor_Unit::StealPotion()
{
	if (isPlanning)
	{
		planningBlackboard.EditValue("HasPotion", true);
		planningBlackboard.EditValue("HasStolen", true);
	

		return ActionStatus::ACTION_SUCCESS;
	}

	std::cout << "Actor is Stealing Potion" << std::endl;
	currentAction = "Stealing Potion";
	EditBBValue("HasStolen", true);

	if (AIRandom_Global::getInstance()->GetRandom01() < stealPotionChance)
	{
		std::cout << "Actor SUCEEDED to steal Potion" << std::endl;

		EditBBValue("HasPotion", true);

		moveState = MoveState::Idle;


		return ActionStatus::ACTION_SUCCESS;

	}
	else
	{
		std::cout << "Actor FAILED to steal Potion" << std::endl;

		moveState = MoveState::Idle;

		return ActionStatus::ACTION_FAILURE;
	}

	
}


/*
Pathfind will cause the AIActor to calcualte a path to the provided location
pathSteps will be updated with the graph indexes for the path
 - check console for any errors or warnings, such as invalid paths
*/

void AIActor_Unit::Pathfind(int _startX, int _startY, int _goalX, int _goalY )
{
	int _gridStart = mapGridGraph.GridToNode(_startX, _startY);
	int _gridTarget = mapGridGraph.GridToNode(_goalX, _goalY);

	pathHeuristic.Init(mapGridGraph, _gridTarget);

	pathfinder.FindPath(mapGridGraph, _gridStart, _gridTarget, pathHeuristic, pathConnections);

	pathSteps = pathfinder.ConnectionsToNodes(pathConnections, _gridStart);

	for (int i = 0; i < pathSteps.size(); i++)
	{
		std::cout << "Path Step is " << pathSteps[i] << " ( " << mapGridGraph.GetNodeX(pathSteps[i]) << " / " << mapGridGraph.GetNodeY(pathSteps[i]) << " )" << std::endl;
	}

	pathStep = 0;

}