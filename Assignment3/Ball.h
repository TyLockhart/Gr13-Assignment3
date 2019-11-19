#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "physics.h"

using namespace sf;
using namespace std;

struct Ball : public CircleShape {
    Ball(b2World &world, float x, float y, float r) {
        b2BodyDef bodyDef;
        bodyDef.position.Set(x/pixels_per_meter, y/pixels_per_meter);
        bodyDef.type = b2_dynamicBody;
        bodyDef.linearDamping = 0.0;
        b2CircleShape b2shape;

        b2shape.m_radius = r/pixels_per_meter;

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0;
        fixtureDef.friction = 0.0;
        fixtureDef.restitution = 1.00;
        fixtureDef.shape = &b2shape;

        res = world.CreateBody(&bodyDef);
        res->CreateFixture(&fixtureDef);

        this->setRadius(r);
        this->setOrigin(r,r);
        this->setPosition(x,y);
        this->setFillColor(Color::White);

        res->SetUserData(this);

        speed = 1650; //1650 & 350
        angle = (-45.0)/deg_per_rad;
        res->SetLinearVelocity(b2Vec2(speed*cos(angle)/pixels_per_meter,speed*sin(angle)/pixels_per_meter));

    }

    //member functions
    bool checkCollision()
    {
        for (b2ContactEdge* edge = res->GetContactList(); edge; edge = edge->next) {
            if (edge->contact->IsTouching()) return true;
        }
        return false;
    }

    bool checkCollision(Block b)
    {
        for (b2ContactEdge* edge = res->GetContactList(); edge; edge = edge->next) {
            if (edge->other == b) {
                if (edge->contact->IsTouching()) {
                   return true;
                }
            }
        }
        return false;
    }

    bool checkCollision(Block b1, Block b2) {
        for (b2ContactEdge* edge = b1->GetContactList(); edge; edge = edge->next) {
            if (edge->other == b2) {
                if (edge->contact->IsTouching()) {
                   return true;
                }
            }
        }
        return false;
    }

    void setVelocity(sf::Vector2f vel) {
        res->SetLinearVelocity(b2Vec2(vel.x/pixels_per_meter,vel.y/pixels_per_meter));
    }

    void setVelocity(float speed, float angle) {
        res->SetLinearVelocity(b2Vec2(speed*cos(angle)/pixels_per_meter,speed*sin(angle)/pixels_per_meter));
    }

    void updatePosition()
    {
        b2Vec2 velocity = res->GetLinearVelocity();
        res->SetLinearVelocity((speed/pixels_per_meter)/velocity.Length()*velocity);
        this->setPosition(res->GetPosition().x*pixels_per_meter, res->GetPosition().y*pixels_per_meter);
        this->setRotation(res->GetAngle()*deg_per_rad);
    }

    void removeBody()
    {
        res->GetWorld()->DestroyBody(res);
        res->SetUserData(0);
    }


    //member variable
    b2Body* res;
    float speed;
    float angle;

};
