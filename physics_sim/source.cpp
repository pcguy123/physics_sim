#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct Entity {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f acc;
	sf::Color color;
    unsigned int width;
    unsigned int height;
};

void draw(const Entity& e, sf::RenderWindow& window)
{
    sf::RectangleShape rect(sf::Vector2f(e.width, e.height));
    rect.setPosition(e.pos);
    rect.setFillColor(e.color);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2.0f);
    window.draw(rect);
}

void drawAll(const std::vector<Entity>& v, sf::RenderWindow& window)
{
	for (int i = 0; i < v.size(); i++)
	{
		draw(v[i], window);
	}
}

void update(Entity& e, const float dt)
{
	e.vel.x += e.acc.x * dt;
	e.vel.y += e.acc.y * dt;
	e.pos.x += e.vel.x * dt;
	e.pos.y += e.vel.y * dt;
    e.color = sf::Color(0, 0, 0, 255);
}

void updateAll(std::vector<Entity>& v, const float dt)
{
	for (int i = 0; i < v.size(); i++)
	{
		update(v[i], dt);
	}
}

void checkCollision(Entity& e1, Entity& e2)
{
    if(e1.pos.x < e2.pos.x + e2.width &&
       e1.pos.x + e1.width > e2.pos.x &&
       e1.pos.y < e2.pos.y + e2.height && 
       e1.pos.y + e1.height > e2.pos.y)
    {
        e1.color = sf::Color(e1.color.r + 50, e1.color.g + 50, e1.color.b + 50, 255);
        e2.color = sf::Color(e1.color.r + 50, e1.color.g + 50, e1.color.b + 50, 255);
    }
}

void checkAllCollisions(std::vector<Entity>& v)
{
    for(int i = 0; i < v.size(); i++)
    {
        for(int j = i + 1; j < v.size(); j++)
        {
            checkCollision(v[i], v[j]);
        }
    }
}


int main(int argc, char* args[])
{
    sf::RenderWindow window(sf::VideoMode(720, 480), "Physics Sim");
    window.setPosition(sf::Vector2i(100, 100));
    // window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(60);

	std::vector<Entity> es;
	for (int i = 0; i < 5; i++)
	{
		es.push_back({ sf::Vector2f(0.0f, 300.0f), 
					   sf::Vector2f((float)(50 * i + 100), -300.f),
					   sf::Vector2f(0.0f, 200.0f),
					   sf::Color(0, 0, 0, 255),
                       50,
                       75 });
	}
	
	sf::Color bg = sf::Color::Black;
    sf::Clock clock;
	float old_time = clock.getElapsedTime().asSeconds();
    float new_time;
    float dt;
    bool quit = false;
    
	while(!quit)
	{
		new_time = clock.getElapsedTime().asSeconds();
		dt = (float)(new_time - old_time);
		old_time = new_time;
        
        // std::cout << "FPS: " << 1.0f/dt << "\n";

		sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    std::cout << "Quitting\n";
                    quit = true;
                    break;
                case sf::Event::KeyPressed:
                    std::cout << "Button Pressed - Quitting\n";
                    quit = true;
                    break;
                case sf::Event::MouseButtonPressed:
                    std::cout << "Mouse down\n";
                    break;
                default:
                    // std::cout << "No event\n";
                    break;
            }
        }
        
		window.clear(sf::Color::Green);

		updateAll(es, dt);
        checkAllCollisions(es);
		drawAll(es, window);
        
        window.display();
	}

	std::cout << "Window closing\n";

	window.close();

	return EXIT_SUCCESS;
}
