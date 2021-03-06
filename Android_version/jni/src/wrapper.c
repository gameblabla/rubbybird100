/*
   COPYRIGHT (C) 2014-2016 GAMEBLABLA
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#define FPS_VIDEO 60
#define MAX_IMAGE 256

/*	SDL2 port for PC platforms. Recommended port for Windows, Mac, GNU/Linux and BSDs  */

#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <SDL_mixer.h>
#include <time.h>
#include "INPUT.h"

#define UNIX
#define MAX_SFX 32
Mix_Music* music;
Mix_Chunk* gfx_id[MAX_SFX];

struct input BUTTON;
struct tt touch;

const float real_FPS = 1000/FPS_VIDEO;

unsigned short done = 0;
char* game_name = "";

SDL_Window* game_screen;
SDL_Renderer* renderer;
SDL_Texture * backBuffer;

SDL_GameController *pad;
SDL_Joystick *joy;
short instanceID;

SDL_Texture *sprites_img[MAX_IMAGE];
unsigned short sprites_w_[MAX_IMAGE];
unsigned short sprites_h_[MAX_IMAGE];
unsigned char resize;

float ScaleW, ScaleH, resScaleW, resScaleH;

#define Buttons_UP SDL_SCANCODE_UP
#define Buttons_LEFT SDL_SCANCODE_LEFT
#define Buttons_RIGHT SDL_SCANCODE_RIGHT
#define Buttons_DOWN SDL_SCANCODE_DOWN
#define Buttons_A SDL_SCANCODE_X
#define Buttons_B SDL_SCANCODE_C
#define Buttons_C SDL_SCANCODE_V
#define Buttons_D SDL_SCANCODE_B
#define Buttons_START SDL_SCANCODE_SPACE
#define Buttons_SELECT SDL_SCANCODE_RETURN
#define Buttons_QUIT SDL_SCANCODE_ESCAPE

#define Joypad_A SDL_CONTROLLER_BUTTON_A 
#define Joypad_B SDL_CONTROLLER_BUTTON_B
#define Joypad_C SDL_CONTROLLER_BUTTON_X
#define Joypad_D SDL_CONTROLLER_BUTTON_Y
#define Joypad_START SDL_CONTROLLER_BUTTON_START 
#define Joypad_SELECT SDL_CONTROLLER_BUTTON_BACK 


void Clearing(unsigned char error);

void msleep(unsigned char milisec)
{
	struct timespec req={0};
	time_t sec=(unsigned short)(milisec/1000);

	milisec=milisec-(sec*1000);
	req.tv_sec=sec;
	req.tv_nsec=milisec*1000000L;

	while(nanosleep(&req,&req)==-1)
	continue;
}

void Init_video()
{
	//Set texture filtering to neighbor
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );

	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER );

	game_screen = SDL_CreateWindow(game_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);  
	
	/* Create renderer, its resolution, set to HARDWARE aceleration and Vsync turned on */
	renderer = SDL_CreateRenderer(game_screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//SDL_RenderSetLogicalSize(renderer, 0, 0);
	SDL_RenderSetLogicalSize(renderer, 320, 480);

	/* Hide the mouse and grab it */
	SDL_SetRelativeMouseMode(SDL_FALSE);
	
	resize = 1;
}

void Load_Image(const unsigned short a, const char* directory, const unsigned char transparency)
{
	SDL_Surface* tmp;
	char tmp_directory[128];
	snprintf(tmp_directory, sizeof(tmp_directory), "%s", directory);
	
	if (sprites_img[a] != NULL)
	{
		SDL_DestroyTexture(sprites_img[a]);
	}
	
	#ifdef DEBUG
		fprintf(stderr, "Trying to load image %d (%s)...\n", a, directory);
	#endif

	tmp = SDL_LoadBMP(tmp_directory);
	
	if (!tmp)
	{
		SDL_Log("ERROR!!! CAN'T FIND IMAGE %s!!!, %s\n", directory, SDL_GetError());
		Clearing(1);
		return;
	}
	
	sprites_w_[a] = tmp->w;
	sprites_h_[a] = tmp->h;
	
	if (transparency == 1)
	{
		SDL_SetColorKey(tmp, 1, SDL_MapRGB(tmp->format, 255, 0, 255));
	}
	SDL_SetSurfaceRLE(tmp, 1);

	sprites_img[a] = SDL_CreateTextureFromSurface(renderer, tmp);
	
	SDL_FreeSurface(tmp);
}

void Put_image(unsigned short a, short x, short y, unsigned char scale)
{
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put image %d on screen and update its position\n X: %d \n Y: %d\n", a, x ,y);
	#endif	
	
	SDL_Rect position;
	position.x = x;
	position.y = y;
	position.w = (sprites_w_[a]*scale);
	position.h = (sprites_h_[a]*scale);
	
	SDL_RenderCopy(renderer, sprites_img[a], NULL, &position);
}

void Put_image_alpha (unsigned short a, unsigned char alpha)
{
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put image %d on screen and update its position\n X: %d \n Y: %d\n", a, x ,y);
	#endif	
	
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	position.w = sprites_w_[a];
	position.h = sprites_h_[a];
	
	SDL_SetTextureAlphaMod(sprites_img[a], alpha);
	SDL_RenderCopy(renderer, sprites_img[a], NULL, &position);
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f, float scale)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;
	position.w = (w*scale);
	position.h = (h*scale);
	
	SDL_Rect frame;
	frame.x = f*w;
	frame.y = 0;
	frame.w = w;
	frame.h = h;
	
	SDL_RenderCopy(renderer, sprites_img[a], &frame, &position);
}

void Put_sprite_darken(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f, float scale, short c)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;
	position.w = (w*scale);
	position.h = (h*scale);
	
	SDL_Rect frame;
	frame.x = f*w;
	frame.y = 0;
	frame.w = w;
	frame.h = h;
	
	SDL_SetTextureColorMod(sprites_img[a], c-255, c-255, c-255);
	SDL_RenderCopy(renderer, sprites_img[a], &frame, &position);
}

void UnDarken(unsigned char a)
{
	SDL_SetTextureColorMod(sprites_img[a], 255, 255, 255);
}

void Clear_screen()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

SDL_Rect newWindowSize;
SDL_Rect nativeSize;

void Resize()
{
	float w, h;

    nativeSize.x = 0;
    nativeSize.y = 0;
    nativeSize.w = 320;
    nativeSize.h = 480;
	
	SDL_GetWindowSize(game_screen, &w, &h);
    
    ScaleW = ((float)w / (float)nativeSize.w);
    ScaleH = ((float)h / (float)nativeSize.h);
    resScaleW = ( (float)nativeSize.w / (float)w);
    resScaleH = ( (float)nativeSize.h / (float)h);

    newWindowSize.w = w;
    newWindowSize.h = h;

    SDL_DestroyTexture(backBuffer);
    backBuffer = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(game_screen), 
                                   SDL_TEXTUREACCESS_TARGET, //Again, must be created using this
                                   nativeSize.w, 
                                   nativeSize.h);
	
    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer, &viewPort);

    if(viewPort.w != newWindowSize.w || viewPort.h != newWindowSize.h)
    {
        SDL_RenderSetViewport(renderer, &newWindowSize);
    }
    
    resize = 0;
}

void Update_video()
{  
	/* Unused, might be useful on 144Hz panels though ? */
	/*Uint32 start;
	start = SDL_GetTicks();*/
	
	#ifndef KEEP_ASPECT_RATIO
	SDL_SetRenderTarget(renderer, NULL);
	
	if (resize) Resize();
	#endif

	#ifndef KEEP_ASPECT_RATIO
	SDL_RenderCopy(renderer, backBuffer, &nativeSize, &newWindowSize); //Render the backBuffer onto the screen at (0,0) 
	#endif
	SDL_RenderPresent(renderer);
	#ifndef KEEP_ASPECT_RATIO
    SDL_SetRenderTarget(renderer, backBuffer);
    #endif
    /* Unused, might be useful on 144Hz panels though ? */
	//if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));
}

void Controls()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	const short x_joy = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX);
	const short y_joy = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY);

	BUTTON.UP = state[Buttons_UP] ? 1 : 0;
	BUTTON.DOWN = state[Buttons_DOWN] ? 1 : 0;
	BUTTON.LEFT = state[Buttons_LEFT] ? 1 : 0;
	BUTTON.RIGHT = state[Buttons_RIGHT] ? 1 : 0;
		
	BUTTON.A = (state[Buttons_A] || SDL_GameControllerGetButton(pad, Joypad_A)) ? 1 : 0;
	BUTTON.B = (state[Buttons_B] || SDL_GameControllerGetButton(pad, Joypad_B)) ? 1 : 0;
	BUTTON.C = (state[Buttons_C] || SDL_GameControllerGetButton(pad, Joypad_C)) ? 1 : 0;
	BUTTON.D = (state[Buttons_D] || SDL_GameControllerGetButton(pad, Joypad_D)) ? 1 : 0;
	BUTTON.START = (state[Buttons_START] || SDL_GameControllerGetButton(pad, Joypad_START)) ? 1 : 0;
	BUTTON.SELECT = (state[Buttons_SELECT] || SDL_GameControllerGetButton(pad, Joypad_SELECT)) ? 1 : 0;
	BUTTON.QUIT = state[Buttons_QUIT] ? 1 : 0;

        if (y_joy < -6000)
        {
			BUTTON.UP = 1;
        }
		else if (y_joy > 6000)
        {
			BUTTON.DOWN = 1;
        }

        if (x_joy < -6000)
        {
			BUTTON.LEFT = 1;
        }
        else if (x_joy > 6000)
        {
			BUTTON.RIGHT = 1;
        }
        
        touch.state = 0;
        touch.inmotion = 0;
		
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    done = 1;
                    break;
                }  
                
				case SDL_CONTROLLERDEVICEADDED:
					if( SDL_IsGameController( 0 ) ) 
					{
						pad = SDL_GameControllerOpen( 0 );
						if (pad) 
						{
							joy = SDL_GameControllerGetJoystick( pad );
							instanceID = SDL_JoystickInstanceID( joy );
						}
					}
				break;
				#ifdef SELFISHOS
				case SDL_FINGERDOWN:
					touch.x = (event.tfinger.x)*(resScaleW);
					touch.y = (event.tfinger.y)*(resScaleH);
					touch.state = 1;
				break;
				case SDL_FINGERMOTION:
					touch.x = (event.tfinger.x)*(resScaleW);
					touch.y = (event.tfinger.y)*(resScaleH);
					touch.inmotion = 1;
				break;
				case SDL_FINGERUP:
					touch.x = (event.tfinger.x)*(resScaleW);
					touch.y = (event.tfinger.y)*(resScaleH);
					touch.state = 2;
				break;
				#else
				case SDL_FINGERDOWN:
					touch.x = (event.tfinger.x * nativeSize.w);
					touch.y = (event.tfinger.y * nativeSize.h);
					touch.state = 1;
				break;
				case SDL_FINGERMOTION:
					touch.x = (event.tfinger.x * nativeSize.w);
					touch.y = (event.tfinger.y * nativeSize.h);
					touch.inmotion = 1;
				break;
				case SDL_FINGERUP:
					touch.x = (event.tfinger.x * nativeSize.w);
					touch.y = (event.tfinger.y * nativeSize.h);
					touch.state = 2;
				break;
				case SDL_MOUSEBUTTONDOWN:
					touch.x = event.motion.x*(resScaleW);
					touch.y = event.motion.y*(resScaleH);
					touch.state = 1;
				break;
				case SDL_MOUSEMOTION:
					touch.x = event.motion.x*(resScaleW);
					touch.y = event.motion.y*(resScaleH);
					touch.inmotion = 1;
				break;
				case SDL_MOUSEBUTTONUP:
					touch.x = event.motion.x*(resScaleW);
					touch.y = event.motion.y*(resScaleH);
					touch.state = 2;
				break;
				#endif
            }
        }
		
}

void Clear_Images()
{
	short i;
	
	for (i=0;i<MAX_IMAGE;i++) 
	{
		if (sprites_img[i] != NULL)
		{
			SDL_DestroyTexture(sprites_img[i]);
		}
	}
}

void Init_sound()
{
	#ifdef DEBUG
		fprintf(stderr, "Init sound system\n");
	#endif
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024);
	Mix_AllocateChannels(MAX_SFX);
}
	
void Clean_Music()
{
	#ifdef DEBUG
		fprintf(stderr, "Stop and clear music from memory\n");
	#endif
			
	if (music)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
}

void Load_Music(const char* directory)
{
	#ifdef DEBUG
		fprintf(stderr, "Load music in memory\n");
	#endif
	music = Mix_LoadMUS(directory);
	if(!music)
	{
		SDL_Log("ERROR !!! Music not found !");
	}
}

void Play_Music(int loop)
{
	if (loop == 1)
	{
		#ifdef DEBUG
			fprintf(stderr, "Playing music in loop\n");
		#endif
		Mix_PlayMusic(music, -1);
	}
	else
	{
		#ifdef DEBUG
			fprintf(stderr, "Playing music\n");
		#endif
		Mix_PlayMusic(music, 0);
	}
}

void Stop_Music()
{
	Mix_HaltMusic();
}

void Load_SFX(int i, char* directory)
{
	char tmp_directory[128];
	#ifdef SELFISHOS
	snprintf(tmp_directory, sizeof(tmp_directory), "/usr/share/killminds/%s", directory);
	#else
	snprintf(tmp_directory, sizeof(tmp_directory), "%s", directory);
	#endif
	
	#ifdef DEBUG
		fprintf(stderr, "Load sound effect %d (%s) in memory\n", i, directory);
	#endif
			
	if (gfx_id[i])
	{
		Mix_FreeChunk(gfx_id[i]);
		gfx_id[i] = NULL;
	}
	gfx_id[i] = Mix_LoadWAV(tmp_directory);
	if (!gfx_id[i])
	{
		SDL_Log("ERROR!!! CAN'T FIND SOUND %s!!!\n", tmp_directory);
		Clearing(1);
	}
}

void Play_SFX(int i)
{
	#ifdef DEBUG
		fprintf(stderr, "Play sound effect %d loaded in memory\n", i);
	#endif
	Mix_PlayChannel(-1, gfx_id[i], 0) ;
}

void Unload_SFX()
{
	unsigned short i;	
	#ifdef DEBUG
		fprintf(stderr, "Free sound effect from memory\n");
	#endif
			
	for (i=0;i<MAX_SFX;i++) 
	{
		if (gfx_id[i])
		{
			Mix_FreeChunk(gfx_id[i]);
			gfx_id[i] = NULL;
		}
	}
}

void Clearing(unsigned char error)
{	
	/*Clean_Music();*/

	#ifdef DEBUG
		fprintf(stderr, "Clean surface and free SDL2 from memory\n");
	#endif
	
	Clear_Images();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(game_screen);
	
	Unload_SFX();
	Mix_CloseAudio();
	
    SDL_Quit();
    exit(error);
}
