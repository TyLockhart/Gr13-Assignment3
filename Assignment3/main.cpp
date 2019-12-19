#include "physics.h"
#include <vector>
#include <SFML/Audio.hpp>
#include <string>
#include "Paddle.h"
#include "Ball.h"

using namespace std;
using namespace sf;

int main()
{
    srand(time(0));
//Declares
    vector<RectangleShape> Menu_Background,Level1_Background,Bar_Background,Lost_Background;
    vector<Vertex> Menu_Vertex,Lost_Vertex;
    vector<Block> Menu_Wall,Menu_Brick,Level1_Brick,Level1_Wall;
    vector<Vector2f> Menu_BrickPoints,Level1_BrickPoints;
    vector<Text> Level1_Text,Menu_Text,Bar_Text,Lost_Text;
    vector<Clock> Menu_Clock,Level1_Clock,Bar_Clock,Lost_Clock;
    vector<bool> Lost_Fades;

    Font Game_Font1;
    Texture Menu_BlurSidesTex;
    SoundBuffer OV_SoundBuffer1;
    Sound OV_Soundtrack1;

    Color Level1_BPurpleC(Color(155,89,182,255)),Level1_BBlueC(Color(52,152,219,255)),Level1_BGreenC(Color(46,204,113,255)),Level1_BYellowC(Color(241,196,15,255)),Level1_BOrangeC(Color(230,126,34,255)),Level1_BRedC(Color(231,76,60,255)),Level1_BrickColor(Color(142,68,173,255)),Menu_BrickColor(Color(243,156,18,255));
    //Next game will have vector<Color>

//Bools
    bool OV_Mouse=true;
    bool Menu=true;
    bool Menu_Once=true;
    bool Game_BarReset=false;

    bool Menu_PaddleRight,Menu_PaddleLeft,Menu_CountDown,Menu_CountUp,Menu_DeleteBlock;
    bool Level1_NextLevel,Level1_CounterOnce,Level1,Level1_Once,Level1_CountDown,Level1_CountUp,Level1_AllowCollisions,Level1_CreateBricks;
    bool Lost,Lost_Once,Lost_CountDown,Lost_CountUp;
    bool Bar,Bar_Once;

//Ints
    int Menu_VxR,Menu_VxG,Menu_VxB,Menu_BrickSpaceX,Menu_BrickSpaceY;
    int Level1_CountOfBlocksLeft,Level1_FadeBricks,Level1_Counter,Level1_CounterRemind,Level1_i,Level1_BrickSpaceX,Level1_BrickSpaceY,Level1_BCR;
    int Bar_Score,Bar_Lives;
    int Lost_VxR,Lost_VxG,Lost_VxB;
    int Game_TotalScore,Game_CurrentLevel;

//Sounds
    OV_SoundBuffer1.loadFromFile("Game_Audio1.ogg");
    OV_Soundtrack1.setBuffer(OV_SoundBuffer1);
    OV_Soundtrack1.setLoop(true);
    OV_Soundtrack1.setVolume(100);
    OV_Soundtrack1.play();

//Fonts
    Game_Font1.loadFromFile("Game_Font1.ttf");

//Textures
    Menu_BlurSidesTex.loadFromFile("Menu_BlurSidesTex.png");

    RenderWindow window(VideoMode(800,800),"Assignment3: Brick Breaker");
    window.setFramerateLimit(60);
    b2World World_Menu(b2Vec2(0.0,0.0)),World_Level1(b2Vec2(0.0,0.1));

    Paddle *Menu_Paddle = NULL,*Level1_Paddle = NULL;
    Ball *Menu_Ball = NULL,*Level1_Ball = NULL;

    while(window.isOpen()&& !Keyboard::isKeyPressed(Keyboard::Escape))
    {
        OV_Mouse=false;
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
            if(event.type==Event::MouseButtonReleased&&event.mouseButton.button==Mouse::Left)
                OV_Mouse=true;
        }
        window.clear();

        if(Menu==true)
        {
            if(Menu_Once==true)
            {
                Menu_PaddleRight=true;
                Menu_PaddleLeft=false;
                Menu_CountDown=true;
                Menu_CountUp=false;
                Menu_DeleteBlock=false;

                Bar=false;
                Bar_Once=false;
                Lost=false;
                Lost_Once=false;

                Menu_VxR=142;
                Menu_VxG=68;
                Menu_VxB=173;
                Menu_BrickSpaceX=0;
                Menu_BrickSpaceY=0;

                Menu_Clock.resize(2);

                for(int i=0; i<Menu_Wall.size(); i++)
                {
                    World_Menu.DestroyBody(Menu_Wall[i]);
                }
                Menu_Wall.clear();

                for(int i=0; i<Menu_Brick.size(); i++)
                {
                    World_Menu.DestroyBody(Menu_Brick[i]);
                }
                Menu_Brick.clear();

                Menu_Background.clear();
                Menu_Text.clear();

                if(Menu_Ball != NULL)
                {
                    Menu_Ball->removeBody();
                }
                if(Menu_Paddle != NULL)
                {
                    Menu_Paddle->removeBody();
                }

                Menu_BrickPoints.clear();
                Menu_BrickPoints.push_back(Vector2f(0,0));
                Menu_BrickPoints.push_back(Vector2f(40,6));
                Menu_BrickPoints.push_back(Vector2f(80,0));
                Menu_BrickPoints.push_back(Vector2f(80,40));
                Menu_BrickPoints.push_back(Vector2f(40,34));
                Menu_BrickPoints.push_back(Vector2f(0,40));
                Menu_BrickPoints.push_back(Vector2f(0,0));


                Menu_Paddle=new Paddle(World_Menu,400,700,150,25);
                Menu_Paddle->setOutlineColor(Color(149,165,166,255));
                Menu_Paddle->setOutlineThickness(4);

                Menu_Ball=new Ball(World_Menu,400,500,25);
                Menu_Ball->setOutlineColor(Color(149,165,166,255));
                Menu_Ball->setOutlineThickness(4);

                Menu_Wall.push_back(physics::createBox(World_Menu,-1,0,1,800,b2_staticBody));
                physics::setBlockColor(Menu_Wall.back(),Color::Transparent);//0 Wall left
                Menu_Wall.push_back(physics::createBox(World_Menu,800,0,1,800,b2_staticBody));
                physics::setBlockColor(Menu_Wall.back(),Color::Transparent);//1 Wall right
                Menu_Wall.push_back(physics::createBox(World_Menu,0,-1,800,1,b2_staticBody));
                physics::setBlockColor(Menu_Wall.back(),Color::Transparent);//2 Wall top
                Menu_Wall.push_back(physics::createBox(World_Menu,0,800,800,1,b2_staticBody));
                physics::setBlockColor(Menu_Wall.back(),Color::Transparent);//3 Wall bottom

                //Creating bricks
                for(int i=1; i<=8; i++)
                {
                    Menu_Brick.push_back(physics::createPolygon(World_Menu,26+Menu_BrickSpaceX,25+Menu_BrickSpaceY,Menu_BrickPoints,b2_staticBody));
                    physics::setBlockColor(Menu_Brick.back(),Menu_BrickColor);
                    physics::setBlockOutline(Menu_Brick.back(),Color(255,255,255,255));
                    physics::setOutlineThickness(Menu_Brick.back(),3);

                    Menu_BrickSpaceX+=95;

                    if(i==8&&Menu_BrickSpaceY<=240)
                    {
                        i=0;
                        Menu_BrickSpaceX=0;
                        Menu_BrickSpaceY+=60;
                    }
                }
                //End of creating bricks

                //Creating rectangle shapes
                Menu_Background.push_back(RectangleShape(Vector2f(250,100)));//behind play
                Menu_Background.back().setFillColor(Color(Menu_BrickColor));
                Menu_Background.back().setOrigin(Vector2f(Menu_Background.back().getSize().x/2,Menu_Background.back().getSize().y/2));
                Menu_Background.back().setPosition(Vector2f(400,520));

                Menu_Background.push_back(RectangleShape(Vector2f(800,800)));
                Menu_Background.back().setFillColor(Color(255,255,255,60));
                Menu_Background.back().setPosition(Vector2f(0,0));
                Menu_Background.back().setTexture(&Menu_BlurSidesTex);

                Menu_Text.push_back(Text("BRICK",Game_Font1,160));
                Menu_Text.back().setPosition(Vector2f(300,250));
                Menu_Text.back().setFillColor(Color::White);
                Menu_Text.back().setOutlineColor(Color::Black);
                Menu_Text.back().setOutlineThickness(7);
                Menu_Text.back().setOrigin(Vector2f(Menu_Text.back().getCharacterSize()/2+40,Menu_Text.back().getCharacterSize()/2));

                Menu_Text.push_back(Text("BREAKER",Game_Font1,100));
                Menu_Text.back().setPosition(Vector2f(275,375));
                Menu_Text.back().setFillColor(Color::White);
                Menu_Text.back().setOutlineColor(Color::Black);
                Menu_Text.back().setOutlineThickness(7);
                Menu_Text.back().setOrigin(Vector2f(Menu_Text.back().getCharacterSize()/2+40,Menu_Text.back().getCharacterSize()/2));

                Menu_Text.push_back(Text("PLAY",Game_Font1,70));
                Menu_Text.back().setPosition(Vector2f(408,510));
                Menu_Text.back().setFillColor(Color::White);
                Menu_Text.back().setOutlineColor(Color::Black);
                Menu_Text.back().setOutlineThickness(7);
                Menu_Text.back().setOrigin(Vector2f(Menu_Text.back().getCharacterSize()/2+60,Menu_Text.back().getCharacterSize()/2));


                Menu_Once=false;
            }
            //Menu background gradient
            Menu_Vertex.clear();
            Menu_Vertex.push_back(Vertex(Vector2f(0,0),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(800,0),Color(Menu_VxR,125,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(800,800),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(0,800),Color(Menu_VxR,125,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(0,0),Color(Menu_VxR,Menu_VxG,Menu_VxB)));

            if(Menu_CountDown==true&&Menu_Clock[0].getElapsedTime().asSeconds()>=0.00001)
            {
                Menu_VxR--;
                Menu_VxG--;
                Menu_VxB--;
                Menu_Clock[0].restart();
            }
            if(Menu_CountUp==true&&Menu_Clock[0].getElapsedTime().asSeconds()>=0.00001)
            {
                Menu_VxR++;
                Menu_VxG++;
                Menu_VxB++;
                Menu_Clock[0].restart();
            }
            if(Menu_VxR==105)
            {
                Menu_CountDown=false;
                Menu_CountUp=true;
            }
            if(Menu_VxR==175)
            {
                Menu_CountUp=false;
                Menu_CountDown=true;
            }
            //End gradient

            for(int i=0; i<Menu_Brick.size(); i++)
            {
                if(Menu_Ball->checkCollision(Menu_Brick[i])==true)
                {
                    if(static_cast<Shape *>(Menu_Brick[i]->GetUserData())->getFillColor()!=Menu_BrickColor)
                    {
                        Menu_Brick[i]->SetType(b2_dynamicBody);
                        physics::spin(Menu_Brick[i],17);
                        Menu_Clock[1].restart();
                    }
                    if(static_cast<Shape *>(Menu_Brick[i]->GetUserData())->getFillColor()==Menu_BrickColor)
                    {
                        physics::setBlockColor(Menu_Brick[i],Color(rand()%255,rand()%255,rand()%255));
                    }
                }
                //Deleting any dynamic bricks after 0.5s after hitting any brick
                if(Menu_Clock[1].getElapsedTime().asSeconds()>=0.5&&physics::isDestroyed(Menu_Brick[i])!=true&&Menu_Brick[i]->GetType()==b2_dynamicBody)
                {
                    physics::deleteBlock(World_Menu,Menu_Brick[i]);
                    Menu_Brick.erase(Menu_Brick.begin()+i);
                }
            }

            //Paddle following ball
            if(Menu_Ball->getPosition().x>Menu_Paddle->getPosition().x&&Menu_Ball->getPosition().y<700)
            {
                Menu_Paddle->setVelocity(Vector2f(275,0));
            }
            else if(Menu_Ball->getPosition().x<Menu_Paddle->getPosition().x&&Menu_Ball->getPosition().y<700)
            {
                Menu_Paddle->setVelocity(Vector2f(-275,0));
            }
            //End paddle following ball

            //Clicking play
            if(Mouse::getPosition(window).x>275&&Mouse::getPosition(window).y>470&&Mouse::getPosition(window).x<524&&Mouse::getPosition(window).y<569)
            {
                Menu_Text[2].setFillColor(Color(rand()%255,rand()%255,rand()%255));
                if(OV_Mouse==true)
                {
                    Menu=false;
                    Level1=true;
                    Level1_Once=true;
                }
            }
            else
            {
                Menu_Text[2].setFillColor(Color::White);
            }

            window.draw(&Menu_Vertex[0],Menu_Vertex.size(),TrianglesFan);//Background gradient
            window.draw(Menu_Background[0]);
            physics::displayWorld(World_Menu,window);
            Menu_Paddle->updatePosition();
            Menu_Ball->updatePosition();
            for(auto i:Menu_Text)
                window.draw(i);
        }
        if(Level1==true)
        {
            if(Level1_Once==true)
            {
                Game_CurrentLevel=1;
                Level1_CountDown=false;
                Level1_CountUp=true;
                Level1_AllowCollisions=false;
                Level1_CreateBricks=true;
                Level1_CounterOnce=true;
                Level1_NextLevel=true;

                Bar=true;
                Bar_Once=true;

                Game_TotalScore=0;
                Bar_Score=0;
                Bar_Lives=999;
                Level1_CountOfBlocksLeft=0;

                if(Game_BarReset==true)
                {
                    Bar_Text[2].setString("POINTS: "+to_string(Bar_Score));
                    Bar_Text[3].setString("LIVES: "+to_string(Bar_Lives));
                    Game_BarReset=false;
                }

                Level1_Counter=0;
                Level1_CounterRemind=0;
                Level1_i=0;
                Level1_BrickSpaceX=0;
                Level1_BrickSpaceY=0;
                Level1_BCR=143;
                Level1_FadeBricks=0;

                Level1_Clock.resize(8);
                Level1_Clock[0].restart();
                Level1_Clock[5].restart();

                for(int i=0; i<Level1_Wall.size(); i++)
                {
                    World_Level1.DestroyBody(Level1_Wall[i]);
                }
                Level1_Wall.clear();

                for(int i=0; i<Level1_Brick.size(); i++)
                {
                    World_Level1.DestroyBody(Level1_Brick[i]);
                }
                Level1_Brick.clear();

                if(Level1_Ball != NULL)
                {
                    Level1_Ball->removeBody();
                }
                if(Level1_Paddle != NULL)
                {
                    Level1_Paddle->removeBody();
                }

                Level1_Text.clear();

                //Creating rectangle shapes
                Level1_Background.push_back(RectangleShape(Vector2f(800,800)));
                Level1_Background.back().setFillColor(Color(Menu_BrickColor));
                Level1_Background.back().setPosition(Vector2f(0,0));

                Level1_Background.push_back(RectangleShape(Vector2f(800,800)));
                Level1_Background.back().setFillColor(Color(0,0,0,0));
                Level1_Background.back().setPosition(Vector2f(0,0));

                //Text
                Level1_Text.push_back(Text("CONGRATS!\n Next level!",Game_Font1,60));
                Level1_Text.back().setPosition(Vector2f(400,400));
                Level1_Text.back().setFillColor(Color(255,255,255,0));
                Level1_Text.back().setOutlineColor(Color::Black);
                Level1_Text.back().setOutlineThickness(7);
                Level1_Text.back().setOrigin(Vector2f(Level1_Text.back().getLocalBounds().width/2,Level1_Text.back().getLocalBounds().height/2));

                Level1_BrickPoints.clear();
                Level1_BrickPoints.push_back(Vector2f(0,0));
                Level1_BrickPoints.push_back(Vector2f(40,6));
                Level1_BrickPoints.push_back(Vector2f(80,0));
                Level1_BrickPoints.push_back(Vector2f(80,40));
                Level1_BrickPoints.push_back(Vector2f(40,34));
                Level1_BrickPoints.push_back(Vector2f(0,40));
                Level1_BrickPoints.push_back(Vector2f(0,0));

                //Paddle and walls
                Level1_Paddle=new Paddle(World_Level1,400,700,150,25);
                Level1_Paddle->setOutlineColor(Color(149,165,166,255));
                Level1_Paddle->setOutlineThickness(4);
                Level1_Ball=new Ball(World_Level1,400,500,25);
                Level1_Ball->setOutlineColor(Color(149,165,166,255));
                Level1_Ball->setOutlineThickness(4);

                Level1_Wall.push_back(physics::createBox(World_Level1,-1,0,1,800,b2_staticBody));
                physics::setBlockColor(Level1_Wall.back(),Color::Transparent);//0 Wall left
                Level1_Wall.push_back(physics::createBox(World_Level1,800,0,1,800,b2_staticBody));
                physics::setBlockColor(Level1_Wall.back(),Color::Transparent);//1 Wall right
                Level1_Wall.push_back(physics::createBox(World_Level1,0,-1,800,1,b2_staticBody));
                physics::setBlockColor(Level1_Wall.back(),Color::Transparent);//2 Wall top
                Level1_Wall.push_back(physics::createBox(World_Level1,0,800,800,1,b2_staticBody));
                physics::setBlockColor(Level1_Wall.back(),Color::Transparent);//3 Wall bottom
                //end paddle walls

                Level1_Once=false;
            }

            if(Level1_Brick.size()==48)
                Level1_AllowCollisions=true;
            //Level1 brick background gradient
            if(Level1_AllowCollisions==true)
            {
                for(int i=0; i<=7; i++)
                    physics::setBlockColor(Level1_Brick[i],Level1_BRedC);//red
                for(int i=8; i<=15; i++)
                    physics::setBlockColor(Level1_Brick[i],Level1_BOrangeC);//orange
                for(int i=16; i<=23; i++)
                    physics::setBlockColor(Level1_Brick[i],Level1_BYellowC);//yellow
                for(int i=24; i<=31; i++)
                    physics::setBlockColor(Level1_Brick[i],Level1_BGreenC);//green
                for(int i=32; i<=39; i++)
                    physics::setBlockColor(Level1_Brick[i],Level1_BBlueC);//blue
                for(int i=40; i<=47; i++)
                    physics::setBlockColor(Level1_Brick[i],Level1_BPurpleC);//purple
            }
            if(Level1_CountDown==true&&Level1_Clock[1].getElapsedTime().asSeconds()>=0.00001)
            {
                Level1_BRedC.r--;
                Level1_BOrangeC.r--;
                Level1_BYellowC.r--;
                Level1_BGreenC.g--;
                Level1_BBlueC.b--;
                Level1_BPurpleC.r--;
                Level1_Clock[1].restart();
            }
            if(Level1_CountUp==true&&Level1_Clock[1].getElapsedTime().asSeconds()>=0.00001)
            {
                Level1_BRedC.r++;
                Level1_BOrangeC.r++;
                Level1_BYellowC.r++;
                Level1_BGreenC.g++;
                Level1_BBlueC.b++;
                Level1_BPurpleC.r++;
                Level1_Clock[1].restart();
            }
            if(Level1_BRedC.r==180)
            {
                Level1_CountDown=false;
                Level1_CountUp=true;
            }
            if(Level1_BRedC.r==231)
            {
                Level1_CountUp=false;
                Level1_CountDown=true;
            }
            //End gradient

            //Creating bricks
            if(Level1_Clock[0].getElapsedTime().asMilliseconds()>0.1&&Level1_Counter<=48)//change to asseconds if wanted slower creation
            {
                Level1_CounterRemind=Level1_Counter;
                Level1_Counter++;
                Level1_Clock[0].restart();
            }
            if(Level1_Counter>Level1_CounterRemind&&Level1_Brick.size()<=47&&Level1_CreateBricks==true)
            {
                Level1_Brick.push_back(physics::createPolygon(World_Level1,26+Level1_BrickSpaceX,65+Level1_BrickSpaceY,Level1_BrickPoints,b2_staticBody));
                physics::setBlockColor(Level1_Brick.back(),Level1_BrickColor);
                physics::setOutlineThickness(Level1_Brick.back(),3);
                physics::setBlockColor(Level1_Brick.back(),Color(0,0,0,255));

                Level1_i++;
                Level1_BrickSpaceX+=95;

                if(Level1_i==8&&Level1_BrickSpaceY<=240)
                {
                    Level1_i=0;
                    Level1_BrickSpaceX=0;
                    Level1_BrickSpaceY+=60;
                }
                Level1_CounterRemind=Level1_Counter;
            }
            //End of creating bricks
            if(Level1_Counter<47)
            {
                Level1_Ball->speed=1;
            }
            if(Level1_Counter>47)
            {
                if(Level1_CounterOnce==true)
                {
                    Level1_CreateBricks=false;
                    Level1_Ball->speed=350;
                    Level1_CounterOnce=false;
                }
                //moving paddle
                if(Keyboard::isKeyPressed(Keyboard::A)==true)
                    Level1_Paddle->setVelocity(Vector2f(-300,0));
                else if(Keyboard::isKeyPressed(Keyboard::D)==false)
                {
                    Level1_Paddle->setVelocity(Vector2f(0,0));
                }
                if(Keyboard::isKeyPressed(Keyboard::D)==true)
                    Level1_Paddle->setVelocity(Vector2f(300,0));
                else if(Keyboard::isKeyPressed(Keyboard::A)==false)
                {
                    Level1_Paddle->setVelocity(Vector2f(0,0));
                }
                //end paddle move
                //check if paddle is going outside
                if(Level1_Paddle->getPosition().x<=75)
                {
                    Level1_Paddle->removeBody();
                    Level1_Paddle=new Paddle(World_Level1,1,700,150,25);
                    Level1_Paddle->setOutlineColor(Color(149,165,166,255));
                    Level1_Paddle->setOutlineThickness(4);
                }
                if(Level1_Paddle->getPosition().x>=725)
                {
                    Level1_Paddle->removeBody();
                    Level1_Paddle=new Paddle(World_Level1,649,700,150,25);
                    Level1_Paddle->setOutlineColor(Color(149,165,166,255));
                    Level1_Paddle->setOutlineThickness(4);
                }
                //end check

                //ball glitch into wall fix//untested
                if(Level1_Ball->getPosition().x<0&&Level1_Ball->getPosition().y<0&&Level1_Ball->getPosition().x>800&&Level1_Ball->getPosition().y>800)
                {
                    Level1_Ball->removeBody();
                    Level1_Ball=new Ball(World_Level1,Level1_Paddle->getPosition().x,Level1_Paddle->getPosition().y-20,25);
                    Level1_Ball->setOutlineColor(Color(149,165,166,255));
                    Level1_Ball->setOutlineThickness(4);
                }
                //if ball is on the bottom layer for 10s, reset pos
                if(Level1_Ball->getPosition().y>700&&Level1_Clock[2].getElapsedTime().asSeconds()>10)
                {
                    Level1_Ball->removeBody();
                    Level1_Ball=new Ball(World_Level1,Level1_Paddle->getPosition().x,Level1_Paddle->getPosition().y-20,25);
                    Level1_Ball->setOutlineColor(Color(149,165,166,255));
                    Level1_Ball->setOutlineThickness(4);
                }
                if(Level1_Ball->getPosition().y<670)
                    Level1_Clock[2].restart();
                //end reset

                if(Level1_AllowCollisions==true)
                {
                    for(int i=0; i<=7; i++) //red bricks collision
                    {
                        if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
                        {
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BRedC)
                            {
                                Level1_Brick[i]->SetType(b2_dynamicBody);
                                physics::spin(Level1_Brick[i],17);
                                Level1_Clock[3].restart();
                            }
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BRedC)
                            {
                                physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
                            }
                        }
                    }
                    for(int i=8; i<=15; i++) //orange bricks collision
                    {
                        if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
                        {
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BOrangeC)
                            {
                                Level1_Brick[i]->SetType(b2_dynamicBody);
                                physics::spin(Level1_Brick[i],17);
                                Level1_Clock[3].restart();
                            }
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BOrangeC)
                            {
                                physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
                            }
                        }
                    }
                    for(int i=16; i<=23; i++) //yellow bricks collision
                    {
                        if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
                        {
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BYellowC)
                            {
                                Level1_Brick[i]->SetType(b2_dynamicBody);
                                physics::spin(Level1_Brick[i],17);
                                Level1_Clock[3].restart();
                            }
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BYellowC)
                            {
                                physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
                            }
                        }
                    }
                    for(int i=24; i<=31; i++) //green bricks collision
                    {
                        if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
                        {
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BGreenC)
                            {
                                Level1_Brick[i]->SetType(b2_dynamicBody);
                                physics::spin(Level1_Brick[i],17);
                                Level1_Clock[3].restart();
                            }
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BGreenC)
                            {
                                physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
                            }
                        }
                    }
                    for(int i=32; i<=39; i++) //blue bricks collision
                    {
                        if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
                        {
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BBlueC)
                            {
                                Level1_Brick[i]->SetType(b2_dynamicBody);
                                physics::spin(Level1_Brick[i],17);
                                Level1_Clock[3].restart();
                            }
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BBlueC)
                            {
                                physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
                            }
                        }
                    }
                    for(int i=40; i<=47; i++) //purple bricks collision
                    {
                        if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
                        {
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BPurpleC)
                            {
                                Level1_Brick[i]->SetType(b2_dynamicBody);
                                physics::spin(Level1_Brick[i],17);
                                Level1_Clock[3].restart();
                            }
                            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BPurpleC)
                            {
                                physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
                            }
                        }
                    }

                    //setting gravity to any brick after .5s
                    for(int i=0; i<Level1_Brick.size(); i++)
                    {
                        if(Level1_Clock[3].getElapsedTime().asSeconds()>=0.5&&physics::isDestroyed(Level1_Brick[i])!=true&&Level1_Brick[i]->GetType()==b2_dynamicBody)
                        {
                            Level1_Brick[i]->SetGravityScale(200);
                        }
                        //falling brick hitting floor
                        if(physics::checkCollision(Level1_Brick[i],Level1_Wall[3])==true)
                        {
                            Level1_Brick[i]->SetTransform(b2Vec2(-10,-10),Level1_Brick[i]->GetAngle());
                            Level1_CountOfBlocksLeft++;
                            Game_TotalScore++;
                            Bar_Score++;
                            Bar_Text[4].setString("+1");
                            Bar_Clock[0].restart();
                            Bar_Text[2].setString("POINTS: "+to_string(Bar_Score));
                        }
                        //falling brick hitting paddle
                        if(physics::checkCollision(Level1_Brick[i])==true&&physics::getPosition(Level1_Brick[i]).y<730&&physics::getPosition(Level1_Brick[i]).y>630)
                        {
                            Level1_Brick[i]->SetTransform(b2Vec2(-10,-10),Level1_Brick[i]->GetAngle());
                            Level1_CountOfBlocksLeft++;
                            Game_TotalScore+=2;
                            Bar_Score+=2;
                            Bar_Text[4].setString("+2");
                            Bar_Clock[0].restart();
                            Bar_Text[2].setString("POINTS: "+to_string(Bar_Score));
                        }
                        //ball hitting floor
                        if(Level1_Ball->checkCollision(Level1_Wall[3])==true&&Level1_CountOfBlocksLeft<47)
                        {
                            cout<<"ball set to pad"<<endl;
                            Level1_Ball->removeBody();
                            Level1_Ball=new Ball(World_Level1,Level1_Paddle->getPosition().x,Level1_Paddle->getPosition().y-20,25);
                            Level1_Ball->setOutlineColor(Color(149,165,166,255));
                            Level1_Ball->setOutlineThickness(4);
                            Bar_Clock[1].restart();
                            Bar_Lives--;
                            Bar_Text[3].setString("LIVES: "+to_string(Bar_Lives));
                        }
                    }
                }
            }
            Level1_Ball->updatePosition();
            Level1_Paddle->updatePosition();
            window.draw(Level1_Background[0]);
            physics::displayWorld(World_Level1,window);
            cout<<Level1_CountOfBlocksLeft<<endl;
            if(Level1_CountOfBlocksLeft==49)
            {
                if(Level1_NextLevel==true)
                {
                    Level1_Clock[7].restart();
                    Level1_Ball->speed=50;
                    Level1_NextLevel=false;
                }

                if(Level1_Clock[6].getElapsedTime().asSeconds()>=0.1&&Level1_Background[1].getFillColor().a<125)
                {
                    Level1_Background[1].setFillColor(Level1_Background[1].getFillColor()+Color(0,0,0,15));
                    Level1_Text[0].setFillColor(Level1_Background[0].getFillColor()+Color(0,0,0,17));
                    Level1_Clock[6].restart();
                }
                else if(Level1_Background[1].getFillColor().a>125)
                {
                    Level1_Text[0].setFillColor(Color(255,255,255,255));
                }
                if(Level1_Clock[7].getElapsedTime().asSeconds()>6)
                {
                    Level1=false;
                    Menu=true;
                    Menu_Once=true;
                }
                window.draw(Level1_Background[1]);
                window.draw(Level1_Text[0]);
            }
        }
        if(Bar==true)
        {
            if(Bar_Once==true)
            {
                Bar_Clock.resize(2);

                Bar_Background.clear();
                Bar_Text.clear();

                Bar_Background.push_back(RectangleShape(Vector2f(800,50)));
                Bar_Background.back().setFillColor(Color(0,0,0,180));
                Bar_Background.back().setPosition(Vector2f(0,0));
                Bar_Background.back().setOutlineThickness(4);
                Bar_Background.back().setOutlineColor(Color(255,255,255,255));

                Bar_Text.push_back(Text("BRICK",Game_Font1,16));
                Bar_Text.back().setPosition(Vector2f(425,13));
                Bar_Text.back().setFillColor(Color::White);
                Bar_Text.back().setOutlineColor(Color::Black);
                Bar_Text.back().setOutlineThickness(2);
                Bar_Text.back().setOrigin(Vector2f(Bar_Text.back().getCharacterSize()/2+40,Bar_Text.back().getCharacterSize()/2));

                Bar_Text.push_back(Text("BREAKER",Game_Font1,16));
                Bar_Text.back().setPosition(Vector2f(410,33));
                Bar_Text.back().setFillColor(Color::White);
                Bar_Text.back().setOutlineColor(Color::Black);
                Bar_Text.back().setOutlineThickness(2);
                Bar_Text.back().setOrigin(Vector2f(Bar_Text.back().getCharacterSize()/2+40,Bar_Text.back().getCharacterSize()/2));

                Bar_Text.push_back(Text("POINTS: "+to_string(Bar_Score),Game_Font1,16));
                Bar_Text.back().setPosition(Vector2f(10,23));
                Bar_Text.back().setFillColor(Color::White);
                Bar_Text.back().setOutlineColor(Color::Black);
                Bar_Text.back().setOutlineThickness(2);

                Bar_Text.push_back(Text("LIVES: "+to_string(Bar_Lives),Game_Font1,16));
                Bar_Text.back().setPosition(Vector2f(700,23));
                Bar_Text.back().setFillColor(Color::White);
                Bar_Text.back().setOutlineColor(Color::Black);
                Bar_Text.back().setOutlineThickness(2);

                Bar_Text.push_back(Text("+1",Game_Font1,16));
                Bar_Text.back().setPosition(Vector2f(Bar_Text[2].getPosition().x+90,23));
                Bar_Text.back().setFillColor(Color(46,204,113,255));
                Bar_Text.back().setOutlineColor(Color::Black);
                Bar_Text.back().setOutlineThickness(2);

                Bar_Text.push_back(Text("-1",Game_Font1,16));
                Bar_Text.back().setPosition(Vector2f(775,23));
                Bar_Text.back().setFillColor(Color(231,76,60,255));
                Bar_Text.back().setOutlineColor(Color::Black);
                Bar_Text.back().setOutlineThickness(2);

                Bar_Once=false;
            }

            //moving +1 over if score reaches 10
            if(Bar_Text[2].getLocalBounds().width>90)
            {
                Bar_Text[4].setPosition(Vector2f(Bar_Text[2].getPosition().x+97,23));
            }

            if(Bar_Lives==0)
            {
                Bar=false;
                Level1=false;
                Lost=true;
                Lost_Once=true;
            }

            for(auto i:Bar_Background)
                window.draw(i);
            for(int i=0; i<=3; i++)
                window.draw(Bar_Text[i]);
            if(Bar_Clock[0].getElapsedTime().asSeconds()<2)
            {
                window.draw(Bar_Text[4]);
            }
            if(Bar_Clock[1].getElapsedTime().asSeconds()<2)
            {
                window.draw(Bar_Text[5]);
            }
        }
        if(Lost==true)
        {
            if(Lost_Once==true)
            {
                Lost_Clock.resize(2);
                Lost_Fades.resize(20);
                Lost_CountDown=true;
                Lost_CountUp=false;

                Lost_VxR=231;
                Lost_VxG=76;
                Lost_VxB=60;

                Lost_Clock[1].restart();
                for(int i=1; i<=9; i++)
                    Lost_Fades[i]=false;

                Lost_Fades[0]=true;

                Lost_Vertex.clear();
                Lost_Background.clear();
                Lost_Text.clear();

                Lost_Background.push_back(RectangleShape(Vector2f(780,780)));
                Lost_Background.back().setFillColor(Color(0,0,0,1));
                Lost_Background.back().setPosition(Vector2f(10,10));
                Lost_Background.back().setOutlineThickness(10);
                Lost_Background.back().setOutlineColor(Color(161,53,42,255));

                Lost_Background.push_back(RectangleShape(Vector2f(260,150)));
                Lost_Background.back().setFillColor(Color(149,165,166,0));//opacity 100
                Lost_Background.back().setPosition(Vector2f(110,450));
                Lost_Background.back().setOutlineThickness(4);
                Lost_Background.back().setOutlineColor(Color(255,255,255,0));

                Lost_Background.push_back(RectangleShape(Vector2f(260,150)));
                Lost_Background.back().setFillColor(Color(149,165,166,0));//opacity 100
                Lost_Background.back().setPosition(Vector2f(410,450));
                Lost_Background.back().setOutlineThickness(4);
                Lost_Background.back().setOutlineColor(Color(255,255,255,0));

                Lost_Text.push_back(Text("YOU LOST! D=",Game_Font1,110));
                Lost_Text.back().setPosition(Vector2f(30,650));
                Lost_Text.back().setFillColor(Color(255,255,255,0));
                Lost_Text.back().setOutlineColor(Color(0,0,0,0));
                Lost_Text.back().setOutlineThickness(2);

                Lost_Text.push_back(Text("You got to Level: "+to_string(Game_CurrentLevel),Game_Font1,65));
                Lost_Text.back().setPosition(Vector2f(100,600));
                Lost_Text.back().setFillColor(Color(255,255,255,0));
                Lost_Text.back().setOutlineColor(Color(0,0,0,0));
                Lost_Text.back().setOutlineThickness(2);

                Lost_Text.push_back(Text("TIME ALIVE: \n"+to_string(Level1_Clock[5].getElapsedTime().asSeconds()),Game_Font1,50));//lv1 time alive
                Lost_Text.back().setPosition(Vector2f(40,40));
                Lost_Text.back().setFillColor(Color(255,255,255,0));
                Lost_Text.back().setOutlineColor(Color(0,0,0,0));
                Lost_Text.back().setOutlineThickness(2);

                Lost_Text.push_back(Text("POINTS: \n"+to_string(Bar_Score),Game_Font1,50));
                Lost_Text.back().setPosition(Vector2f(400,40));
                Lost_Text.back().setFillColor(Color(255,255,255,0));
                Lost_Text.back().setOutlineColor(Color(0,0,0,0));
                Lost_Text.back().setOutlineThickness(2);

                Lost_Text.push_back(Text("TOTAL SCORE: \n"+to_string(Game_TotalScore),Game_Font1,50));
                Lost_Text.back().setPosition(Vector2f(400,200));
                Lost_Text.back().setFillColor(Color(255,255,255,0));
                Lost_Text.back().setOutlineColor(Color(0,0,0,0));
                Lost_Text.back().setOutlineThickness(2);

                Lost_Text.push_back(Text("MENU",Game_Font1,50));
                Lost_Text.back().setOrigin(Vector2f(Lost_Text.back().getLocalBounds().width/2,Lost_Text.back().getLocalBounds().height/2));
                Lost_Text.back().setPosition(Vector2f(Lost_Background[1].getPosition().x+(Lost_Background[1].getSize().x/2),Lost_Background[1].getPosition().y+(Lost_Background[1].getSize().y/2)-10));
                Lost_Text.back().setFillColor(Color(255,255,255,0));
                Lost_Text.back().setOutlineColor(Color(0,0,0,0));
                Lost_Text.back().setOutlineThickness(2);

                Lost_Text.push_back(Text("RESTART",Game_Font1,50));
                Lost_Text.back().setOrigin(Vector2f(Lost_Text.back().getLocalBounds().width/2,Lost_Text.back().getLocalBounds().height/2));
                Lost_Text.back().setPosition(Vector2f(Lost_Background[2].getPosition().x+(Lost_Background[2].getSize().x/2),Lost_Background[2].getPosition().y+(Lost_Background[2].getSize().y/2)-10));
                Lost_Text.back().setFillColor(Color(255,255,255,0));
                Lost_Text.back().setOutlineColor(Color(0,0,0,0));
                Lost_Text.back().setOutlineThickness(2);

                Lost_Once=false;
            }

            //Menu background gradient
            Lost_Vertex.clear();
            Lost_Vertex.push_back(Vertex(Vector2f(0,0),Color(Lost_VxR,Lost_VxG,Lost_VxB)));
            Lost_Vertex.push_back(Vertex(Vector2f(800,0),Color(Lost_VxR,125,Lost_VxB)));
            Lost_Vertex.push_back(Vertex(Vector2f(800,800),Color(Lost_VxR,Lost_VxG,Lost_VxB)));
            Lost_Vertex.push_back(Vertex(Vector2f(0,800),Color(Lost_VxR,125,Lost_VxB)));
            Lost_Vertex.push_back(Vertex(Vector2f(0,0),Color(Lost_VxR,Lost_VxG,Lost_VxB)));

            if(Lost_CountDown==true&&Lost_Clock[0].getElapsedTime().asSeconds()>=0.00001)
            {
                Lost_VxR--;
                Lost_VxG--;
                Lost_VxB--;
                Lost_Clock[0].restart();
            }
            if(Lost_CountUp==true&&Lost_Clock[0].getElapsedTime().asSeconds()>=0.00001)
            {
                Lost_VxR++;
                Lost_VxG++;
                Lost_VxB++;
                Lost_Clock[0].restart();
            }
            if(Lost_VxR==175)
            {
                Lost_CountDown=false;
                Lost_CountUp=true;
            }
            if(Lost_VxR==231)
            {
                Lost_CountUp=false;
                Lost_CountDown=true;
            }
            //End gradient

            if(Lost_Fades[0]==true&&Lost_Clock[1].getElapsedTime().asSeconds()>0.05&&Lost_Text[0].getColor().a<255)
            {
                Lost_Text[0].setFillColor(Lost_Text[0].getFillColor() + Color(0,0,0,15));
                Lost_Text[0].setOutlineColor(Lost_Text[0].getOutlineColor() + Color(0,0,0,15));
                Lost_Clock[1].restart();
            }
            else if(Lost_Text[0].getColor().a==255)
            {
                Lost_Fades[0]=false;
                Lost_Fades[1]=true;
            }//fade in you lost

            if(Lost_Fades[1]==true&&Lost_Clock[1].getElapsedTime().asSeconds()>0.05&&Lost_Text[1].getColor().a<255)
            {
                Lost_Text[1].setFillColor(Lost_Text[1].getFillColor() + Color(0,0,0,15));
                Lost_Text[1].setOutlineColor(Lost_Text[1].getOutlineColor() + Color(0,0,0,15));
                Lost_Clock[1].restart();
            }
            else if(Lost_Text[1].getColor().a==255)
            {
                Lost_Fades[1]=false;
                Lost_Fades[2]=true;
            }//fade in level got to

            if(Lost_Fades[2]==true&&Lost_Clock[1].getElapsedTime().asSeconds()>0.05&&Lost_Text[2].getColor().a<255)
            {
                Lost_Text[2].setFillColor(Lost_Text[2].getFillColor() + Color(0,0,0,15));
                Lost_Text[2].setOutlineColor(Lost_Text[2].getOutlineColor() + Color(0,0,0,15));
                Lost_Clock[1].restart();
            }
            else if(Lost_Text[2].getColor().a==255)
            {
                Lost_Fades[2]=false;
                Lost_Fades[3]=true;
            }//fade in time

            if(Lost_Fades[3]==true&&Lost_Clock[1].getElapsedTime().asSeconds()>0.05&&Lost_Text[3].getColor().a<255)
            {
                Lost_Text[3].setFillColor(Lost_Text[3].getFillColor() + Color(0,0,0,15));
                Lost_Text[3].setOutlineColor(Lost_Text[3].getOutlineColor() + Color(0,0,0,15));
                Lost_Clock[1].restart();
            }
            else if(Lost_Text[3].getColor().a==255)
            {
                Lost_Fades[3]=false;
                Lost_Fades[4]=true;
            }//fade points

            if(Lost_Fades[4]==true&&Lost_Clock[1].getElapsedTime().asSeconds()>0.05&&Lost_Text[4].getColor().a<255)
            {
                Lost_Text[4].setFillColor(Lost_Text[4].getFillColor() + Color(0,0,0,15));
                Lost_Text[4].setOutlineColor(Lost_Text[4].getOutlineColor() + Color(0,0,0,15));
                Lost_Clock[1].restart();
            }
            else if(Lost_Text[4].getColor().a==255)
            {
                Lost_Fades[4]=false;
                Lost_Fades[5]=true;
            }//fade total score

            if(Lost_Fades[5]==true&&Lost_Clock[1].getElapsedTime().asSeconds()>0.05&&Lost_Background[1].getFillColor().a<100)
            {
                Lost_Background[1].setFillColor(Lost_Background[1].getFillColor() + Color(0,0,0,10));
                Lost_Background[1].setOutlineColor(Lost_Background[1].getOutlineColor() + Color(0,0,0,10));
                Lost_Clock[1].restart();
            }
            else if(Lost_Background[1].getFillColor().a==100)
            {
                Lost_Fades[5]=false;
                Lost_Fades[6]=true;
            }//fade menu box

            if(Lost_Fades[6]==true&&Lost_Clock[1].getElapsedTime().asSeconds()>0.05&&Lost_Text[5].getColor().a<255)
            {
                Lost_Text[5].setFillColor(Lost_Text[5].getFillColor() + Color(0,0,0,15));
                Lost_Text[5].setOutlineColor(Lost_Text[5].getOutlineColor() + Color(0,0,0,15));
                Lost_Clock[1].restart();
            }
            else if(Lost_Text[5].getColor().a==255)
            {
                Lost_Fades[6]=false;
                Lost_Fades[7]=true;
            }//fade menu text

            if(Lost_Fades[7]==true&&Lost_Clock[1].getElapsedTime().asSeconds()>0.05&&Lost_Background[2].getFillColor().a<100)
            {
                Lost_Background[2].setFillColor(Lost_Background[2].getFillColor() + Color(0,0,0,10));
                Lost_Background[2].setOutlineColor(Lost_Background[2].getOutlineColor() + Color(0,0,0,10));
                Lost_Clock[1].restart();
            }
            else if(Lost_Background[2].getFillColor().a==100)
            {
                Lost_Fades[7]=false;
                Lost_Fades[8]=true;
            }//fade restart box

            if(Lost_Fades[8]==true&&Lost_Clock[1].getElapsedTime().asSeconds()>0.05&&Lost_Text[6].getColor().a<255)
            {
                Lost_Text[6].setFillColor(Lost_Text[6].getFillColor() + Color(0,0,0,15));
                Lost_Text[6].setOutlineColor(Lost_Text[6].getOutlineColor() + Color(0,0,0,15));
                Lost_Clock[1].restart();
            }
            else if(Lost_Text[6].getColor().a==255)
            {
                Lost_Fades[8]=false;
                Lost_Fades[9]=true;
            }//fade restart text

            if(Lost_Fades[9]==true)
            {
                if(Mouse::getPosition(window).x>106&&Mouse::getPosition(window).y>446&&Mouse::getPosition(window).x<373&&Mouse::getPosition(window).y<603)
                {
                    //on menu box
                    Lost_Text[5].setCharacterSize(60);
                    Lost_Text[5].setFillColor(Color(243,156,18,255));
                    Lost_Text[5].setPosition(Vector2f(Lost_Background[1].getPosition().x+(Lost_Background[1].getSize().x/2)-15,Lost_Background[1].getPosition().y+(Lost_Background[1].getSize().y/2)-15));
                    if(OV_Mouse==true)
                    {
                        Game_BarReset=true;
                        Menu=true;
                        Menu_Once=true;
                        Lost=false;
                    }
                }
                else
                {
                    Lost_Text[5].setCharacterSize(50);
                    Lost_Text[5].setFillColor(Color(255,255,255,255));
                    Lost_Text[5].setPosition(Vector2f(Lost_Background[1].getPosition().x+(Lost_Background[1].getSize().x/2),Lost_Background[1].getPosition().y+(Lost_Background[1].getSize().y/2)-10));
                }

                if(Mouse::getPosition(window).x>406&&Mouse::getPosition(window).y>446&&Mouse::getPosition(window).x<673&&Mouse::getPosition(window).y<603)
                {
                    //on restart box
                    Lost_Text[6].setCharacterSize(60);
                    Lost_Text[6].setFillColor(Color(243,156,18,255));
                    Lost_Text[6].setPosition(Vector2f(Lost_Background[2].getPosition().x+(Lost_Background[2].getSize().x/2)-15,Lost_Background[2].getPosition().y+(Lost_Background[2].getSize().y/2)-15));
                    if(OV_Mouse==true)
                    {
                        Game_BarReset=true;
                        Level1=true;
                        Level1_Once=true;
                        Lost=false;
                    }
                }
                else
                {
                    Lost_Text[6].setCharacterSize(50);
                    Lost_Text[6].setFillColor(Color(255,255,255,255));
                    Lost_Text[6].setPosition(Vector2f(Lost_Background[2].getPosition().x+(Lost_Background[2].getSize().x/2),Lost_Background[2].getPosition().y+(Lost_Background[2].getSize().y/2)-10));
                }
            }
            window.draw(&Lost_Vertex[0],Lost_Vertex.size(),TrianglesFan);//Background gradient
            for(auto i:Lost_Background)
                window.draw(i);
            for(auto i:Lost_Text)
                window.draw(i);
        }
        window.draw(Menu_Background[1]);
        window.display();
    }

    return 0;
}
