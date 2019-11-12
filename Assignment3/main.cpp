//!!work o nsetting paddle same x as ball
//work on creating menu bricks correctly

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
    vector<RectangleShape> Menu_Background;
    vector<Vertex> Menu_Vertex;
    vector<Block> Menu_Wall,Menu_Brick;
    vector<Vector2f> Menu_BrickPoints;

    Clock Menu_Clock;

    Color Menu_BrickColor(Color(243,156,18,255));

//Bools
    bool Menu=true;
    bool Menu_Once=true;
    bool Menu_PaddleRight=false;
    bool Menu_PaddleLeft=false;
    bool Menu_CountDown=false;
    bool Menu_CountUp=false;

//Int
    int Menu_VxR=142;
    int Menu_VxG=68;
    int Menu_VxB=173;
    int Menu_BrickSpaceX=0;
    int Menu_BrickSpaceY=0;

//Points
    Menu_BrickPoints.push_back(Vector2f(0,0));
    Menu_BrickPoints.push_back(Vector2f(40,6));
    Menu_BrickPoints.push_back(Vector2f(80,0));
    Menu_BrickPoints.push_back(Vector2f(80,40));
    Menu_BrickPoints.push_back(Vector2f(40,34));
    Menu_BrickPoints.push_back(Vector2f(0,40));
    Menu_BrickPoints.push_back(Vector2f(0,0));

    RenderWindow window(VideoMode(800,800),"Assignment3: Brick Breaker");
    window.setFramerateLimit(60);
    b2World World_Menu(b2Vec2(0.0,0.0));
    Paddle *Menu_Paddle;
    Ball *Menu_Ball;


    while(window.isOpen()&& !Keyboard::isKeyPressed(Keyboard::Escape))
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
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

            for(int i=0; i<Menu_Wall.size(); i++){
            if(physics::isDestroyed(Menu_Wall[i])!=true)
            {
                physics::deleteBlock(World_Menu,Menu_Wall[i]);
            }
            }

            Menu_Paddle=new Paddle(World_Menu,400,700,150,25);
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
        for(int i=0; i<=8; i++)
        {
//        Menu_Brick.push_back(physics::createBox(World_Menu,25+Menu_BrickSpaceX,25+Menu_BrickSpaceY,65,35,b2_staticBody));
//        physics::setBlockColor(Menu_Brick.back(),Menu_BrickColor);
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

            Menu_Once=false;
        }
        //Menu background gradient
            Menu_Vertex.clear();
            Menu_Vertex.push_back(Vertex(Vector2f(0,0),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(800,0),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(800,800),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(0,800),Color(Menu_VxR,Menu_VxG,Menu_VxB)));
            Menu_Vertex.push_back(Vertex(Vector2f(0,0),Color(Menu_VxR,Menu_VxG,Menu_VxB)));

        if(Menu_CountDown==true&&Menu_Clock.getElapsedTime().asSeconds()>=0.00001)
        {
            Menu_VxR--;
            Menu_VxG--;
            Menu_VxB--;
            Menu_Clock.restart();
        }
        if(Menu_CountUp==true&&Menu_Clock.getElapsedTime().asSeconds()>=0.00001)
        {
            Menu_VxR++;
            Menu_VxG++;
            Menu_VxB++;
            Menu_Clock.restart();
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
        if(physics::checkCollision(Menu_Brick[i])==true)
        {



            if(physics::isDestroyed(Menu_Brick[i])!=true){
            physics::deleteBlock(World_Menu,Menu_Brick[i]);
            Menu_Brick.erase(Menu_Brick.begin()+i);
            }
        }
        }








    //Paddle following ball
    if(Menu_Ball->getPosition().x>Menu_Paddle->getPosition().x&&Menu_Ball->getPosition().y<700)
    {
        Menu_Paddle->setVelocity(Vector2f(280,0));
    }
    else if(Menu_Ball->getPosition().x<Menu_Paddle->getPosition().x&&Menu_Ball->getPosition().y<700)
    {
        Menu_Paddle->setVelocity(Vector2f(-280,0));
    }
    //End paddle following ball




        window.draw(&Menu_Vertex[0],Menu_Vertex.size(),TrianglesFan);//Background gradient
        for(auto i:Menu_Background) window.draw(i);
//        for(auto i:Menu_Brick) window.draw(i);
        physics::displayWorld(World_Menu,window);
//        window.draw(*Menu_Paddle);
        Menu_Paddle->updatePosition();
//        window.draw(*Menu_Ball);
        Menu_Ball->updatePosition();

    }





        window.display();
    }

    return 0;
}
