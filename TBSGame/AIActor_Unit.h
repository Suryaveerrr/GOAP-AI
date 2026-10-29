#pragma once

#ifndef AI_ACTOR_UNIT_H
#define AI_ACTOR_UNIT_H

#include "../../AI_Framework/framework.h"
#include "GOAPNode.h"
#include "AIAStarPathfinder.h"
#include "AIGridGraph.h"
#include "AIPathHeuristic.h"

class GameController;


class AIActor_Unit :
    public AIActorBase
{
public:
	enum CharClass { Fighter=0, Archer, Wizard };

	// Turn State provides the state for the GOAP process
	enum TurnState { Waiting, Planning, Executing, GoalReached };

	// MoveState provides the state for map navigation
	enum MoveState { Idle, Pathing, Complete};

	//GameStateKeys lists the blackboard keys related to the GOAP world state
	const std::vector<std::string> GameStateKeys = { "HasHighHealth", "AtStore", "AtWell","AtTrader", "AtBush", "AtForest","AtForge", "AtCommander", "HasPotion", "HasWater" , "HasBerries", "HasMushrooms",  "HasStolen",   "HasSword", "HasMoney"};

	//StartingState gives the starting state of the GameStateKeys at initialisation
	const std::vector<bool> StartingState = { false, false, false, false, false, false, false, false, false, false, false, false,  false, false, false };


	const float stealPotionChance = 0.1f;

	int classMaxHealth[3] = {20, 15, 10};

	AIActor_Unit();
	AIActor_Unit(CharClass _class);
	AIActor_Unit(int _maxHealth, int _moveSpeed);

	~AIActor_Unit();

	// AIActorBase overrides
	bool Init(std::string _id, AIConstructorBase& _constructor) override;
	void SetGameContext(GameController& _gameController) ;
	void Update(float _dt) override;
	void PreTransition() override;//State Transition Functions

	// Initialisation
	void Spawn(int _x, int _y);
	void InitChar(bool _isActive, CharClass _class, int _team);

	// Action Functions
	ActionStatus FindWater();
	ActionStatus FindForest();
	ActionStatus FindTrader();
	ActionStatus FindBerries();
	ActionStatus FindStore();
	ActionStatus FindForge();
	ActionStatus FindCommander();

	ActionStatus UseWell();	
	ActionStatus PickMushrooms();
	ActionStatus SellMushrooms();
	ActionStatus BuyPotion();
	ActionStatus PickBerries();
	ActionStatus StealPotion(); 
	ActionStatus UseForge();

	ActionStatus CraftPotion();
	ActionStatus DrinkPotion();

	// Actor State 

	bool isPlanning = true;
	TurnState turnState = TurnState::Waiting;
	MoveState moveState = MoveState::Idle;
	bool isActive = false;
	int maxHealth = 20;
	int moveSpeed = 1;
	int teamID = 0;
	CharClass charClass;

	void ClearAtLocations();

	void InitWorldState();
	void SetPlanningState();
	WorldState GetBBWorldState();
	void SetBBWorldState(WorldState _ws);

	// map posiiton
	int gridPosX = 0;
	int gridPosY = 0;

	std::string currentAction = "";

	// map mpvement
	void Move(int _x, int _y);
	void Move(int _gridIndex);
	bool TakePathStep();
	int pathStep = 0;
	bool isActionComplete = false;
	
	void Pathfind(int _startX, int _startY, int _goalX, int _goalY);
	void InitPathfinding();
	void TestPathfinding();

	// Actor State update functions
	void UpdateHealth(int _change);
	void UpdateTurnStatus(TurnState _turnState);



	std::vector<std::string> GetStatusKeys();

	// Pathfinding data - stored per Actor here, but could be more efficiently stored elsewhere
	AIAStarPathfinder pathfinder;
	AIGridGraph mapGridGraph;
	std::vector<AIPathConnection> pathConnections;
	AIEuclideanHeuristic pathHeuristic;
	std::vector<int> pathSteps;

	// link to game controller
	GameController* gameController;

	// blackboard used only for GOAP Planning
	AIBrainBlackboard planningBlackboard;

	AIBrainBlackboard& GetPlanningBlackboard();


	
};


#endif //!AI_ACTOR_UNIT_H