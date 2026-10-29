#pragma once
#include "../../AI_Framework/framework.h"

class AIConstructor_GOAP :
    public AIConstructorBase
{
public:
	bool Init() override;
	void DefineActions() override;
	void DefineConsiderations() override;
	void DefineOptions() override;

	void AddActionByName(std::string _actionName, std::function<ActionStatus(AIBrainBlackboardBase&)> _action, float _cost);

private:


};

