#include <SDL.h>
#include <iostream>
#include <vector>

struct Vec2 {
	float x;
	float y;
    
	inline struct Vec2 operator+ (struct Vec2& v) const {
		return { this->x + v.x, this->y + v.y };
	};

	inline struct Vec2 operator+= (struct Vec2& v) {
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
	Vec2 pos; // position vector
	Vec2 vel; // velocity vector
	Vec2 acc; // acceleration vector
	Color color;
};

// draw an entity
void draw(struct Entity* e, SDL_Renderer* renderer)
{
	SDL_Rect rect = { (int)(e->pos.x), (int)(e->pos.y), 50, 50 };

	// Set rect color to entity color
	SDL_SetRenderDrawColor(renderer, e->color.r, e->color.g, e->color.b, e->color.a);
	// Render rect
	SDL_RenderFillRect(renderer, &rect);
	// Set rect color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// Render rect outline
	SDL_RenderDrawRect(renderer, &rect);
}

// update all entities in a vector
void drawAll(struct std::vector<struct Entity>* v, SDL_Renderer* renderer)
{
	for (int i = 0; i < v->size(); i++)
	{
		draw(&((*v)[i]), renderer);
	}
}

// update an entity
void update(struct Entity* e, float dt)
{
	e->vel.x += e->acc.x * dt;
	e->vel.y += e->acc.y * dt;
	e->pos.x += e->vel.x * dt;
	e->pos.y += e->vel.y * dt;
}

// update all entities in a vector
void updateAll(struct std::vector<struct Entity>* v, float dt)
{
	for (int i = 0; i < v->size(); i++)
	{
		update(&((*v)[i]), dt);
	}
}


int main(int argc, char* args[])
{
	// try to init SDL, exit program if it doesn't work
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL did not initialize :(" << std::endl << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	
	// create window
	SDL_Window* window = SDL_CreateWindow("Physics Sim", 100, 100, 720, 480, 0);

	if (window == NULL)
	{
		std::cout << "Window was not created" << std::endl << SDL_GetError() << std::endl;
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// create the renderer
	// @Note: maybe add flag SDL_RENDERER_ACCELERATED or other flags
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL)
	{
		std::cout << "Renderer was not created" << std::endl << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// create list of entities
	std::vector<struct Entity> es;
	for (int i = 0; i < 5; i++)
	{
		es.push_back({ { 0.0f, (float)(i * 60 + 100) }, 
					   { (float)(50 * i + 100), -200.f },
					   { 0.0f, 200.0f },
					   { rand() % 255, rand() % 255, rand() % 255, 255 } });
	}
	
	struct Color bg = { 0, 75, 0, 255 };
	SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
	long old_time = SDL_GetTicks();

	// program loop - runs for 3 seconds
	while(SDL_GetTicks() < 3000)
	{
		// calculate delta time
		long new_time = SDL_GetTicks();
		float dt = (float)(new_time - old_time) / 1000;
		old_time = new_time;
		std::cout << "ticks: " << SDL_GetTicks() << "\tdt: ~" << dt << "\tFPS: ~" << 1/dt << std::endl;

		// Clear the screen to black
		SDL_RenderClear(renderer);

		/// update entities
		updateAll(&es, dt);
		// draw entities
		drawAll(&es, renderer);

		// Render everything to the screen
		SDL_RenderPresent(renderer);
		// Reset clear color
		SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
	}

	std::cout << "Window closing" << std::endl;

	// finishd running - delete stuff
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return EXIT_SUCCESS;
}