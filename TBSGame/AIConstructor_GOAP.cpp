#include "AIConstructor_GOAP.h"
#include "AIActor_Unit.h"

bool AIConstructor_GOAP::Init() {

	method = AIConstructorBase::Method::GOAP;
	return true;
}


/*
DefineActions creates all the possible actions that the AI can perform
*/
void AIConstructor_GOAP::DefineActions()
{


	auto aFindCommander = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here


		return 	actor->FindCommander();

		};

	// Link the function to an identifier
	AddActionByName("ActionFindCommander", aFindCommander, 4);


	auto aFindForge = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here


		return 	actor->FindForge();

		};

	// Link the function to an identifier
	AddActionByName("ActionFindForge", aFindForge,8);

	auto aUseForge = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here


		return 	actor->UseForge();

		};

	// Link the function to an identifier
	AddActionByName("ActionUseForge", aUseForge, 8);


	auto aFindWater = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here
	

		return 	actor->FindWater();

		};


	// Link the function to an identifier
	AddActionByName("ActionFindWater", aFindWater, 4);


	auto aFindStore = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here


		return 	actor->FindStore();

		};


	// Link the function to an identifier
	AddActionByName("ActionFindStore", aFindStore, 4);

	auto aFindBerries = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here



		return 	actor->FindBerries();
		};



	// Link the function to an identifier
	AddActionByName("ActionFindBerries", aFindBerries, 4);





	auto aFindForest = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here


		return 	actor->FindForest();

		};


	// Link the function to an identifier
	AddActionByName("ActionFindForest", aFindForest, 4);



	auto aFindFTrader = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here


		return 	actor->FindTrader();

		};


	// Link the function to an identifier
	AddActionByName("ActionFindTrader", aFindFTrader, 4);








	auto aUseWell = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here
	


		return 	actor->UseWell();
		};


	// Link the function to an identifier
	AddActionByName("ActionUseWell", aUseWell, 4);






	auto aCraftPotion= [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

		// Call behaviour functions here

		return actor->CraftPotion();
		};


		// Link the function to an identifier
		AddActionByName("ActionCraftPotion", aCraftPotion, 4);


		auto aDrinkPotion = [](AIBrainBlackboardBase& bb) -> ActionStatus {
			AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

			// Call behaviour functions here
			


			return actor->DrinkPotion();
			};


		// Link the function to an identifier
		AddActionByName("ActionDrinkPotion", aDrinkPotion, 4);

		



		auto aSellMushrooms = [](AIBrainBlackboardBase& bb) -> ActionStatus {
			AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

			// Call behaviour functions here


			return 	actor->SellMushrooms();

			};


		// Link the function to an identifier
		AddActionByName("ActionSellMushrooms", aSellMushrooms, 4);


		auto aBuyPotion = [](AIBrainBlackboardBase& bb) -> ActionStatus {
			AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

			// Call behaviour functions here


			return 	actor->BuyPotion();

			};


		// Link the function to an identifier
		AddActionByName("ActionBuyPotion", aBuyPotion, 4);



		auto aCollectBerries = [](AIBrainBlackboardBase& bb) -> ActionStatus {
			AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

			// Call behaviour functions here


			return 	actor->PickBerries();

			};


		// Link the function to an identifier
		AddActionByName("ActionCollectBerries", aCollectBerries, 4);



		auto aCollectMushrooms = [](AIBrainBlackboardBase& bb) -> ActionStatus {
			AIActor_Unit* actor = static_cast<AIActor_Unit*>(bb.GetActorContext());

			// Call behaviour functions here


			return 	actor->PickMushrooms();;

			};


		// Link the function to an identifier
		AddActionByName("ActionCollectMushrooms", aCollectMushrooms, 4);

		

}


/*
	DefineConsiderations creates all Considerations that will be linked to Options
// Options will only be selected if the Condition is true
*/
void AIConstructor_GOAP::DefineConsiderations()
{

	auto cHasStolen = [](AIBrainBlackboardBase& bb) -> bool {

		bool stolen = (bb.GetValue<bool>("HasStolen") == 1);

		return(stolen);

		};


	AddConsiderationByName("ConsiderationHasStolen", cHasStolen);


	auto cHasHighHealth = [](AIBrainBlackboardBase& bb) -> bool {

		bool highHealth = (bb.GetValue<bool>("HasHighHealth") ==1);

		return(highHealth);

	};


	AddConsiderationByName("ConsiderationHasHighHealth", cHasHighHealth);



	auto cAtStore = [](AIBrainBlackboardBase& bb) -> bool {

		bool atStore = (bb.GetValue<bool>("AtStore") == 1);

		return(atStore);

		};

	AddConsiderationByName("ConsiderationAtStore", cAtStore);


	auto cAtForest = [](AIBrainBlackboardBase& bb) -> bool {

		bool atForest = (bb.GetValue<bool>("AtForest") == 1);

		return(atForest);

		};


	AddConsiderationByName("ConsiderationAtForest", cAtForest);


	auto cAtTrader = [](AIBrainBlackboardBase& bb) -> bool {

		bool atTrader = (bb.GetValue<bool>("AtTrader") == 1);

		return(atTrader);

		};


	AddConsiderationByName("ConsiderationAtTrader", cAtTrader);


	auto cHasMoney = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasMoney = (bb.GetValue<bool>("HasMoney") == 1);

		return(hasMoney);

		};


	AddConsiderationByName("ConsiderationHasMoney", cHasMoney);



	auto cAtBush = [](AIBrainBlackboardBase& bb) -> bool {

		bool aBush = (bb.GetValue<bool>("AtBush") == 1);

		return(aBush);

		};


	AddConsiderationByName("ConsiderationAtBush", cAtBush);


	auto cAtWell = [](AIBrainBlackboardBase& bb) -> bool {

		bool atWell = (bb.GetValue<bool>("AtWell") == 1);

		return(atWell);

		};


	AddConsiderationByName("ConsiderationAtWell", cAtWell);



	auto cAtCommander = [](AIBrainBlackboardBase& bb) -> bool {

		bool aBush = (bb.GetValue<bool>("AtCommander") == 1);

		return(aBush);

		};


	AddConsiderationByName("ConsiderationAtCommander", cAtCommander);


	auto cAtForge = [](AIBrainBlackboardBase& bb) -> bool {

		bool atForge = (bb.GetValue<bool>("AtForge") == 1);

		return(atForge);

		};


	AddConsiderationByName("ConsiderationAtForge", cAtForge);


	auto cHasPotion = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasPotion = (bb.GetValue<bool>("HasPotion") == 1);

		return(hasPotion);

		};


	AddConsiderationByName("ConsiderationHasPotion", cHasPotion);


	auto cHasSword = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasSword = (bb.GetValue<bool>("HasSword") == 1);

		return(hasSword);

		};


	AddConsiderationByName("ConsiderationHasSword", cHasSword);



	auto cHasWater = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasWater = (bb.GetValue<bool>("HasWater") == 1);

		return(hasWater);

		};


	AddConsiderationByName("ConsiderationHasWater", cHasWater);


	auto cHasBerries = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasBerries = (bb.GetValue<bool>("HasBerries") == 1);

		return(hasBerries);

		};


	AddConsiderationByName("ConsiderationHasBerries", cHasBerries);



	auto cHasMushrooms = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasMushrooms = (bb.GetValue<bool>("HasMushrooms") == 1);

		return(hasMushrooms);

		};


	AddConsiderationByName("ConsiderationHasMushrooms", cHasMushrooms);


	// inverse considerations

	auto cNotHasHighHealth = [](AIBrainBlackboardBase& bb) -> bool {

		bool highHealth = (bb.GetValue<bool>("HasHighHealth") == 0);

		return(highHealth);

		};


	AddConsiderationByName("ConsiderationNOTHighHealth", cNotHasHighHealth);



	auto cNotAtStore = [](AIBrainBlackboardBase& bb) -> bool {

		bool atStore = (bb.GetValue<bool>("AtStore") == 0);

		return(atStore);

		};


	AddConsiderationByName("ConsiderationNOTAtStore", cNotAtStore);



	auto cNotHasMoney = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasMoney = (bb.GetValue<bool>("HasMoney") == 0);

		return(hasMoney);

		};


	AddConsiderationByName("ConsiderationNOTHasMoney", cNotHasMoney);


	auto cNotAtWell = [](AIBrainBlackboardBase& bb) -> bool {

		bool atWell = (bb.GetValue<bool>("AtWell") == 0);

		return(atWell);

		};


	AddConsiderationByName("ConsiderationNOTAtWell", cNotAtWell);



	auto cNotHasPotion = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasPotion = (bb.GetValue<bool>("HasPotion") == 0);

		return(hasPotion);

		};


	AddConsiderationByName("ConsiderationNOTHasPotion", cNotHasPotion);


	auto cNotHasWater = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasWater = (bb.GetValue<bool>("HasWater") == 0);

		return(hasWater);

		};


	AddConsiderationByName("ConsiderationNOTHasWater", cNotHasWater);

	auto cNotHasSword = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasSword = (bb.GetValue<bool>("HasSword") == 0);

		return(hasSword);

		};


	AddConsiderationByName("ConsiderationNOTHasSword", cNotHasSword);


	auto cNotHasBerries = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasBerries = (bb.GetValue<bool>("HasBerries") == 0);

		return(hasBerries);

		};


	AddConsiderationByName("ConsiderationNOTHasBerries", cNotHasBerries);



	auto cNotHasMushrooms = [](AIBrainBlackboardBase& bb) -> bool {

		bool hasMushrooms = (bb.GetValue<bool>("HasMushrooms") == 0);

		return(hasMushrooms);

		};


	AddConsiderationByName("ConsiderationNOTHasMushrooms", cNotHasMushrooms);


	auto cNotAtForest = [](AIBrainBlackboardBase& bb) -> bool {

		bool noteAtForest = (bb.GetValue<bool>("AtForest") == 0);

		return(noteAtForest);

		};

	AddConsiderationByName("ConsiderationNOTAtForest", cNotAtForest);




	auto cNotAtTrader = [](AIBrainBlackboardBase& bb) -> bool {

		bool notAtTrader = (bb.GetValue<bool>("AtTrader") == 0);

		return(notAtTrader);

		};

	AddConsiderationByName("ConsiderationNOTAtTrader", cNotAtTrader);


	auto cNotAtBush = [](AIBrainBlackboardBase& bb) -> bool {

		bool notAtBush = (bb.GetValue<bool>("AtBush") == 0);

		return(notAtBush);

		};

	AddConsiderationByName("ConsiderationNOTAtBush", cNotAtBush);



	auto cHasNotStolen = [](AIBrainBlackboardBase& bb) -> bool {

		bool stolen = (bb.GetValue<bool>("HasStolen") == 0);

		return(stolen);

		};


	AddConsiderationByName("ConsiderationNOTHasStolen", cHasNotStolen);

	auto cNotAtForge = [](AIBrainBlackboardBase& bb) -> bool {

		bool notAtForge = (bb.GetValue<bool>("AtForge") == 0);

		return(notAtForge);

		};

	AddConsiderationByName("ConsiderationNOTAtForge", cNotAtForge);



	auto cNotAtCommander= [](AIBrainBlackboardBase& bb) -> bool {

		bool notAtCommander = (bb.GetValue<bool>("AtCommander") == 0);

		return(notAtCommander);

		};


	AddConsiderationByName("ConsiderationNOTAtCommander", cNotAtCommander);



}



/*
* DefineOptions links the Options with Considerations and Actions
*/
void AIConstructor_GOAP::DefineOptions()
{
	// TASK TODO - add any additional Options you need to create, linking them with an Action
	AddOptionByName("OptionFindWater", "ActionFindWater");
	AddOptionByName("OptionFindBerries", "ActionFindBerries");
	AddOptionByName("OptionFindForest", "ActionFindForest");
	AddOptionByName("OptionFindTrader", "ActionFindTrader");
	AddOptionByName("OptionFindStore", "ActionFindStore");
	AddOptionByName("OptionFindForge", "ActionFindForge");
	AddOptionByName("OptionFindCommander", "ActionFindCommander");


	AddOptionByName("OptionUseWell", "ActionUseWell");
	AddOptionByName("OptionSellMushrooms", "ActionSellMushrooms");
	AddOptionByName("OptionBuyPotion", "ActionBuyPotion");
	AddOptionByName("OptionCollectBerries", "ActionCollectBerries");
	AddOptionByName("OptionCollectMushrooms", "ActionCollectMushrooms");
	AddOptionByName("OptionUseForge", "ActionUseForge");

	AddOptionByName("OptionCraftPotion", "ActionCraftPotion");
	AddOptionByName("OptionDrinkPotion", "ActionDrinkPotion");



	// TASK TODO - add Considerations to Options by calling 'AddOptionConsideration()'
	// Options will only be selected if the Condition is true

	AddOptionConsideration("OptionFindWater", "ConsiderationNOTAtWell");

	AddOptionConsideration("OptionFindBerries", "ConsiderationNOTAtBush");

	AddOptionConsideration("OptionFindForest", "ConsiderationNOTAtForest");

	AddOptionConsideration("OptionFindTrader", "ConsiderationNOTAtTrader");

	AddOptionConsideration("OptionFindStore", "ConsiderationNOTAtStore");

	AddOptionConsideration("OptionFindForge", "ConsiderationNOTAtForge");


	AddOptionConsideration("OptionCollectBerries", "ConsiderationAtBush");
	AddOptionConsideration("OptionCollectBerries", "ConsiderationNOTHasBerries");

	AddOptionConsideration("OptionCollectMushrooms", "ConsiderationAtForest");
	AddOptionConsideration("OptionCollectMushrooms", "ConsiderationNOTHasMushrooms");

	AddOptionConsideration("OptionUseWell", "ConsiderationAtWell");
	AddOptionConsideration("OptionUseWell", "ConsiderationNOTHasWater");

	AddOptionConsideration("OptionUseForge", "ConsiderationAtForge");
	AddOptionConsideration("OptionUseForge", "ConsiderationNOTHasSword");

	AddOptionConsideration("OptionSellMushrooms", "ConsiderationHasMushrooms");
	AddOptionConsideration("OptionSellMushrooms", "ConsiderationAtTrader");

	AddOptionConsideration("OptionBuyPotion", "ConsiderationAtStore");
	AddOptionConsideration("OptionBuyPotion", "ConsiderationHasMoney");


	AddOptionConsideration("OptionCraftPotion", "ConsiderationNOTHasPotion");
	AddOptionConsideration("OptionCraftPotion", "ConsiderationHasBerries");
	AddOptionConsideration("OptionCraftPotion", "ConsiderationHasWater");

	AddOptionConsideration("OptionDrinkPotion", "ConsiderationHasPotion");

	AddOptionConsideration("OptionFindCommander", "ConsiderationNOTAtCommander");
	AddOptionConsideration("OptionFindCommander", "ConsiderationHasHighHealth");
	AddOptionConsideration("OptionFindCommander", "ConsiderationHasSword");
		

	

}








/*
AddActionByName is overridden to allow initialisation of a cost to the action
Cost will be used by GOAP A*
*/

void AIConstructor_GOAP::AddActionByName(std::string _actionName, std::function<ActionStatus(AIBrainBlackboardBase&)> _function, float _cost)
{

	//Define an action, and init with the function which will be called when the action is selected
	auto _action = std::make_shared<AIAction>();
	_action->Init(_actionName, _function);
	_action->SetCost(_cost);
	actions.insert({ _actionName, _action });
}