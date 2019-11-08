//work o nsetting paddle same x as ball


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
    vector<Block> Menu_Wall;

    Clock Menu_Clock;
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



            Menu_Once=false;
        }

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






        if(Menu_Paddle->getPosition().x<=0)
        {
            Menu_PaddleRight=false;
            Menu_PaddleLeft=true;
        }
        if(Menu_Paddle->getPosition().x>=800)
        {
            Menu_PaddleLeft=false;
            Menu_PaddleRight=true;
        }
        if(Menu_PaddleLeft==true) Menu_Paddle->setVelocity(Vector2f(60,0));
        if(Menu_PaddleRight==true) Menu_Paddle->setVelocity(Vector2f(-60,0));

    if(Keyboard::isKeyPressed(Keyboard::A)==true)
    {
        cout<<"test"<<endl;
    }

        window.draw(&Menu_Vertex[0],Menu_Vertex.size(),TrianglesFan);
        for(auto i:Menu_Background) window.draw(i);
        physics::displayWorld(World_Menu,window);
        window.draw(*Menu_Paddle);
        Menu_Paddle->updatePosition();

    }





        window.display();
    }

    return 0;
}
