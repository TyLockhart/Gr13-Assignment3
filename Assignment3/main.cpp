//LEARNED: ALL VECTORS NEED A PUSHBACK FIRST, also learned resize
//future: can add some health losing/point gaining iamges on menu/explosions on removed blocks

//!!work o nsetting paddle same x as ball
//!!work on creating menu bricks correctly
//!!work on spinning menu bricks
//!!work on menu text and paly  button
//!!Work on creating level1 bricks
//!!Work on colouring shapes by changing like gradient and add paddle/ball
//work on creating/moving paddle!! ball point systems
//work on destrouying bricks

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
    vector<RectangleShape> Menu_Background,Level1_Background,Bar_Background;
    vector<Vertex> Menu_Vertex;
    vector<Block> Menu_Wall,Menu_Brick,Level1_Brick,Level1_Wall;
    vector<Vector2f> Menu_BrickPoints;
    vector<Text> Menu_Text,Bar_Text;
    vector<Clock> Menu_Clock,Level1_Clock,Bar_Clock;

    Font Game_Font1;

    Color Menu_BrickColor(Color(243,156,18,255));
    Color Level1_BrickColor(Color(142,68,173,255));

    Color Level1_BRedC(Color(231,76,60,255));
    Color Level1_BOrangeC(Color(230,126,34,255));
    Color Level1_BYellowC(Color(241,196,15,255));
    Color Level1_BGreenC(Color(46,204,113,255));
    Color Level1_BBlueC(Color(52,152,219,255));
    Color Level1_BPurpleC(Color(155,89,182,255));

//Bools
    bool OV_Mouse=true;
    bool Menu=true;
    bool Menu_Once=true;
    bool Bar=false;
    bool Bar_Once=false;

    bool Menu_PaddleRight,Menu_PaddleLeft,Menu_CountDown,Menu_CountUp,Menu_DeleteBlock;
    bool Level1,Level1_Once;
    bool Level1_CountDown;
    bool Level1_CountUp;
    bool Level1_AllowCollisions;
    bool Level1_CreateBricks;
//Int
    int Menu_VxR,Menu_VxG,Menu_VxB,Menu_BrickSpaceX,Menu_BrickSpaceY;
    int Level1_FadeBricks,Level1_Counter,Level1_CounterRemind,Level1_i,Level1_BrickSpaceX,Level1_BrickSpaceY,Level1_BCR;
    int Bar_Score=0;
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
    b2World World_Level1(b2Vec2(0.0,0.1));
    Paddle *Menu_Paddle,*Level1_Paddle;
    Ball *Menu_Ball,*Level1_Ball;

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

    cout<<Mouse::getPosition(window).x<<": Mouse X-Pos"<<endl;
    cout<<Mouse::getPosition(window).y<<": Mouse Y-Pos"<<endl;
    cout<<"---"<<endl;
//    if(OV_Mouse==true) cout<<"Clicked Mouse"<<endl;

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

            Menu_VxR=142;
            Menu_VxG=68;
            Menu_VxB=173;
            Menu_BrickSpaceX=0;
            Menu_BrickSpaceY=0;

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
            Level1_CountDown=false;
            Level1_CountUp=true;
            Level1_AllowCollisions=false;
            Level1_CreateBricks=true;

            Bar=true;
            Bar_Once=true;

            Level1_Counter=0;
            Level1_CounterRemind=0;
            Level1_i=0;
            Level1_BrickSpaceX=0;
            Level1_BrickSpaceY=0;
            Level1_BCR=143;
            Level1_FadeBricks=0;

            Level1_Clock.resize(5);
            Level1_Clock[0].restart();

            //Creating rectangle shapes
            Level1_Background.push_back(RectangleShape(Vector2f(800,800)));
            Level1_Background.back().setFillColor(Color(Menu_BrickColor));
            Level1_Background.back().setPosition(Vector2f(0,0));

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

    if(Level1_Brick.size()==48) Level1_AllowCollisions=true;
    //Level1 brick background gradient
    if(Level1_AllowCollisions==true){
    for(int i=0; i<=7;i++) physics::setBlockColor(Level1_Brick[i],Level1_BRedC);//red
    for(int i=8; i<=15;i++) physics::setBlockColor(Level1_Brick[i],Level1_BOrangeC);//orange
    for(int i=16; i<=23;i++) physics::setBlockColor(Level1_Brick[i],Level1_BYellowC);//yellow
    for(int i=24; i<=31;i++) physics::setBlockColor(Level1_Brick[i],Level1_BGreenC);//green
    for(int i=32; i<=39;i++) physics::setBlockColor(Level1_Brick[i],Level1_BBlueC);//blue
    for(int i=40; i<=47;i++) physics::setBlockColor(Level1_Brick[i],Level1_BPurpleC);//purple
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
        Level1_Brick.push_back(physics::createPolygon(World_Level1,26+Level1_BrickSpaceX,65+Level1_BrickSpaceY,Menu_BrickPoints,b2_staticBody));
        physics::setBlockColor(Level1_Brick.back(),Level1_BrickColor);
        physics::setOutlineThickness(Level1_Brick.back(),3);
        physics::setBlockColor(Level1_Brick.back(),Color(0,0,0,255));

        Level1_i++;
        Level1_BrickSpaceX+=95;
        cout<<"creating bricks: "<<Level1_Brick.size()<<endl;

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
        Level1_CreateBricks=false;
        Level1_Ball->speed=350;
        //moving paddle
        if(Keyboard::isKeyPressed(Keyboard::A)==true) Level1_Paddle->setVelocity(Vector2f(-300,0));
        else if(Keyboard::isKeyPressed(Keyboard::D)==false)
        {
            Level1_Paddle->setVelocity(Vector2f(0,0));
        }
        if(Keyboard::isKeyPressed(Keyboard::D)==true) Level1_Paddle->setVelocity(Vector2f(300,0));
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
        cout<<"ball has left the box"<<endl;
        Level1_Ball->removeBody();
        Level1_Ball=new Ball(World_Level1,Level1_Paddle->getPosition().x,Level1_Paddle->getPosition().y-20,25);
        Level1_Ball->setOutlineColor(Color(149,165,166,255));
        Level1_Ball->setOutlineThickness(4);
        //remove life?
    }
    //if ball is on the bottom layer for 10s, reset pos
    if(Level1_Ball->getPosition().y>700&&Level1_Clock[2].getElapsedTime().asSeconds()>10)
    {
            Level1_Ball->removeBody();
            Level1_Ball=new Ball(World_Level1,Level1_Paddle->getPosition().x,Level1_Paddle->getPosition().y-20,25);
            Level1_Ball->setOutlineColor(Color(149,165,166,255));
            Level1_Ball->setOutlineThickness(4);
    }
    if(Level1_Ball->getPosition().y<670) Level1_Clock[2].restart();
    //end reset

if(Level1_AllowCollisions==true){
    for(int i=0;i<=7;i++){//red bricks collision
        if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
        {
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BRedC){
            Level1_Brick[i]->SetType(b2_dynamicBody);
            physics::spin(Level1_Brick[i],17);
            Level1_Clock[3].restart();
            }
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BRedC){
            physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
            }
        }
    }
    for(int i=8; i<=15;i++){//orange bricks collision
            if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
        {
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BOrangeC){
            Level1_Brick[i]->SetType(b2_dynamicBody);
            physics::spin(Level1_Brick[i],17);
            Level1_Clock[3].restart();
            }
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BOrangeC){
            physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
            }
        }
    }
    for(int i=16; i<=23;i++){//yellow bricks collision
            if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
        {
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BYellowC){
            Level1_Brick[i]->SetType(b2_dynamicBody);
            physics::spin(Level1_Brick[i],17);
            Level1_Clock[3].restart();
            }
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BYellowC){
            physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
            }
        }
    }
    for(int i=24; i<=31;i++){//green bricks collision
            if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
        {
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BGreenC){
            Level1_Brick[i]->SetType(b2_dynamicBody);
            physics::spin(Level1_Brick[i],17);
            Level1_Clock[3].restart();
            }
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BGreenC){
            physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
            }
        }
    }
    for(int i=32; i<=39;i++){//blue bricks collision
            if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
        {
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BBlueC){
            Level1_Brick[i]->SetType(b2_dynamicBody);
            physics::spin(Level1_Brick[i],17);
            Level1_Clock[3].restart();
            }
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BBlueC){
            physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
            }
        }
    }
    for(int i=40;i<=47;i++){//purple bricks collision
            if(Level1_Ball->checkCollision(Level1_Brick[i])==true)
        {
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()!=Level1_BPurpleC){
            Level1_Brick[i]->SetType(b2_dynamicBody);
            physics::spin(Level1_Brick[i],17);
            Level1_Clock[3].restart();
            }
            if(static_cast<Shape *>(Level1_Brick[i]->GetUserData())->getFillColor()==Level1_BPurpleC){
            physics::setBlockColor(Level1_Brick[i],Color(rand()%255,rand()%255,rand()%255));
            }
        }
    }





            //Deleting any dynamic bricks after 0.5s after hitting any brick
            for(int i=0;i<=47;i++){
            if(Level1_Clock[3].getElapsedTime().asSeconds()>=0.5&&physics::isDestroyed(Level1_Brick[i])!=true&&Level1_Brick[i]->GetType()==b2_dynamicBody){

//            physics::setBlockColor(Level1_Brick[i],Color(0,0,0,0));
//            physics::setBlockOutline(Level1_Brick[i],Color(0,0,0,0));
            Level1_Brick[i]->SetGravityScale(200);

//            physics::deleteBlock(World_Level1,Level1_Brick[i]);
//            Level1_Brick.erase(Level1_Brick.begin()+i);
//            Level1_Brick.push_back(Level1_Brick[0]+i);
//            Level1_Brick.back(physics::createBox(World_Level1,1,1,1,1,b2_staticBody));
            }
            if(physics::checkCollision(Level1_Brick[i],Level1_Wall[3])==true)
            {
                cout<<"DELETE"<<endl;
                Level1_Brick[i]->SetTransform(b2Vec2(-10,-10),Level1_Brick[i]->GetAngle());
                Level1_Brick[i]->SetType(b2_staticBody);
                Bar_Score++;
                Bar_Text[3].setString("+1");
                Bar_Clock[0].restart();
                Bar_Text[2].setString("POINTS: "+to_string(Bar_Score));
                Bar_Text[3].setPosition(Vector2f(Bar_Text[2].getPosition().x+90,23));
            }
            if(physics::checkCollision(Level1_Brick[i])==true&&physics::getPosition(Level1_Brick[i]).y<730&&physics::getPosition(Level1_Brick[i]).y>630)
            {
                cout<<"hit bar"<<endl;
                Level1_Brick[i]->SetTransform(b2Vec2(-10,-10),Level1_Brick[i]->GetAngle());
                Level1_Brick[i]->SetType(b2_staticBody);
                Bar_Score+=2;
                Bar_Text[3].setString("+2");
                Bar_Clock[0].restart();
                Bar_Text[2].setString("POINTS: "+to_string(Bar_Score));
                Bar_Text[3].setPosition(Vector2f(Bar_Text[2].getPosition().x+90,23));
                //working on setting ldistance from +1 to points
            }
//                Levels_MaisyChar->SetTransform(b2Vec2(0,14),Levels_MaisyChar->GetAngle());
//                physics::deleteBlock(World_Level1,Level1_Brick[i]);
//                Level1_Brick.erase(Level1_Brick.begin()+i);
//                Level1_Brick.push_back(Level1_Brick[i]+1);
//                if(Level1_Clock[4].getElapsedTime().asSeconds()>0.001){
//                Level1_FadeBricks++;
//                Level1_Clock[4].restart();
//                cout<<"hit"<<endl;
//               }
//               if(Level1_FadeBricks%5==0)
//               {
//
//               }
//            if(Level1_FadeBricks==255)
//               {
//                   physics::setBlockColor(Level1_Brick[i],Color(0,0,0,0));
//                   Level1_FadeBricks=0;
//               }
//            }
//            cout<<Level1_FadeBricks<<endl;


}
//Menu background gradient
//    if(Level1_Brick.size()==48){
//    for(int i=0; i<=7;i++) physics::setBlockColor(Level1_Brick[i],Level1_BRedC);//red
//    for(int i=8; i<=15;i++) physics::setBlockColor(Level1_Brick[i],Level1_BOrangeC);//orange
//    for(int i=16; i<=23;i++) physics::setBlockColor(Level1_Brick[i],Level1_BYellowC);//yellow
//    for(int i=24; i<=31;i++) physics::setBlockColor(Level1_Brick[i],Level1_BGreenC);//green
//    for(int i=32; i<=39;i++) physics::setBlockColor(Level1_Brick[i],Level1_BBlueC);//blue
//    for(int i=40; i<=47;i++) physics::setBlockColor(Level1_Brick[i],Level1_BPurpleC);//purple

}
    }
        Level1_Ball->updatePosition();
        Level1_Paddle->updatePosition();
        for(auto i : Level1_Background) window.draw(i);
        physics::displayWorld(World_Level1,window);

    }
    if(Bar==true)
    {
        if(Bar_Once==true)
        {
            Bar_Clock.resize(1);

           Bar_Background.push_back(RectangleShape(Vector2f(800,50)));
            Bar_Background.back().setFillColor(Color(0,0,0,180));
            Bar_Background.back().setPosition(Vector2f(0,0));
            Bar_Background.back().setOutlineThickness(4);
            Bar_Background.back().setOutlineColor(Color(255,255,255,255));

        Bar_Text.push_back(Text());
        Bar_Text.back().setCharacterSize(16);
        Bar_Text.back().setFont(Game_Font1);
        Bar_Text.back().setPosition(Vector2f(425,13));
        Bar_Text.back().setString("BRICK");
        Bar_Text.back().setFillColor(Color::White);
        Bar_Text.back().setOutlineColor(Color::Black);
        Bar_Text.back().setOutlineThickness(2);
        Bar_Text.back().setOrigin(Vector2f(Bar_Text.back().getCharacterSize()/2+40,Bar_Text.back().getCharacterSize()/2));

        Bar_Text.push_back(Text());
        Bar_Text.back().setCharacterSize(16);
        Bar_Text.back().setFont(Game_Font1);
        Bar_Text.back().setPosition(Vector2f(410,33));
        Bar_Text.back().setString("BREAKER");
        Bar_Text.back().setFillColor(Color::White);
        Bar_Text.back().setOutlineColor(Color::Black);
        Bar_Text.back().setOutlineThickness(2);
        Bar_Text.back().setOrigin(Vector2f(Bar_Text.back().getCharacterSize()/2+40,Bar_Text.back().getCharacterSize()/2));

        Bar_Text.push_back(Text());
        Bar_Text.back().setCharacterSize(16);
        Bar_Text.back().setFont(Game_Font1);
        Bar_Text.back().setPosition(Vector2f(10,23));
        Bar_Text.back().setString("POINTS: "+to_string(Bar_Score));
        Bar_Text.back().setFillColor(Color::White);
        Bar_Text.back().setOutlineColor(Color::Black);
        Bar_Text.back().setOutlineThickness(2);

        Bar_Text.push_back(Text());
        Bar_Text.back().setCharacterSize(16);
        Bar_Text.back().setFont(Game_Font1);
        Bar_Text.back().setPosition(Vector2f(Bar_Text[2].getPosition().x+90,23));
        Bar_Text.back().setString("+1");
        Bar_Text.back().setFillColor(Color(46,204,113,255));
        Bar_Text.back().setOutlineColor(Color::Black);
        Bar_Text.back().setOutlineThickness(2);

           Bar_Once=false;
        }




    for(auto i:Bar_Background) window.draw(i);
    for(int i=0; i<=2;i++) window.draw(Bar_Text[i]);
    if(Bar_Clock[0].getElapsedTime().asSeconds()<2)
    {
        window.draw(Bar_Text[3]);
    }
//    for(auto i:Bar_Text) window.draw(i);
    }





        window.display();
    }

    return 0;
}
