#pragma once


#ifndef AI_GAME_CONTROLLER_H
#define AI_GAME_CONTROLLER_H


#include <SFML/Graphics.hpp>
#include "AIActor_Unit.h"
#include "AIConstructor_GOAP.h"

struct MapBuilding
{
public:
	int x;
	int y;
	sf::Texture* texture;

	MapBuilding(int _x, int _y, sf::Texture& tex) {
		x = _x;
		y = _y;
		texture = &tex;
	}
	
};


class GameController
{
public:

	static const int mapWidth = 20;
	static const int mapHeight = 10;

	static const int Team0ID = 0;
	static const int Team1ID = 1;


	enum TerrainType { Ground1 , Ground2, Ground3};
	enum BuildingType { None, House, Bush, ShopKeeper, Crate, Mushrooms, Well , Forest, Trader, Forge, Commander};



	GameController();
	~GameController();



	//Intialisation
	void Init();


	//Events and Game Loop

	void Update(float dt);
	void HandleKeyPress(const sf::Event::KeyPressed* _keyPress);
	void HandleMousePress(const sf::Event::MouseButtonPressed* _mousePress);
	


	// Rendering
	void DisplayUI(sf::RenderWindow& window);
	void Render(sf::RenderWindow& window);
	

	// Game Status
	void StartGame();
	void EndGame();
	bool gameRunning = false;
	int activeChar = 0;

	void UpdateCharStatus(AIActor_Unit::TurnState _turnState);
private:


	const float mapSectionXY = 64.0f;

	// Create the AI constructors
	AIConstructor_GOAP goapAIConstructor;


	//Intialisation
	void InitGame();
	void InitAI();
	void LoadGraphicsAssests();
	void SetupCharacters();
	void CreateGround();
	void CreateBuildings();
	void SetMapSprites();
	void CreateCharacter(int _team, int _spawnX, int _spawnY, AIActor_Unit::CharClass _class, std::string _actorID, AIConstructorBase& _constructor);

	void SetCharacterStartState();

	// Rendering
	void DisplayButtonPanel(sf::RenderWindow& window);
	void DisplayHUD(sf::RenderWindow& window);
	void DrawCharacters(sf::RenderWindow& window);
	void DrawBuildings(sf::RenderWindow& window);
	void DrawMap(sf::RenderWindow& window);
	void DisplayUIText(sf::RenderWindow& window, std::string _text, float _yPos);

	// Math/Conversion

	int ConvertScreenPosToGridPos(float screenPos);
	float ConvertGridPosToSreenPos(int gridPos);

	// Game Status, Rules & Logic


	void AITurn();


	void UpdateTileStatus();
	void UpdateStatusText(std::string _newText);


	// Player Actions

	

	// Status & Tracking
	AIActor_Unit::TurnState turnState;
	bool mapHighlight[mapWidth][mapHeight];
	bool tileOccupied[mapWidth][mapHeight];

	sf::Clock clock;
	const float turnDelay = 0.5f;

	std::string statusText;


	//Graphics

	sf::Font font;

	TerrainType terrain[mapWidth][mapHeight];
	BuildingType buildings[mapWidth][mapHeight];

	sf::Texture textureGround1;
	sf::Texture textureGround2;
	sf::Texture textureGround3;

	sf::Texture textureHouse;
	sf::Texture textureBush;
	sf::Texture textureCrate;
	sf::Texture textureStoreKeeper;
	sf::Texture textureMushrooms;
	sf::Texture textureWell;
	sf::Texture textureForest;
	sf::Texture textureTrader;
	sf::Texture textureFighter;
	sf::Texture textureWizard;
	sf::Texture textureForge;
	sf::Texture textureCommander;


	std::vector< sf::Sprite > mapArea;
	std::vector< MapBuilding > mapBuildings;

	std::vector< sf::Sprite > buildingSprites;
	std::vector<sf::Sprite> charSprites;
	std::vector<sf::Sprite> hudSprites;

	std::vector<AIActor_Unit*> chars;

	sf::Sprite* moveButtonSprite;

	sf::Texture moveButton;

	
	//Global Access
	AIRandom_Global* Global_Random;
	AIMath_Global*  Global_Math;



};


#endif // !AI_GAME_CONTROLLER_H