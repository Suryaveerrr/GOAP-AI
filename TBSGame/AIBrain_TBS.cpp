#include "AIBrain_TBS.h"
#include "GOAP_Reasoner.h"

void AIBrain_TBS::Update()
{
	// call the reasoner to get the selected action
	brainReasoner->Update();

}

bool AIBrain_TBS::Init(AIConstructorBase& _constructor, AIActorBase& _actorContext)
{
	brainBlackboard = std::make_unique<AIBrainBlackboard>();
	brainBlackboard->Init(_actorContext);


	
	brainReasoner = std::make_unique < GOAP_Reasoner >();

	brainReasoner->SetOptions(_constructor);

	brainReasoner->Init("GOAP Reasoner", *brainBlackboard);


	GOAP_Reasoner* _goapReasoner = static_cast<GOAP_Reasoner*>(brainReasoner.get());

	// for GOAP, we must create a Planning Database, provide the actor context to this
	_goapReasoner->InitPlanningDatabase(_actorContext);

	return true;

}