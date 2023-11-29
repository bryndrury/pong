#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

#include "vec3.h"

void render_number(SDL_Renderer* renderer, int num, int x, int y);
void check_collision(vec3& pos, vec3& vel, vec3& left_paddle, vec3 right_paddle);
void update_ball(vec3& pos, vec3& vel, int screen_width, int screen_height, vec3& left_paddle, vec3& right_paddle);

int right_score;
int left_score;
bool pause = false;


int main(int argc, char** argv) 
{
	float screen_width = 858;
	float screen_height = 525;
	SDL_Init( SDL_INIT_VIDEO );

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	window = SDL_CreateWindow("pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, (SDL_WINDOW_SHOWN, SDL_WINDOW_BORDERLESS ,SDL_WINDOW_OPENGL));
	if ( window == nullptr ) std::cerr << "Failed to apply video mode : " << SDL_GetError();
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED, SDL_RENDERER_PRESENTVSYNC));
	if ( renderer == nullptr ) std::cerr << "Could not create renderer!";

	// Render Black Screen and initialize colors
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
	SDL_RenderClear( renderer );

	// Initializing main loop
	SDL_Event event;
    Uint32 starting_tick;
	bool running = true;

    // game objects
    vec3 ball_pos(260, 260, 0);
    vec3 ball_vel(5, 5, 0);

    vec3 left_paddle(10, (int) screen_height/2 - 50, 0);
    vec3 right_paddle((int) screen_width-20, (int) screen_height/2 - 50, 0);

    while (running) 
    {
        starting_tick = SDL_GetTicks();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) 
                    {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_SPACE:
                            pause = !pause;
                            break;
                        case SDLK_UP:
                            if (left_paddle.y() > 0) left_paddle.setY(left_paddle.y() - 15);
                            break;
                        case SDLK_DOWN:
                            if (left_paddle.y() < screen_height - 100) left_paddle.setY(left_paddle.y() + 15);
                        default:
                            break;
                    }
                    break;
            }
        }

        // background
        for (int i = 0; i < screen_height; i+=(screen_height/20))
        {
            SDL_Rect rect{int(screen_width/2)-5, i, 10, 20};
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        // move opponent paddle
        if (ball_pos.y() > right_paddle.y() + 50)
        {
            right_paddle.setY(right_paddle.y() + 4);
        }
        else if (ball_pos.y() < right_paddle.y() + 50)
        {
            right_paddle.setY(right_paddle.y() - 4);
        }

        if (pause==false) update_ball(ball_pos, ball_vel, screen_width, screen_height, left_paddle, right_paddle);

        SDL_Rect ball{(int)ball_pos.x(), (int)ball_pos.y(), 10, 10};
        SDL_Rect left{(int)left_paddle.x(), (int)left_paddle.y(), 10, 100};
        SDL_Rect right{(int)right_paddle.x(), (int)right_paddle.y(), 10, 100};

        render_number(renderer, right_score, screen_width/2 + 30, 20);
        render_number(renderer, left_score, screen_width/2 - 60, 20);

        std::vector<SDL_Rect> rects{ball, left, right};
        for (auto& rect : rects)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);

        // if (1000/240 > SDL_GetTicks() - starting_tick) SDL_Delay(1000/240 - (SDL_GetTicks() - starting_tick));
        // std::cout << 1000/(SDL_GetTicks() - starting_tick) << std::endl;
    }
}

void check_collision(vec3& pos, vec3& vel, vec3& left_paddle, vec3 right_paddle)
{
    if (pos.x() < left_paddle.x() + 10 && pos.x() > left_paddle.x() && pos.y() < left_paddle.y() + 100 && pos.y() > left_paddle.y())
    {
        vel.setX(-vel.x());
    }
    if (pos.x() < right_paddle.x() + 10 && pos.x() > right_paddle.x() - 10 && pos.y() < right_paddle.y() + 100 && pos.y() > right_paddle.y())
    {
        vel.setX(-vel.x());
    }
}

void update_ball(vec3& pos, vec3& vel, int screen_width, int screen_height, vec3& left_paddle, vec3& right_paddle)
{
    pos = pos + vel;
    if (pos.x() < 0 || pos.x() > screen_width-10)
    {
        vel.setX(-vel.x());
    }
    if (pos.y() < 0 || pos.y() > screen_height-10)
    {
        vel.setY(-vel.y());
    }
    check_collision(pos, vel, left_paddle, right_paddle);
    if (pos.x() < 10)
    {
        right_score++;
        pos.setValues(screen_width/2, screen_height/2, 0);
        vel.setValues(5, 5, 0);
    }
    if (pos.x() > screen_width - 10)
    {
        left_score++;
        pos.setValues(screen_width/2, screen_height/2, 0);
        vel.setValues(-5, -5, 0);
    }
}

void render_number(SDL_Renderer* renderer, int num, int x, int y)
{
    std::vector<SDL_Rect> zero{
        {x,y,30,10},
        {x,y+10,10,30},
        {x+20,y+10,10,30},
        {x,y+40,30,10},
    };
    std::vector<SDL_Rect> one{
        {x+20,y,10,50},
    };
    std::vector<SDL_Rect> two{
        {x,y,30,10},
        {x+20,y+10,10,10},
        {x,y+20,30,10},
        {x,y+30,10,10},
        {x,y+40,30,10},
    };
    std::vector<SDL_Rect> three{
        {x,y,30,10},
        {x+20,y+10,10,10},
        {x,y+20,30,10},
        {x+20,y+30,10,10},
        {x,y+40,30,10},
    };
    std::vector<SDL_Rect> win{
        // W
        {x-10,y+70,10,40},
        {x+10,y+70,10,40},
        {x+30,y+70,10,40},
        {x,y+110,10,10},
        {x+20,y+110,10,10},

        // I
        {x+10,y+130,10,40},
        {x,y+130,30,10},
        {x,y+170,30,10},

        // N
        {x-10,y+190,10,40},
        {x,y+190,10,10},
        {x+10,y+200,10,10},
        {x+20,y+210,10,10},
        {x+30,y+190,10,40},

        // N
        {x-10,y+240,10,40},
        {x,y+240,10,10},
        {x+10,y+250,10,10},
        {x+20,y+260,10,10},
        {x+30,y+240,10,40},

        // E
        {x-10,y+290,10,40},
        {x-10,y+290,50,10},
        {x-10,y+310,50,10},
        {x-10,y+330,50,10},

        // R
        {x-10,y+350,10,50},
        {x-10,y+350,40,10},
        {x-10,y+370,40,10},
        {x+30,y+360,10,10},
        {x+30,y+380,10,10},
        {x+30,y+390,10,10},

    };
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 100);
    for (auto& rect : zero)
    {
        SDL_RenderFillRect(renderer, &rect);
    }
    if (num > 3) 
    {
        pause = true;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    for (auto& rect : win)
    {
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    if (num == 1)
    {
        for (auto& rect : one)
        {
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    else if (num == 2)
    {
        for (auto& rect : two)
        {
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    else if (num == 3)
    {
        for (auto& rect : three)
        {
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

 