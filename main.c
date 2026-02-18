#include "main.h"

int main(int argc, char **argv){

    struct HackComputer computer;

    if(argc < 2){
        initComputer(&computer, "b.out");
    }
    else{
        initComputer(&computer, argv[1]);
    }
    //computerLoop(&computer, 20);

    SDL_Window *window = initDisplay();
    SDL_Renderer *rend = initRender(window);

    SDL_Texture *texture = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    uint32_t *pixels = malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
    parsePixels(&computer, pixels);

    displayLoop(window, rend, texture, pixels, &computer);

    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);

    SDL_Quit();

}


uint8_t scancodeToAscii(SDL_KeyboardEvent *key) {
    SDL_Scancode sc = key->keysym.scancode;
    bool shift = (key->keysym.mod & KMOD_SHIFT);

    // Letters
    if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z) {
        return shift ? ('A' + (sc - SDL_SCANCODE_A))
                     : ('a' + (sc - SDL_SCANCODE_A));
    }

    // Numbers
    if (sc >= SDL_SCANCODE_1 && sc <= SDL_SCANCODE_9) {
        static const char shifted[] = {'!', '@', '#', '$', '%', '^', '&', '*', '('};
        return shift ? shifted[sc - SDL_SCANCODE_1]
                     : ('1' + (sc - SDL_SCANCODE_1));
    }

    if (sc == SDL_SCANCODE_0)
        return shift ? ')' : '0';

    // Space
    if (sc == SDL_SCANCODE_SPACE) return 32;

    // Punctuation
    switch (sc) {
        case SDL_SCANCODE_RETURN: return 10;
        case SDL_SCANCODE_BACKSPACE: return 8;
        case SDL_SCANCODE_TAB: return 9;

        case SDL_SCANCODE_MINUS:        return shift ? '_' : '-';
        case SDL_SCANCODE_EQUALS:       return shift ? '+' : '=';
        case SDL_SCANCODE_LEFTBRACKET:  return shift ? '{' : '[';
        case SDL_SCANCODE_RIGHTBRACKET: return shift ? '}' : ']';
        case SDL_SCANCODE_BACKSLASH:    return shift ? '|' : '\\';
        case SDL_SCANCODE_SEMICOLON:    return shift ? ':' : ';';
        case SDL_SCANCODE_APOSTROPHE:   return shift ? '"' : '\'';
        case SDL_SCANCODE_GRAVE:        return shift ? '~' : '`';
        case SDL_SCANCODE_COMMA:        return shift ? '<' : ',';
        case SDL_SCANCODE_PERIOD:       return shift ? '>' : '.';
        case SDL_SCANCODE_SLASH:        return shift ? '?' : '/';
    }

    return 0; // unsupported key
}



void displayLoop(SDL_Window *window, SDL_Renderer *rend, SDL_Texture *texture, uint32_t *pixels, struct HackComputer *computer){
    SDL_Event event;
    uint8_t running = 1;
    int fps = 60;
    // hz here is cycles per minute
    int cpu_hz = 1000000;
    //int cpu_hz = 100000;
    int cycles_per_frame = cpu_hz / fps;
    uint32_t frame_time = 1000 / fps;
    while (running){
        uint32_t frame_start = SDL_GetTicks();

        //Run cycle_per_frame steps of the cpu
        computerLoop(computer, cycles_per_frame);
        while (SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_QUIT:
					running = 0;
					break;

                case SDL_KEYDOWN:
                    uint8_t ascii = scancodeToAscii(&event.key);
                    computer->ram[0x6000] = ascii; 
                    break;

                case SDL_KEYUP:
                    computer->ram[0x6000] = 0;
                    break;
            }
        }

        printCPU(&(computer->cpu));
        parsePixels(computer, pixels);
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, texture, NULL, NULL);
        SDL_RenderPresent(rend);

        uint32_t elapsed = SDL_GetTicks() - frame_start;
        if(elapsed < frame_time){
            SDL_Delay(frame_time - elapsed);
        }

    }
}


void parsePixels(struct HackComputer *computer, uint32_t *pixels){

    uint32_t *p = pixels;

    for(int i = 0x4000; i < 0x6000; i++){
        //each 16 bit value holds 16 pixels of information
        for(int j = 0; j < 16; j++){
            uint8_t is_pixel = ((computer->ram[i] >> j) & 1);
            *p = -(uint16_t)is_pixel;
            p++;
        }
    }
}


SDL_Window *initDisplay(){
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 0;
	}
	/* Create a window */
	SDL_Window* wind = SDL_CreateWindow("Hack Computer",
				      SDL_WINDOWPOS_CENTERED,
				      SDL_WINDOWPOS_CENTERED,
				      WIDTH, HEIGHT, 0);
	if(!wind){
		printf("Error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	return wind;
}


SDL_Renderer *initRender(SDL_Window *wind){
	/* Create a renderer */
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* rend = SDL_CreateRenderer(wind, -1, render_flags);
	if (!rend){
		printf("Error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(wind);
		SDL_Quit();
		return 0;
	}

	return rend;
}
