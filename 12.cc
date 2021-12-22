#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>

std::shared_ptr<SDL_Window> win;
std::shared_ptr<SDL_Renderer> ren;
std::shared_ptr<SDL_Texture>bird_tex; 	//птица
std::shared_ptr<SDL_Texture>cat_tex; 	// кот
std::shared_ptr<SDL_Texture>ground_tex; //земл€
std::shared_ptr<SDL_Texture>tree_tex; 	//дерево
std::shared_ptr<SDL_Texture>house_tex; 	//дом
std::shared_ptr<SDL_Texture>fence_tex; 	//забор



constexpr int WIDTH = 800, HEIGHT = 600;
constexpr int BW = 68, BH = 68;
constexpr int CW = 118, CH = 120;
int bx = 100, by = 70;
int cx = 400, cy = 590, cdy = 0;
bool cat_jump = false;

void load_texture(
		std::shared_ptr<SDL_Texture>&tex,
		const char *filename)
{
	tex = std::shared_ptr<SDL_Texture>(
			IMG_LoadTexture(ren.get(), filename), SDL_DestroyTexture);
	if (tex == nullptr) {
		std::cerr << "Ќе удалось загрузить текстуру: " << SDL_GetError()
				<< std::endl;
		exit(1);
	}
}

void initialise_window()
{
	win = std::shared_ptr<SDL_Window>(
		SDL_CreateWindow("SDL Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT,
		0),
		SDL_DestroyWindow);
	if (win == nullptr) {
		std::cerr << "Ќе удалось создать окно: " <<
			SDL_GetError() << std::endl;
		exit(1);
	}

	ren = std::shared_ptr<SDL_Renderer>(
		SDL_CreateRenderer(win.get(), -1,
			SDL_RENDERER_ACCELERATED |
			SDL_RENDERER_PRESENTVSYNC),
		SDL_DestroyRenderer);
	if (ren == nullptr) {
		std::cerr << "Ќе удалось создать рендерер: " <<
			SDL_GetError() << std::endl;
		exit(1);
	}


load_texture(bird_tex,"bird.png");
load_texture(cat_tex,"cat.png");
load_texture(ground_tex,"ground.png");
load_texture(tree_tex,"tree.png");
load_texture(house_tex,"house.png");
load_texture(fence_tex,"fence.png");
}

void draw_tree_lines() {
/*	SDL_RenderDrawLine(ren.get(), 150, 550, 150, 200);
	for (int i = 0; i < 10; ++i) {
	 	SDL_RenderDrawLine(ren.get(), 150, 230 + i * 30, 150 - 10 * (i + 1),
				230 + i * 30 - 20);
		SDL_RenderDrawLine(ren.get(), 150, 230 + i * 30, 150 + 10 * (i + 1),
				230 + i * 30 - 20);
	}*/
}

void draw_house_lines() {
/*	SDL_RenderDrawLine(ren.get(), 350, 250, 700, 250); //крыша
	SDL_RenderDrawLine(ren.get(), 380, 250, 380, 550);
	SDL_RenderDrawLine(ren.get(), 660, 250, 660, 550);
	SDL_RenderDrawLine(ren.get(), 500, 350, 500, 550); //дом
	SDL_RenderDrawLine(ren.get(), 600, 350, 600, 550);
	SDL_RenderDrawLine(ren.get(), 500, 350, 600, 350);
	SDL_RenderDrawLine(ren.get(), 400, 350, 450, 350);
	SDL_RenderDrawLine(ren.get(), 400, 400, 450, 400);
	SDL_RenderDrawLine(ren.get(), 400, 350, 400, 400);
	SDL_RenderDrawLine(ren.get(), 450, 350, 450, 400);
	SDL_RenderDrawLine(ren.get(), 400, 350, 450, 400);
	SDL_RenderDrawLine(ren.get(), 450, 350, 400, 400);
	SDL_RenderDrawLine(ren.get(), 350, 250, 525, 100); //решЄтка на окнах
	SDL_RenderDrawLine(ren.get(), 525, 100, 700, 250);*/
}

void draw_ground_lines() {
	// рисуем землю
	SDL_RenderDrawLine(ren.get(), 0, 550, 799, 550);
}

void main_loop()
{
	SDL_Event event;
	auto keys = SDL_GetKeyboardState(nullptr);

	for (;;) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return;
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode ==
					SDL_SCANCODE_ESCAPE)
				return;
			}
		}
	if (keys[SDL_SCANCODE_D]) bx += 2;
	if (keys[SDL_SCANCODE_A]) bx -= 2;
	if (keys[SDL_SCANCODE_D]) by -= 4;
	if (keys[SDL_SCANCODE_A]) by += 4;
	if (keys[SDL_SCANCODE_LEFT]) cx -= 6;
	if (keys[SDL_SCANCODE_RIGHT]) cx += 6;
	if (keys[SDL_SCANCODE_SPACE]) {
		if (not cat_jump) {
			cat_jump = true;
			cdy = -24;
		}
	}


		bx += 4;
		if (bx-BW/2 >= WIDTH) bx = -BW/2;
		if (by-BH/2 < 0) by = BH/2;
		if (by+BH/2 >= 550) by = 550-BH/2;
		if (cx-CW/2 < 0) cx=CW/2;
		if (cx+CW/2 >= WIDTH) cx=WIDTH-CW/2;
		if (cat_jump) {
			cy += cdy;
			cdy += 1;
			if (cy >= 550) {
				cy = 550;
				cdy = 0;
				cat_jump = false;
			}
		}

		SDL_SetRenderDrawColor(ren.get(), 31, 63, 127, 255);
		SDL_RenderClear(ren.get());

		//SDL_SetRenderDrawColor(ren.get(), 255, 255, 255, 255);
	    // рисуем землю
		draw_ground_lines();
		// рисуем дерево
		draw_tree_lines();
		// дом

		 draw_house_lines();

		SDL_Rect bird_rect {bx-BW/2, by-BH/2, BW, BH};
		SDL_RenderCopy(ren.get(),bird_tex.get(),
				nullptr,&bird_rect);


		SDL_Rect tree_rect {500, 230, 300,350};
		SDL_RenderCopy(ren.get(),tree_tex.get(),
						nullptr,&tree_rect);

		SDL_Rect fence_rect {530, 500, 270,100};
		SDL_RenderCopy(ren.get(),fence_tex.get(),
			  	nullptr,&fence_rect);

SDL_Rect house_rect {50,250, 500,350};
		SDL_RenderCopy(ren.get(),house_tex.get(),
				nullptr,&house_rect);

		SDL_Rect cat_rect {cx-CW/2,cy-CH,CW,CH};
			SDL_RenderCopy(ren.get(), cat_tex.get(),
					nullptr,&cat_rect);



		SDL_Rect ground_rect {0, 550, 800,70};
				SDL_RenderCopy(ren.get(),ground_tex.get(),
						nullptr,&ground_rect);

		SDL_RenderPresent(ren.get());
	}
}

int main(int, char **)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	initialise_window();
	main_loop();

	return 0;
}
