#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>
struct Vector2{
    public:
    float x;
    float y;
    Vector2(float X, float Y){
        x=X;
        y=Y;
    }
   Vector2 operator+(Vector2 const& obj){
    Vector2 newVec(0,0);
    newVec.x = x+obj.x;
    newVec.y = y+obj.y;
    return  newVec;
   }
   Vector2 operator*(float Amt){
      Vector2 newVec(0,0);
      newVec.x = x*Amt;
      newVec.y = y*Amt;
      return newVec;

   }
};
class Ball{
    public:
  Vector2 Position=Vector2(0,0);
  Vector2 Velocity = Vector2(0,0);  
  sf::Color MyColor=sf::Color(255,255,255);
  float Size = 4;
  Ball(float X, float Y){
    Position = Vector2(X,Y);
  }
};
int main()
{
    std::vector<Ball> Balls;
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!", sf::Style::Fullscreen);
    for (int i=0; i<1000; i++) {
           Ball newBall = Ball(500,500);
           //Ball((float)rand()/RAND_MAX*window.getSize().x,(float)rand()/RAND_MAX*window.getSize().y);
            newBall.Velocity = Vector2((float)rand()/RAND_MAX-(float)rand()/RAND_MAX,(float)rand()/RAND_MAX-(float)rand()/RAND_MAX-(float)rand()/RAND_MAX);
            newBall.MyColor =sf::Color((float)rand()/RAND_MAX*255,(float)rand()/RAND_MAX*255,(float)rand()/RAND_MAX*255);
            newBall.Size = (float)rand()/RAND_MAX*14;
            Balls.emplace_back(newBall);
            
    }
 

  
 

    while (window.isOpen())
    {
        //std::cout << Balls[0].Velocity.x;
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for (auto &ball : Balls) {
            sf::CircleShape shape(ball.Size);
   
            
            shape.setFillColor(ball.MyColor);
            shape.setPosition(ball.Position.x, ball.Position.y);
            ball.Position = ball.Position+ball.Velocity;
            if(ball.Position.x <0||ball.Position.x>window.getSize().x)
            {
                ball.Velocity.x *= -1;
            }
            if(ball.Position.y<0||ball.Position.y>window.getSize().y){
                //invert x if wall, y if ground
            ball.Velocity.y *= -1;
            
            }
            window.draw(shape);
            
        }
        
       
        window.display();
    }

    return 0;
}