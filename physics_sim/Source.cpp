#include <SDL.h>
#include <iostream>
#include <vector>

struct Vec2 {
	float x;
	float y;
    
	inline Vec2 operator+ (Vec2& v) const {
		return { this->x + v.x, this->y + v.y };
	};

	inline Vec2 operator+= (Vec2& v) {
		this->x += v.x;
		this->y += v.y;
	};
};

struct Color {
	int r;
	int g;
	int b;
	int a;
};

struct Entity {
	Vec2 pos;
	Vec2 vel;
	Vec2 acc;
	Color color;
};

void draw(const Entity& e, SDL_Renderer* renderer)
{
	SDL_Rect rect = { (int)(e.pos.x), (int)(e.pos.y), 50, 50 };

	SDL_SetRenderDrawColor(renderer, e.color.r, e.color.g, e.color.b, e.color.a);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect);
}

void drawAll(const std::vector<Entity>& v, SDL_Renderer* renderer)
{
	for (int i = 0; i < v.size(); i++)
	{
		draw(v[i], renderer);
	}
}

void update(Entity& e, const float dt)
{
	e.vel.x += e.acc.x * dt;
	e.vel.y += e.acc.y * dt;
	e.pos.x += e.vel.x * dt;
	e.pos.y += e.vel.y * dt;
    e.color = {0, 0, 0, 255};
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
    if(e1.pos.x < e2.pos.x + 50 && e1.pos.x + 50 > e2.pos.x && e1.pos.y < e2.pos.y + 50 && e1.pos.y + 50 > e2.pos.y)
    {
        e1.color = {e1.color.r + 50, e1.color.g + 50, e1.color.b + 50, 255};
        e2.color = {e2.color.r + 50, e2.color.g + 50, e2.color.b + 50, 255};
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
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL did not initialize :(" << std::endl << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	
	SDL_Window* window = SDL_CreateWindow("Physics Sim", 100, 100, 720, 480, 0);

	if (window == NULL)
	{
		std::cout << "Window was not created" << std::endl << SDL_GetError() << std::endl;
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL)
	{
		std::cout << "Renderer was not created" << std::endl << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	std::vector<Entity> es;
	for (int i = 0; i < 5; i++)
	{
		es.push_back({ { 0.0f, (float)(300) }, 
					   { (float)(50 * i + 100), -300.f },
					   { 0.0f, 200.0f },
					   { 0, 0, 0, 255 } });
	}
	
	Color bg = { 0, 75, 0, 255 };
	SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
	long old_time = SDL_GetTicks();
    bool quit = false;
    
	while(!quit)
	{
		long new_time = SDL_GetTicks();
		float dt = (float)(new_time - old_time) / 1000;
		old_time = new_time;
		// std::cout << "ticks: " << SDL_GetTicks() << "\tdt: ~" << dt << "\tFPS: ~" << 1/dt << std::endl;

		SDL_RenderClear(renderer);

		updateAll(es, dt);
        checkAllCollisions(es);
		drawAll(es, renderer);

		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
        
		SDL_Event event;
        while(SDL_PollEvent(&event)){  /* Loop until there are no events left on the queue */
            switch(event.type) {  /* Process the appropiate event type */
                case SDL_KEYDOWN:  /* Handle a KEYDOWN event */
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        printf("Quitting\n");
                        quit = true;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printf("Oh! Mouse down\n");
                    break;
                default: /* Report an unhandled event */
                    printf("I don't know what this event is!\n");
            }
        }
	}

	std::cout << "Window closing" << std::endl;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return EXIT_SUCCESS;
}
