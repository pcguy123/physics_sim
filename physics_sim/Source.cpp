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
}

void updateAll(std::vector<Entity>& v, const float dt)
{
	for (int i = 0; i < v.size(); i++)
	{
		update(v[i], dt);
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
		es.push_back({ { 0.0f, (float)(i * 60 + 100) }, 
					   { (float)(50 * i + 100), -200.f },
					   { 0.0f, 200.0f },
					   { rand() % 255, rand() % 255, rand() % 255, 255 } });
	}
	
	Color bg = { 0, 75, 0, 255 };
	SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
	long old_time = SDL_GetTicks();

	while(SDL_GetTicks() < 3000)
	{
		long new_time = SDL_GetTicks();
		float dt = (float)(new_time - old_time) / 1000;
		old_time = new_time;
		std::cout << "ticks: " << SDL_GetTicks() << "\tdt: ~" << dt << "\tFPS: ~" << 1/dt << std::endl;

		SDL_RenderClear(renderer);

		updateAll(es, dt);
		drawAll(es, renderer);

		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
	}

	std::cout << "Window closing" << std::endl;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return EXIT_SUCCESS;
}
