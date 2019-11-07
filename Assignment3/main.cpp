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


//Bools
    bool Menu=true;
    bool Menu_Once=true;



    RenderWindow window(VideoMode(800,600),"Assignment3: Brick Breaker");
    window.setFramerateLimit(60);
    b2World World_Menu(b2Vec2(0.0,9.8));
    Paddle *Menu_Paddle;


    while (window.isOpen()&& !Keyboard::isKeyPressed(Keyboard::Escape))
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear();

    if(Menu==true)
    {
        if(Menu_Once==true)
        {
            //Paddle Menu_Paddle(World_Menu,50,50,50,15);
            Menu_Paddle = new Paddle(World_Menu, 50,50,50,15);



            Menu_Once=false;
        }

    if(Keyboard::isKeyPressed(Keyboard::A)==true)
    {
        cout<<"test"<<endl;
    }



        physics::displayWorld(World_Menu,window);
        window.draw(*Menu_Paddle);
        Menu_Paddle->updatePosition();

    }





        window.display();
    }

    return 0;
}
