#include "GameController.h"

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

GameController::GameController()
{

    // obtain Math and Random singletons
    Global_Random = AIRandom_Global::getInstance();
    Global_Math = AIMath_Global::getInstance();
}

GameController::~GameController()
{

}



// ** Intialisation **

/*
* Init should be called prior to starting to game
*/

void GameController::Init()
{
    //setup display
    LoadGraphicsAssests();
    CreateGround();
    CreateBuildings();
    SetMapSprites();

    // initialise the AI 
    InitAI();

    //initialise the game environment
    InitGame();

}


/*
*InitGame should be called once to setup all the game components and set the game ready for the first turn
*/
void GameController::InitGame()
{
    SetupCharacters();
    SetCharacterStartState();
    UpdateTileStatus();
    activeChar = 0;
    turnState = AIActor_Unit::TurnState::Waiting;
    statusText = "Waiting for Player";
}


/*
* InitAI initalises the AIConstructor. Must be called before game starts.
*/

void GameController::InitAI()
{
    //DefineAI will initialise all of the Actions, Considerations and Options used by the AI
    goapAIConstructor.Init();
    goapAIConstructor.DefineAI();


}



/*
LoadGraphicsAssests will load the graphics assets from file erady for use
*/
void GameController::LoadGraphicsAssests()
{
    bool load = textureGround1.loadFromFile("Assets/Ground1.png");
    load = textureGround2.loadFromFile("Assets/Ground2.png");
    load = textureGround3.loadFromFile("Assets/Ground3.png");
    load = textureFighter.loadFromFile("Assets/Fighter_v3.png");

    load = textureHouse.loadFromFile("Assets/Shop.png");
    load = textureStoreKeeper.loadFromFile("Assets/Storekeeper.png");
    load = textureCrate.loadFromFile("Assets/Crate.png");
    load = textureBush.loadFromFile("Assets/Bush.png");
    load = textureWell.loadFromFile("Assets/Well.png");
    load = textureMushrooms.loadFromFile("Assets/Mushrooms.png");
    load = textureForest.loadFromFile("Assets/Tree.png");
    load = textureTrader.loadFromFile("Assets/Trader.png");

    load = textureForge.loadFromFile("Assets/Forge.png");
    load = textureCommander.loadFromFile("Assets/Commander.png");

    load = moveButton.loadFromFile("Assets/ButtonPlan.png");

    moveButtonSprite = new sf::Sprite(moveButton);

    font.openFromFile("Assets/DefaultAriel.ttf");

}



void GameController::UpdateCharStatus(AIActor_Unit::TurnState _turnState)
{
    turnState = _turnState;
}

/*
* CreateCharacter will create and initialise a AIActor_Unit with the provided settings
*/
void GameController::CreateCharacter(int _team, int _spawnX, int _spawnY, AIActor_Unit::CharClass _class, string _actorID, AIConstructorBase& _constructor)
{
    AIActor_Unit* _c = new AIActor_Unit(_class);
    _c->Spawn(_spawnX, _spawnY);
    _c->Init(_actorID, _constructor);
    _c->InitChar(true,  _class, _team);
    _c->SetGameContext(*this);

    _c->InitPathfinding();

    chars.push_back(_c);

    charSprites.push_back(sf::Sprite(textureFighter));



}

/*
* SetupCharacters should be used to configure any character that appears on the map
* 

* 
*/
void GameController::SetupCharacters()
{


    // only 1 character for example

    //team, posx, posy, class, name,  AI constructor
    CreateCharacter(0, 1, 1, AIActor_Unit::CharClass::Fighter, "HumanFighter1", goapAIConstructor);




}

void GameController::SetCharacterStartState()
{
    for (auto& c : chars)
    {
        c->InitWorldState();
    }
}




/**
* CreateGround will set the map tile terrain types
 - currently all grass
*/
void GameController::CreateGround()
{
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            float _groundType = Global_Random->GetRandom01();

            if (_groundType < 0.8f)
            {
                terrain[x][y] = TerrainType::Ground1;
            }else if(_groundType >=0.8 && _groundType <0.9f){
                terrain[x][y] = TerrainType::Ground2;
            }
            else
            {
                terrain[x][y] = TerrainType::Ground3;
            }

            mapHighlight[x][y] = false;
        }

    }
}


/**
* CreateGround will set the building tile terrain types
*/
void GameController::CreateBuildings()
{

    sf::Sprite s(textureHouse);
    s.setPosition({  6 * mapSectionXY, 0 * mapSectionXY });
    buildingSprites.push_back(s);


    sf::Sprite sk(textureStoreKeeper);
    sk.setPosition({ 7 * mapSectionXY, 4 * mapSectionXY });
    buildingSprites.push_back(sk);


    sf::Sprite b(textureBush);
    b.setPosition({ 3 * mapSectionXY, 3 * mapSectionXY });
    buildingSprites.push_back(b);


    sf::Sprite m(textureMushrooms);
    m.setPosition({ 13 * mapSectionXY, 8 * mapSectionXY });
    buildingSprites.push_back(m);


    sf::Sprite mt(textureForest);
    mt.setPosition({ 13 * mapSectionXY, 5 * mapSectionXY });
    buildingSprites.push_back(mt);


    sf::Sprite mt2(textureForest);
    mt2.setPosition({ 10 * mapSectionXY, 5 * mapSectionXY });
    buildingSprites.push_back(mt2);



    sf::Sprite w(textureWell);
    w.setPosition({ 1 * mapSectionXY, 7 * mapSectionXY });
    buildingSprites.push_back(w);

    sf::Sprite t(textureTrader);
    t.setPosition({ 15 * mapSectionXY, 3 * mapSectionXY });
    buildingSprites.push_back(t);

    sf::Sprite c(textureCrate);
    c.setPosition({ 14 * mapSectionXY, 2 * mapSectionXY });
    buildingSprites.push_back(c);

    sf::Sprite c2(textureCrate);
    c2.setPosition({ 16 * mapSectionXY, 2 * mapSectionXY });
    buildingSprites.push_back(c2);


    sf::Sprite c3(textureCrate);
    c3.setPosition({ 1 * mapSectionXY, 4 * mapSectionXY });
    buildingSprites.push_back(c3);


    sf::Sprite c4(textureCrate);
    c4.setPosition({ 2 * mapSectionXY, 4 * mapSectionXY });
    buildingSprites.push_back(c4);


    sf::Sprite co(textureCommander);
    co.setPosition({ 18 * mapSectionXY, 8 * mapSectionXY });
    buildingSprites.push_back(co);

    sf::Sprite fo(textureForge);
    fo.setPosition({ 6 * mapSectionXY, 7 * mapSectionXY });
    buildingSprites.push_back(fo);
}


/*
* SetMapSprites will create the sprites for the map terrain
*/
void GameController::SetMapSprites()
{
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            if (terrain[x][y] == TerrainType::Ground1)
            {
                mapArea.push_back(sf::Sprite(textureGround1));
            }else if (terrain[x][y] == TerrainType::Ground2)
            {
                mapArea.push_back(sf::Sprite(textureGround2));
            }
            else if (terrain[x][y] == TerrainType::Ground3)
            {
                mapArea.push_back(sf::Sprite(textureGround3));
            }


        }
    }
}






//** Events  **


/*
* HandleKeyPress will handle the SFML key presses
*/

void GameController::HandleKeyPress(const sf::Event::KeyPressed* _keyPress)
{

    if (!gameRunning)
        return;



}


/*
* HandleKeyPress will handle the SFML mouse presses
*/
void GameController::HandleMousePress(const sf::Event::MouseButtonPressed* _mousePress)
{

    if (!gameRunning)
        return;

    if (_mousePress->button == sf::Mouse::Button::Right)
    {


    }
    else if (_mousePress->button == sf::Mouse::Button::Left)
    {

        // based in the x,y click position and game state, check each button for actions

        sf::Vector2f _clickPos(_mousePress->position.x, _mousePress->position.y);

        if (turnState == AIActor_Unit::TurnState::Waiting)
        {

            // trigger the Plan when button is clicked

            if (moveButtonSprite->getGlobalBounds().contains(_clickPos))
            {
                 turnState = AIActor_Unit::TurnState::Planning;
                 chars[activeChar]->UpdateTurnStatus(AIActor_Unit::TurnState::Planning);

            }

        }
      
     

    }
}




//**  Game Loop **



/*
* Update is called each frame and should only contain code desined to be called this often
*/
void GameController::Update(float dt)
{

    if (gameRunning)
    {
        if (turnState == AIActor_Unit::TurnState::Waiting)
        {
            statusText = "Character Waiting for Instruction";
        }
        else if (turnState == AIActor_Unit::TurnState::Planning) {
            statusText = "Character Is Planning";
            AITurn();
        }
        else if (turnState == AIActor_Unit::TurnState::Executing)
        {
            statusText = "Character Is Executing Plan";
            AITurn();
        }
        else if (turnState == AIActor_Unit::TurnState::GoalReached)
        {
            statusText = "Character Has Achieved Goal";
    
        }
    }
}



/*
*StartGame will start the game running, allowing actions to take place
*/

void GameController::StartGame()
{
    gameRunning = true;
}


/*
*StartGame will start the game running, allowing actions to take place
*/

void GameController::EndGame()
{
    gameRunning = false;
}











// ** Rendering **




void GameController::Render(sf::RenderWindow& window)
{
    DrawMap(window);
    DrawBuildings(window);
    DrawCharacters(window);

}


/*
* DisplayUI will display the button input panel, and the lower HUD
*/
void GameController::DisplayUI(sf::RenderWindow& window)
{

    DisplayHUD(window);
    DisplayButtonPanel(window);


}


void GameController::DisplayUIText(sf::RenderWindow& window, std::string _text, float _yPos)
{

    sf::Text text(font); // a font is required to make a text object



    // set the string to display
    text.setString(_text);

    // set the character size
    text.setCharacterSize(20); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);
    text.setPosition({ 1300,_yPos });

    window.draw(text);

    ostringstream os2;

}


void GameController::DisplayHUD(sf::RenderWindow& window)
{

    ostringstream os1;
    os1 << "Key ______  Value "  <<std::endl;

    DisplayUIText(window, os1.str(), 150 );


    int _startY = 200;
    int _offset = 50;
    int _i = 0;

    for (auto k : chars[activeChar]->GetStatusKeys())
    {
        ostringstream os;
        os <<  k << " = " << chars[activeChar]->GetBBValue<bool>(k) << std::endl;

        DisplayUIText(window, os.str(), _startY + (_offset * _i) );

        _i++;
    }


    sf::Text text(font);



    // status text
    ostringstream os;


    os << statusText << std::endl;
    string s = os.str();

    // set the string to display
    text.setString(s);

    // set the character size
    text.setCharacterSize(36); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);
    text.setPosition({ 250.f,675.f });

    window.draw(text);


    // action text
    ostringstream os2;


    os2 << chars[activeChar]->currentAction << std::endl;
    string s2 = os2.str();

    // set the string to display
    text.setString(s2);

    // set the character size
    text.setCharacterSize(36); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);
    text.setPosition({ 250.f,725.f });

    window.draw(text);



}

/*
* DisplayButtonPanel will show hte button panel on the right side of the scree that the user used to take their turn
*/
void GameController::DisplayButtonPanel(sf::RenderWindow& window)
{

    sf::Text text(font); // a font is required to make a text object


    ostringstream os;


    os << "GOAP Game";
    string s = os.str();

    // set the string to display
    text.setString(s);

    // set the character size
    text.setCharacterSize(28); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);
    text.setPosition({ 1400,0.f });

    window.draw(text);

    moveButtonSprite->setPosition(sf::Vector2f(50, 650));
    window.draw(*moveButtonSprite);

    
}




/*
* DrawMap will render all the map tiles
*/
void GameController::DrawMap(sf::RenderWindow& window)
{
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            int _index = (x * mapHeight) + y;

            mapArea[_index].setPosition({ x * mapSectionXY, y * mapSectionXY });
            window.draw(mapArea[_index]);
        }
    }
}

/*
* DrawMap will render all the map tiles
*/
void GameController::DrawBuildings(sf::RenderWindow& window)
{
    for (auto &b : buildingSprites)
    {
        window.draw(b);
    }


}


/*
* DrawCharacters will draw each of the characters on the map at their current position
*/
void GameController::DrawCharacters(sf::RenderWindow& window)
{
    for (int i = 0; i < chars.size(); i++)
    {
        if (chars[i]->isActive)
        {
            charSprites[i].setPosition({ ConvertGridPosToSreenPos(chars[i]->gridPosX),ConvertGridPosToSreenPos(chars[i]->gridPosY) });
            window.draw(charSprites[i]);
        }

    }
}



// ** Game Status **



/*
* UpdateStatusText will set the text that appears below the game play area, as well as log to the console output
*/
void GameController::UpdateStatusText(std::string _newText)
{
    cout << "TURN LOG: " << _newText << std::endl;
    statusText = _newText;
}


/*
/ UpdateTileStatus will refresh the tileOccupied variable based on the current character positions
*/
void GameController::UpdateTileStatus()
{
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            tileOccupied[x][y] = false;
        }
    }

    for (int i=0;i<chars.size();i++)
    {
        if (chars[i]->isActive)
        {
           tileOccupied[chars[i]->gridPosX][chars[i]->gridPosY] = true;
        }
        
    }

}

// ** Game Rules and Logic **



/*
* AITurn will tick the active character, causing them to select and execude an action for their turn
*/
void GameController::AITurn()
{

    // implement a slight delay, so the player sees the AI thinking and then acting
    if (clock.getElapsedTime().asSeconds() < turnDelay)
        return;

   // std::cout << "AI Actor is acting..." << std::endl;

    chars[activeChar]->Update(turnDelay);
    clock.restart();
   



}






// ** Conversion / Math **




int GameController::ConvertScreenPosToGridPos(float screenPos)
{

    return (screenPos / mapSectionXY);
}

float GameController::ConvertGridPosToSreenPos(int gridPos)
{

    return (gridPos * mapSectionXY);
}







