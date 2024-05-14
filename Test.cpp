#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <random>
#include <vector>
struct Vector2 {
public:
  float x;
  float y;
  Vector2(float X, float Y) {
    x = X;
    y = Y;
  }
  Vector2 operator+(Vector2 const &obj) {
    Vector2 newVec(0, 0);
    newVec.x = x + obj.x;
    newVec.y = y + obj.y;
    return newVec;
  }
  Vector2 operator-(Vector2 const &obj) {
    Vector2 newVec(0, 0);
    newVec.x = x - obj.x;
    newVec.y = y - obj.y;
    return newVec;
  }
  Vector2 operator*(float Amt) {
    Vector2 newVec(0, 0);
    newVec.x = x * Amt;
    newVec.y = y * Amt;
    return newVec;
  }
  Vector2 operator/(float Amt) {
    Vector2 newVec(0, 0);
    newVec.x = x / Amt;
    newVec.y = y / Amt;
    return newVec;
  }
  float Distance(Vector2 Other) {
    return std::abs(Other.x-x) + std::abs(Other.y - y);
  }
};
class Ball {
public:
  Vector2 Position = Vector2(0, 0);
  Vector2 Velocity = Vector2(0, 0);
  sf::Color MyColor = sf::Color(255, 255, 255);
  float Size = 4;
  bool HasGravity;
  Ball(float X, float Y) { Position = Vector2(X, Y); }
};
const int colorOffset = 20;
int rand255(){
      std::random_device r;
 
    // Choose a random mean between 1 and 6
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0+colorOffset, 255-colorOffset);
     int mean = uniform_dist(e1);
     return mean;
}
int main(int argc, char *argv[]) {
    //multiple same shade
    //bounce in 3d
    //bounce = glow and fade
  const float avoidFactor = 0.05;
  const float matchFact = 0.03;
  const float centerFact = 0.01;
  
   
  int Iter = 0;
  std::vector<Ball> Balls;
   sf::Color baseColor;
   baseColor.r = rand255();
   baseColor.g = rand255();
   baseColor.b = rand255();
  sf::RenderWindow window(sf::VideoMode(200, 200), "Funny Balls",sf::Style::Fullscreen);
 
  for (int i = 0; i < 400; i++) {
       

    Ball newBall = Ball((float)rand() / RAND_MAX * window.getSize().x,(float)rand() / RAND_MAX * window.getSize().y);
    // newBall.Velocity =
    // Vector2(((float)rand()/RAND_MAX-(float)rand()/RAND_MAX),((float)rand()/RAND_MAX-(float)rand()/RAND_MAX));
    newBall.MyColor = sf::Color(baseColor.r+(((((float)rand() / RAND_MAX)*2)-1) * colorOffset),baseColor.g+(((((float)rand() / RAND_MAX)*2)-1) * colorOffset),baseColor.b+(((((float)rand() / RAND_MAX)*2)-1) * colorOffset));
    //newBall.MyColor = sf::Color::Blue;
    newBall.Size = (float)rand() / RAND_MAX * 9 + 8;
    newBall.HasGravity = false;
    Balls.emplace_back(newBall);
  }
    sf::Clock clock;
  while (window.isOpen()) {
    // std::cout << Balls[0].Velocity.x;
    sf::Time deltaTime = clock.restart();
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    // Iter++;
    // if(Iter >=10){
    window.clear(sf::Color::White);
    // Iter = 0;
    //}
    if (strcmp(argv[1], "Screen") == 0) {
      for (auto &ball : Balls) {
        sf::CircleShape shape(ball.Size);

        shape.setFillColor(ball.MyColor);
        shape.setPosition(ball.Position.x, ball.Position.y);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          ball.Velocity =ball.Velocity +(Vector2((sf::Mouse::getPosition(window).x - ball.Position.x) /400,((sf::Mouse::getPosition(window).y - ball.Position.y) /400))) *-1;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
          ball.Velocity =
              ball.Velocity +
              (Vector2((sf::Mouse::getPosition(window).x - ball.Position.x) /
                           400,
                       ((sf::Mouse::getPosition(window).y - ball.Position.y) /
                        400)));
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
          ball.Velocity = ball.Velocity / 1.1f;
        }
        if (ball.Position.x + ball.Velocity.x / 100 < 0 ||
            ball.Position.x + ball.Velocity.x / 100 > window.getSize().x) {
          ball.Velocity.x *= -1;
        }
        if (ball.Position.y + ball.Velocity.y / 100 < 0 ||
            ball.Position.y + ball.Velocity.y / 100 > window.getSize().y) {
          // invert x if wall, y if ground
          ball.Velocity.y *= -1;
        }

        if (ball.HasGravity) {
          ball.Velocity.y += 5;
        }

        ball.Position = ball.Position + ball.Velocity;
        window.draw(shape);
      }
    }
    if (strcmp(argv[1], "Boids") == 0) {
      for (auto &ball : Balls) {

        std::vector<Ball> NearBirds;
        float closedx = 0;
        float closedy = 0;
        float avgXvel = 0;
        float avgYvel = 0;
        float avgYpos;
        float avgXpos;
        int TotVis = 0;
        for (auto &ball2 : Balls) {

          if (ball.Position.Distance(ball2.Position) < 15) {
            closedx += (ball.Position.x - ball2.Position.x) * 40;
            closedy += (ball.Position.y - ball2.Position.y) * 40;
          }
          if (ball.Position.Distance(ball2.Position) < 150) {
            avgXvel += ball2.Velocity.x;
            avgYvel += ball2.Velocity.y;
            avgXpos += ball2.Position.x;
            avgYpos += ball2.Position.y;
            TotVis += 1;
          }
        }
        avgXvel /= TotVis;
        avgYvel /= TotVis;
        avgXpos /= TotVis;
        avgYpos /= TotVis;

        ball.Velocity = ball.Velocity +
                        Vector2((avgXvel - ball.Velocity.x) * matchFact +
                                    (avgXpos - ball.Position.x) * centerFact,
                                (avgYvel - ball.Velocity.y) * matchFact +
                                    (avgYpos - ball.Position.y) * centerFact);
        ball.Velocity = ball.Velocity +
                        Vector2(closedx * avoidFactor, closedy * avoidFactor);
        // ball.Velocity = ball.Velocity + Vector2((, );
        if (ball.Position.x < 20) {
          ball.Velocity.x += 100;
        }
        if (ball.Position.x > window.getSize().x - 20) {
          ball.Velocity.x -= 100;
        }
        if (ball.Position.y < 20) {
          ball.Velocity.y += 100;
        }
        if (ball.Position.y > window.getSize().y - 20) {
          ball.Velocity.y -= 100;
        }
        float speed = sqrt(ball.Velocity.x * ball.Velocity.x +
                           ball.Velocity.y * ball.Velocity.y);
        if (speed > 100) {
          ball.Velocity = Vector2((ball.Velocity.x / speed) * 100,
                                  (ball.Velocity.y / speed) * 100);
        }
        if (speed < 4) {
          ball.Velocity = Vector2((ball.Velocity.x / speed) * 4,
                                  (ball.Velocity.y / speed) * 4);
        }
        ball.Velocity.x += ((float)rand()/RAND_MAX)*2-1;
        ball.Velocity.y += ((float)rand()/RAND_MAX)*2-1;
        ball.Position = ball.Position + (ball.Velocity / 50);
        sf::CircleShape shape(ball.Size);
        shape.setFillColor(ball.MyColor);
        shape.setPosition(ball.Position.x, ball.Position.y);
        window.draw(shape);
      }
    }

    window.display();
  }

  return 0;
}