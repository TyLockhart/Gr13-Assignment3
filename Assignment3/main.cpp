//LEARNED: ALL VECTORS NEED A PUSHBACK FIRST, also learned resize

//!!work o nsetting paddle same x as ball
//!!work on creating menu bricks correctly
//!!work on spinning menu bricks
//!!work on menu text and paly  button
//Work on creating level1 bricks

#include "physics.h"
#include <vector>
#include <string>
#include "Paddle.h"
#include "Ball.h"

using namespace std;
using namespace sf;


int main()
{
    srand(time(0));
//Declares
    vector<RectangleShape> Menu_Background,Level1_Background;
    vector<Vertex> Menu_Vertex;
    vector<Block> Menu_Wall,Menu_Brick,Level1_Brick;
    vector<Vector2f> Menu_BrickPoints;
    vector<Text> Menu_Text;
    vector<Clock> Menu_Clock,Level1_Clock;

    Font Game_Font1;

    Color Menu_BrickColor(Color(243,156,18,255));

//Bools
    bool OV_Mouse=true;

    bool Menu=true;
    bool Menu_Once=true;
    bool Menu_PaddleRight=false;
    bool Menu_PaddleLeft=false;
    bool Menu_CountDown=false;
    bool Menu_CountUp=false;
    bool Menu_DeleteBlock=false;

    bool Level1=false;
    bool Level1_Once=false;

//Int
    int Menu_VxR=142;
    int Menu_VxG=68;
    int Menu_VxB=173;
    int Menu_BrickSpaceX=0;
    int Menu_BrickSpaceY=0;

    int Level1_Counter=0;
    int Level1_CounterRemind=0;
    int Level1_i=0;

//Points
    Menu_BrickPoints.push_back(Vector2f(0,0));
    Menu_BrickPoints.push_back(Vector2f(40,6));
    Menu_BrickPoints.push_back(Vector2f(80,0));
    Menu_BrickPoints.push_back(Vector2f(80,40));
    Menu_BrickPoints.push_back(Vector2f(40,34));
    Menu_BrickPoints.push_back(Vector2f(0,40));
    Menu_BrickPoints.push_back(Vector2f(0,0));

//Font
    Game_Font1.loadFromFile("Game_Font1.ttf");







    RenderWindow window(VideoMode(800,800),"Assignment3: Brick Breaker");
    window.setFramerateLimit(60);
    b2World World_Menu(b2Vec2(0.0,0.0));
    Paddle *Menu_Paddle;
    Ball *Menu_Ball;


    while(window.isOpen()&& !Keyboard::isKeyPressed(Keyboard::Escape))
    {
        OV_Mouse=false;
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }
        if(event.type==Event::MouseButtonReleased&&event.mouseButton.button==Mouse::Left) OV_Mouse=true;

        window.clear();

//    cout<<Mouse::getPosition(window).x<<": Mouse X-Pos"<<endl;
//    cout<<Mouse::getPosition(window).y<<": Mouse Y-Pos"<<endl;
//    cout<<"---"<<endl;
    if(OV_Mouse==true) cout<<"Clicked Mouse"<<endl;

    if(Menu==true)
    {
        if(Menu_Once==true)
        {
            Menu_PaddleRight=true;
            Menu_PaddleLeft=false;
            Menu_CountDown=true;
            Menu_CountUp=false;
            Menu_DeleteBlock=false;

            Menu_Clock.resize(2);

            for(int i=0; i<Menu_Wall.size(); i++){
            if(physics::isDestroyed(Menu_Wall[i])!=true)
            {
                physics::deleteBlock(World_Menu,Menu_Wall[i]);
                Menu_Wall.clear();
            }
            }
            for(int i=0; i<Menu_Brick.size(); i++){
            if(physics::isDestroyed(Menu_Brick[i])!=true)
            {
                physics::deleteBlock(World_Menu,Menu_Brick[i]);
                Menu_Brick.clear();
            }
            }
            Menu_Background.clear();
            Menu_Text.clear();


            Menu_Paddle=new Paddle(World_Menu,400,700,150,25);
            Menu_Paddle->setOutlineColor(Color(149,165,166,255));
            Menu_Paddle->setOutlineThickness(6);
            Menu_Ball=new Ball(World_Menu,400,500,25);

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

        Menu_Text.push_back(Text());
        Menu_Text.back().setCharacterSize(160);
        Menu_Text.back().setFont(Game_Font1);
        Menu_Text.back().setPosition(Vector2f(300,250));
        Menu_Text.back().setString("BRICK");
        Menu_Text.back().setFillColor(Color::White);
        Menu_Text.back().setOutlineColor(Color::Black);
        Menu_Text.back().setOutlineThickness(7);
        Menu_Text.back().setOrigin(Vector2f(Menu_Text.back().getCharacterSize()/2+40,Menu_Text.back().getCharacterSize()/2));

        Menu_Text.push_back(Text());
        Menu_Text.back().setCharacterSize(100);
        Menu_Text.back().setFont(Game_Font1);
        Menu_Text.back().setPosition(Vector2f(275,375));
        Menu_Text.back().setString("BREAKER");
        Menu_Text.back().setFillColor(Color::White);
        Menu_Text.back().setOutlineColor(Color::Black);
        Menu_Text.back().setOutlineThickness(7);
        Menu_Text.back().setOrigin(Vector2f(Menu_Text.back().getCharacterSize()/2+40,Menu_Text.back().getCharacterSize()/2));

        Menu_Text.push_back(Text());
        Menu_Text.back().setCharacterSize(70);
        Menu_Text.back().setFont(Game_Font1);
        Menu_Text.back().setPosition(Vector2f(408,510));
        Menu_Text.back().setString("PLAY");
        Menu_Text.back().setFillColor(Color::White);
        Menu_Text.back().setOutlineColor(Color::Black);
        Menu_Text.back().setOutlineThickness(7);
        Menu_Text.back().setOrigin(Vector2f(Menu_Text.back().getCharacterSize()/2+60,Menu_Text.back().getCharacterSize()/2));


            Menu_Once=false;
        }
        //Menu background gradient
            Menu_Vertex.clear();
            Menu_Vertex.push_back(Vertex(Vector2f(0,0),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(800,0),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(800,800),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(0,800),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
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




        for(int i=0;i<Menu_Brick.size();i++){
        if(Menu_Ball->checkCollision(Menu_Brick[i])==true)
        {
            if(static_cast<Shape *>(Menu_Brick[i]->GetUserData())->getFillColor()!=Menu_BrickColor){
            Menu_Brick[i]->SetType(b2_dynamicBody);
            physics::spin(Menu_Brick[i],17);
            Menu_Clock[1].restart();
            }
            if(static_cast<Shape *>(Menu_Brick[i]->GetUserData())->getFillColor()==Menu_BrickColor){
            physics::setBlockColor(Menu_Brick[i],Color(rand()%255,rand()%255,rand()%255));
            }
        }
            //Deleting any dynamic bricks after 0.5s after hitting any brick
            if(Menu_Clock[1].getElapsedTime().asSeconds()>=0.5&&physics::isDestroyed(Menu_Brick[i])!=true&&Menu_Brick[i]->GetType()==b2_dynamicBody){
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
    else{
        Menu_Text[2].setFillColor(Color::White);
    }




        window.draw(&Menu_Vertex[0],Menu_Vertex.size(),TrianglesFan);//Background gradient
        for(auto i:Menu_Background) window.draw(i);
//        for(auto i:Menu_Brick) window.draw(i);
        physics::displayWorld(World_Menu,window);
//        window.draw(*Menu_Paddle);
        Menu_Paddle->updatePosition();
//        window.draw(*Menu_Ball);
        Menu_Ball->updatePosition();
        for(auto i:Menu_Text) window.draw(i);
    }
    if(Level1==true)
    {
        if(Level1_Once==true)
        {
            Level1_Counter=0;
            Level1_CounterRemind=0;
            Level1_i=0;

            Level1_Clock.resize(2);
            Level1_Clock[0].restart();

            //Creating rectangle shapes
            Level1_Background.push_back(RectangleShape(Vector2f(800,800)));
            Level1_Background.back().setFillColor(Color(Menu_BrickColor));
            Level1_Background.back().setPosition(Vector2f(0,0));

            Level1_Once=false;
        }

    if(Level1_Clock[0].getElapsedTime().asSeconds()<5)
    {
        Level1_CounterRemind=Level1_Counter;
        Level1_Counter++;

    }
    if(Level1_Counter>Level1_CounterRemind)
    {
        //Creating bricks
        Level1_Brick.push_back(physics::createPolygon(World_Menu,26+Menu_BrickSpaceX,25+Menu_BrickSpaceY,Menu_BrickPoints,b2_staticBody));
        physics::setBlockColor(Level1_Brick.back(),Menu_BrickColor);

        Level1_i++;

        Menu_BrickSpaceX+=95;

        if(Level1_i==8&&Menu_BrickSpaceY<=240)
        {
            Level1_i=0;
            Menu_BrickSpaceX=0;
            Menu_BrickSpaceY+=60;
        }

        //End of creating bricks

    }



//    if(Level1_Counter==48)

    //gradient drawing





    }





        window.display();
    }

    return 0;
}
