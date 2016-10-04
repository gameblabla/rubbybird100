/*
	Copyright (C) 2016 Gameblabla
	Licensed under the GAMEBLABLA's VIDEO GAME PUBLIC LICENSE version 1.0.
	See COPYING file for more details.
	If a COPYING file was not provided, you can ask the copyright holder for a copy at :
	<gameblabla@openmailbox.org>
		
	This Program is distributed without WITHOUT ANY WARRANTY; 
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
	See the GAMEBLABLA's VIDEO GAME PUBLIC LICENSE for more details.
*/

/*
 * Rubby Bird 100 : the sequel to my Rubby Bird game.
 * This one changes the formula a bit : You have to rub 100 birds instead of 1 !
 * Now THAT is a lot more difficult...
*/

#include "API.h"
#include "INPUT.h"
#include "main.h"
#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#ifndef ANDROID
#include <sys/wait.h>
#include <sys/types.h>
#else
#include <SDL.h>
#endif

/*
 * GLOBALS 
 * All variables are declared here
*/

unsigned char game_mode = 0;
short logo_y;

const unsigned char diffilculty_birds_required[10] = {1, 3, 5, 8, 10, 15, 20, 30, 40, 42};
const unsigned char diffilculty_remove_life[10] = {1, 1, 1, 1, 2, 3, 4, 4, 5, 8};
const unsigned char diffilculty_give_life[10] = {10, 10, 10, 10, 8, 8, 8, 6, 5, 4};

unsigned char birds_dead;
unsigned char birds_limit;
unsigned short birds_limit_time;

struct birdies 
{
	short x;
	short y;
	unsigned char status;
	short life;
	unsigned char time;
	unsigned char time2;
	unsigned char speed;
	unsigned char direction;
	unsigned char direction2;
	float scale;
} birds[128];

unsigned char bird_title_frame;
unsigned char bird_title_time;

#define HIGHSCORE 1
#define NORMAL 0

unsigned char hour[2];
unsigned char minutes[2];
unsigned char seconds[2];
unsigned char microseconds;

/*
 * 
 * Main loop
 * 
*/

int main(int argc, char* argv[])
{
	InitializeGame();
	srand(time(NULL));

	while (!done)
	{
		Controls();
		GameLoop();
		Update_video();
	}

	Clearing(0);

	return 0;
}

/*
 * 
 * This function is used only at startup
 * It loads all the resources needed for the game, plays the "music" (birds crying),
 * and it will also load the highscore.
 * 
*/
void InitializeGame()
{
	unsigned char i;
	Init_video();
	Init_sound();
	
	Load_Music("data/rubby.ogg");
	Play_Music(1);
	
	Load_Image(1,"data/fond.bmp",0);
	Load_Image(2,"data/bird.bmp",1);
	Load_Image(3,"data/fond2.bmp",0);
	Load_Image(4,"data/logo.bmp",1);
	Load_Image(5,"data/score.bmp",1);
	Load_Image(6,"data/go.bmp",1);
	Load_Image(7,"data/ddot.bmp",1);
	Load_Image(8,"data/bird.bmp",1);
	Load_Image(9,"data/limit_score.bmp",1);
	Load_Image(10,"data/inst.bmp",1);
	Load_Image(11,"data/tap.bmp",1);
	Load_Image(12,"data/tweet.bmp",1);
	Load_Image(13,"data/copyright.bmp",1);
	
	Load_SFX(1, "data/dies.wav");
	Load_SFX(2, "data/tick.wav");
	Load_SFX(3, "data/alarm.wav");
	
	Load_Highscore();
	
	logo_y = -60;
	Init_birds();
}


/*
 * 
 * Game loop
 * Screens are handled using the game_mode variable.
 * Quite simple really.
 * 
*/
void GameLoop()
{
	if (BUTTON.QUIT) done = 1;
	
	switch(game_mode)
	{
		case TITLESCREEN:
			Titlescreen();
		break;
		
		case INSTRUCTIONS_INGAME:
			Instructions_screen();
		break;

		case INGAME:
			thegame_ingame();
		break;
		
		case RESULTS:
			results_screen();
		break;
	}
}

/*
 * Titlescreen.
 * Nuff said.
 * 
*/
void Titlescreen()
{
	static unsigned char titlescreen_state = 0;
	Put_image(1,0,0,1);
	Draw_birds_title();
	Put_image(4,24,logo_y,1);
	Draw_time(390, HIGHSCORE);
	Put_image(13,32,400,1);
	
	switch(titlescreen_state)
	{
		case 0:	
			if (logo_y < 30) logo_y+=2;
			
			if (touch.inmotion == 1 && touch.state > 0)
			{
				titlescreen_state = 1;
			}
		break;
		case 1:
			if (logo_y > -60) 
				logo_y-=2;
			else 
			{
				game_mode = INSTRUCTIONS_INGAME;
				birds_limit = 0;
				titlescreen_state = 0;
			}
		break;
	}

}

/*
 * Screen that shows you what you're supposed to do ingame.
 * Tapping the screen will start the game. 
 * 
*/
void Instructions_screen()
{
	static unsigned char appear_time;
	Put_image(1,0,0,1);
	Draw_birds_title();
	Put_image(10,24,92,1);
	
	if (appear_time < 30)
	Put_image(11,70,300,1);
	else if (appear_time > 60) appear_time = 0;
	appear_time++;
	
	if (touch.state > 0)
	{
		game_mode = INGAME;
	}
}

/*
 * The REAL game.
 * Does all sort of crazy stuff too.
 * 
*/
void thegame_ingame()
{
	Put_image(1,0,0,1);
	Draw_birds_ingame();
	Draw_scorebirds_onscreen();
	Draw_time(0, NORMAL);
	
	birds_limit_time++;
	if (birds_limit_time > 300)
	{
		Play_SFX(2);
		if (birds_limit < 10) birds_limit++;
		birds_limit_time = 0;
	}
	
	microseconds++;
	if (microseconds > 59)
	{
		seconds[NORMAL]++;
		microseconds = 0;
		if (seconds[NORMAL] > 59)
		{
			minutes[NORMAL]++;
			seconds[NORMAL] = 0;
			if (minutes[NORMAL] > 59)
			{
				hour[NORMAL]++;
				minutes[NORMAL] = 0;
			}
		}
	}
}

/*
 * The results screen, when you die.
 * You can either decide to share your highscore (via Twitter)
 * or go back to the titlescreen.
 * 
*/
void results_screen()
{
	static short gameover_x = - 272;
	Put_image(1,0,0,1);
	Draw_birds_ingame();
	Put_image(6,gameover_x,202,1);
	Put_image(12,gameover_x+48,320,1);
	if (gameover_x < 24) 
	{
		gameover_x+=4;
	}
	else
	{
		Draw_time(200, NORMAL);	
		if (touch.state > 0)
		{
			if (touch.x > 81 && touch.x < 248 && touch.y > 293 && touch.y < 362)
			{
				share_url();
			}
			else if (touch.x > 69 && touch.x < 264 && touch.y > 380 && touch.y < 444)
			{
				gameover_x = 0;
				toTitlescreen();
			}
		}
	}
}

/*
 * Tells the game to back to the Titlescreen.
 * It also makes sure everything is set to their default value.
 * (Or else you gonna have a bad time) 
 * 
*/

inline void toTitlescreen()
{
	game_mode = TITLESCREEN;
	UnDarken(2);
	logo_y = -60;

	birds_dead = 0;
	birds_limit = 0;
	birds_limit_time = 0;
	
	/*
	 * Checks if the user beat the current highscore. 
	 * If so, it will also make sure to save it.
	*/
	if ((hour[NORMAL] > hour[HIGHSCORE]) || (minutes[NORMAL] > minutes[HIGHSCORE]) || seconds[NORMAL] > seconds[HIGHSCORE])
	{
		hour[HIGHSCORE] = hour[NORMAL];
		minutes[HIGHSCORE] = minutes[NORMAL];
		seconds[HIGHSCORE] = seconds[NORMAL];
		Save_Highscore();
	}
	
	hour[NORMAL] = 0;
	minutes[NORMAL] = 0;
	seconds[NORMAL] = 0;
	Init_birds();
}

/*
 * This function handles collisions.
 * It is quite simple : it only checks if it is inside the collision box.
 * No color detection or some shit like that.
 * 
*/

int Collision(unsigned short x, unsigned short y, unsigned char w, unsigned char h, unsigned short x2, unsigned short y2, unsigned char w2, unsigned char h2)
{
	if ((x + w > x2) && (x < x2 + w2) && (y + h > y2) && (y < y2 + h2))
	{
		return 1;
	}	
	return 0;
}

/*
 * 
 * This draws the HUD mess in-game.
 * Also kills all birds and determine when they go lower the limit (birds_limit)
 * 
*/

void Draw_scorebirds_onscreen()
{
	unsigned char bird_numb[3], alive, bird_tokeep[2], vulnereable = 0;
	static unsigned char vulnereable_time = 0;
	static unsigned char alert_time = 0;
	unsigned char go = 0;
	alive = (100-birds_dead);
	bird_numb[0] = (alive / 100) % 10;
	bird_numb[1] = (alive / 10) % 10;
	bird_numb[2] = (alive) % 10;
	bird_tokeep[0] = (diffilculty_birds_required[birds_limit] / 10) % 10;
	bird_tokeep[1] = (diffilculty_birds_required[birds_limit]) % 10;
	
	if (alive < (diffilculty_birds_required[birds_limit]))
	{
		go = Kill_all_birds();
	}
	else if (alive < (diffilculty_birds_required[birds_limit]+15))
	{
		vulnereable = 10;
		vulnereable_time++;
	}
	
	alert_time++;
	if (alert_time > 45)
	{
		if (vulnereable == 10) Play_SFX(3);
		alert_time = 0;
	}
	
	Put_sprite(8, 95, 36, 18, 24, bird_title_frame, 1);	
	Put_sprite(9, 210, 40, 10, 10, bird_tokeep[0], 2);	
	Put_sprite(9, 227, 40, 10, 10, bird_tokeep[1], 2);	
	switch( (vulnereable_time < 15 ? 1 : 0) + (vulnereable_time > 30 ? 2 : 0) )
	{
		case 1:
			Put_sprite(5, 120, 30, 10, 10, bird_numb[0]+vulnereable, 3);	
			Put_sprite(5, 150, 30, 10, 10, bird_numb[1]+vulnereable, 3);	
			Put_sprite(5, 180, 30, 10, 10, bird_numb[2]+vulnereable, 3);	
		break;
		case 2:
			vulnereable_time = 0;
		break;
	}
	
	if (go == 1)
	{
		Stop_Music();
		game_mode = RESULTS;
	}
}

/*
 * 
 * Draws the current time.
 * mode can take two parameter : HIGHSCORE and NORMAL.
 * HIGHSCORE means it will show your current highscore.
 * NORMAL will simply show your current score
 * 
*/


void Draw_time(unsigned short y, unsigned char mode)
{
	unsigned char minutes_toshow[2], sec_toshow[2], hour_toshow[2];
	sec_toshow[0] = (seconds[mode] / 10) % 10;
	sec_toshow[1] = (seconds[mode]) % 10;
	minutes_toshow[0] = (minutes[mode] / 10) % 10;
	minutes_toshow[1] = (minutes[mode]) % 10;
	hour_toshow[0] = (hour[mode] / 10) % 10;
	hour_toshow[1] = (hour[mode]) % 10;
	
	Put_sprite(5, 95, 70+y, 10, 10, hour_toshow[0], 2);	
	Put_sprite(5, 114, 70+y, 10, 10, hour_toshow[1], 2);	
	Put_image(7,132,70+y,2);
	Put_sprite(5, 145, 70+y, 10, 10, minutes_toshow[0], 2);	
	Put_sprite(5, 164, 70+y, 10, 10, minutes_toshow[1], 2);	
	Put_image(7,183,70+y,2);
	Put_sprite(5, 195, 70+y, 10, 10, sec_toshow[0], 2);	
	Put_sprite(5, 215, 70+y, 10, 10, sec_toshow[1], 2);	
}

/*
 * 
 * Set all the birds to their defaults
 * 
*/

void Init_birds()
{
	unsigned char i;
	
	for(i=0;i<100;i++)
	{
		birds[i].x = rand_a_b(0, 320-36);
		birds[i].y = rand_a_b(150, 480-48);
		birds[i].time = 0;
		birds[i].time2 = 0;
		birds[i].speed = 0;
		birds[i].direction = rand_a_b(0, 2);
		birds[i].direction2 = rand_a_b(0, 2);
		birds[i].scale = 2.0f;
		birds[i].status = 0;
		birds[i].life = rand_a_b(248, 255);
	}
}

/*
 * 
 * This is used to animate the bird. (swap frames)
 * 
*/


inline void animate_bird()
{
	bird_title_time++;
	if (bird_title_time > 8)
	{
		bird_title_frame++;
		if (bird_title_frame > 3) bird_title_frame = 0;
		bird_title_time = 0;
	}
}

/*
 * 
 * Kill all the birds on-screen. (Set their status to dead)
 * This only happens when you don't have enough birds alive. (as set by birds_limit)
 * 
*/

unsigned char Kill_all_birds()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		birds[i].status = 1;
	}
	return 1;
}

/*
 * 
 * Draw the 100 birds on-screen.
 * The reason why i decided to keep them seperated is for performence reasons.
 * And also because Draw_birds_ingame is a huge mess...
 * 
*/

void Draw_birds_title()
{
	unsigned char i;
	animate_bird();
	for(i=0;i<100;i++)
	{
		Put_sprite(2, birds[i].x, birds[i].y, 18, 24, bird_title_frame, birds[i].scale);
		birds[i].time++;
		birds[i].time2++;
					
		/*
		* direction and direction2 is used to know
		* whetever the bird should go up or down
		*/
					
		if (birds[i].direction == 0)
		{
			if (birds[i].time > birds[i].speed) 
			{
				birds[i].x--;
				birds[i].time = 0;
			}
			if (birds[i].x < WIDTH_LIMIT_MIN) birds[i].direction = 1;
		}
		else
		{
			if (birds[i].time > birds[i].speed) 
			{
				birds[i].x++;
				birds[i].time = 0;
			}
			if (birds[i].x > WIDTH_LIMIT_MAX) birds[i].direction = 0;
		}
		if (birds[i].direction2 == 0)
		{
			if (birds[i].time2 > birds[i].speed) 
			{
				birds[i].y--;
				birds[i].time2 = 0;
			}
			if (birds[i].y < HEIGHT_LIMIT_MIN) birds[i].direction2 = 1;
		}
		else
		{
			if (birds[i].time2 > birds[i].speed) 
			{
				birds[i].y++;
				birds[i].time2 = 0;
			}
			if (birds[i].y > HEIGHT_LIMIT_MAX) birds[i].direction2 = 0;
		}
	}
}

void Draw_birds_ingame()
{
	unsigned char i;
	struct ttt
	{
		unsigned short x;
		unsigned short y;	
		unsigned short w;
		unsigned short h;	
	} realtouch;

	if (touch.inmotion)
	{
		realtouch.x = touch.x - 72;
		realtouch.y = touch.y - 72;
		realtouch.w = 148;
		realtouch.h = 148;
	}
	
	birds_dead = 0;
	
	animate_bird();
	
	for(i=0;i<100;i++)
	{
		switch(birds[i].status)
		{
			case 0:
				Put_sprite_darken(2, birds[i].x, birds[i].y, 18, 24, bird_title_frame, birds[i].scale, birds[i].life);

				birds[i].time++;
				birds[i].time2++;
					
				/*
				* Drain the bird's life if not touched
				*/
				if (birds[i].speed == 0)
				{
					birds[i].life-=	diffilculty_remove_life[birds_limit];
				}
					
				/*
				* direction and direction2 is used to know
				* whetever the bird should go up or down
				*/
					
				if (birds[i].direction == 0)
				{
					if (birds[i].time > birds[i].speed) 
					{
						birds[i].x--;
						birds[i].time = 0;
					}
					if (birds[i].x < WIDTH_LIMIT_MIN) birds[i].direction = 1;
				}
				else
				{
					if (birds[i].time > birds[i].speed) 
					{
						birds[i].x++;
						birds[i].time = 0;
					}
					if (birds[i].x > WIDTH_LIMIT_MAX) birds[i].direction = 0;
				}
				if (birds[i].direction2 == 0)
				{
					if (birds[i].time2 > birds[i].speed) 
					{
						birds[i].y--;
						birds[i].time2 = 0;
					}
					if (birds[i].y < HEIGHT_LIMIT_MIN) birds[i].direction2 = 1;
				}
				else
				{
					if (birds[i].time2 > birds[i].speed) 
					{
						birds[i].y++;
						birds[i].time2 = 0;
					}
					if (birds[i].y > HEIGHT_LIMIT_MAX) birds[i].direction2 = 0;
				}

				/*
				* If bird is touched then slow him down and give it some life
				*/
				if (Collision(birds[i].x, birds[i].y, 18, 24, realtouch.x, realtouch.y, realtouch.w, realtouch.h))
				{
					birds[i].speed = 2;
					if (birds[i].life < 255-diffilculty_give_life[birds_limit]) birds[i].life+=diffilculty_give_life[birds_limit];
				}
				else
				{
					birds[i].speed = 0;
				}
					
				/*
				 * if Health down to 0 then bird dies
				*/
				if (birds[i].life < 0) 
				{
					Play_SFX(1);
					birds[i].status = 1;
				}
			break;
			case 1:
				Put_sprite(2, birds[i].x, birds[i].y, 18, 24, bird_title_frame, birds[i].scale);
				birds[i].scale -= 0.05f;
				if (birds[i].scale < 0.1f)
				{
					birds[i].status = 2;
				}
			break;
			case 2:
				birds_dead++;
			break;
		}
	}
}

/*
 * Generate number between a and b 
 * 
*/

short rand_a_b(short a, short b)
{
    return rand()%(b-a) +a;
}

/*
 * 
 * Misc stuff : Share URL for Android/iOS, Save/Load Highscore...
 * 
*/

/*
 *  SDL_OpenURL from libSDL project
 *  https://bugzilla.libsdl.org/show_bug.cgi?id=2783
 *  My license doesn't cover this code.
*/
#ifndef ANDROID
static int SDL_OpenURL(const char *url)
{
    pid_t pid1;
    int status;

    // Usual fork. (cannot use "vfork()" here, because after we will use "vfork()" which is
    // different from "_exit()" and "exec*()" that are the only allowed functions).
    pid1 = fork();
    if (pid1 == 0) {
        // Child process

        // ****************************************
        // Notice this is "vfork()" and not "fork()"
        // See restrictions in documentation
        // ****************************************
        pid_t pid2 = vfork();
        if (pid2 == 0) {
            // Grand child process will try to launch the url
            execlp("xdg-open", "xdg-open", url, (char *)0);
			_exit(EXIT_FAILURE);
        } else if (pid2 < 0) {
            // Error forking
			_exit(EXIT_FAILURE);
        } else {
            // Child process doesn't wait for Grand child process which *might* be blocking.
            // Success forking
            _exit(EXIT_SUCCESS);
        }
    } else if (pid1 < 0) {
        return -1; // Error forking
    } else {
        if (waitpid(pid1, &status, 0) == pid1) {
             if (WIFEXITED(status)) {
                 if (WEXITSTATUS(status) == 0) {
                     return 0; // Sucess
                 } else {
                     return -1; // Error xdg-open
                 }
             } else {
                return -1; // Error in Child process
             }
        } else {
            return -1; // Error waiting for Child process
        }
    }

    return 0;
}
#endif


void share_url()
{
	char tmp[300];
	/*
	 * This needs a patched SDL2 with SDL_OpenURL.
	 * As of my writing, it is still in the discussion stages unfortunely...
	 * On other platforms it could be replaced with equivalent but on Android ?
	 * Not so easily... 
	*/
#ifdef ANDROID
	int size_str;
	if (hour[NORMAL] == 0 && minutes[NORMAL] == 0)
	{
		snprintf(tmp, sizeof(tmp), "twitter://post?message=I have rubbed 100 birds for %d seconds! ", seconds[NORMAL]);
		size_str = 62;
	}
	else if (hour[NORMAL] == 0 && minutes[NORMAL] > 0)
	{
		snprintf(tmp, sizeof(tmp), "twitter://post?message=I have rubbed 100 birds for %dm:%ds! ", minutes[NORMAL], seconds[NORMAL]);
		size_str = 60;
	}
	else
	{
		snprintf(tmp, sizeof(tmp), "twitter://post?message=I have rubbed 100 birds for %dh:%dm:%ds! ", hour[NORMAL], minutes[NORMAL], seconds[NORMAL]);
		size_str = 64;
	}
#else
	if (hour[NORMAL] == 0 && minutes[NORMAL] == 0)
	{
		snprintf(tmp, sizeof(tmp), "https://twitter.com/intent/tweet?text=I have rubbed 100 birds for %d seconds! ", seconds[NORMAL]);
	}
	else if (hour[NORMAL] == 0 && minutes[NORMAL] > 0)
	{
		snprintf(tmp, sizeof(tmp), "https://twitter.com/intent/tweet?text=I have rubbed 100 birds for %dm:%ds! ", minutes[NORMAL], seconds[NORMAL]);
	}
	else
	{
		snprintf(tmp, sizeof(tmp), "https://twitter.com/intent/tweet?text=I have rubbed 100 birds for %dh:%dm:%ds! ", hour[NORMAL], minutes[NORMAL], seconds[NORMAL]);
	}
#endif
	strcat(tmp, "%23rubbybird100 https://play.google.com/store/apps/details?id=com.gameblabla.rubbybird100");
	Play_SFX(1);
	SDL_OpenURL(tmp);
}

void Load_Highscore()
{
	FILE* file;
	char directory[256];
	unsigned char savefile[3];
	unsigned char i;
		
	#if defined(ANDROID)
		/* Saves in the internal storage path for the game */
		snprintf(directory, sizeof(directory), "%s/rubbybird100.save", SDL_AndroidGetInternalStoragePath());
	#elif defined(GCW0)
		/* Saves in the home directory */
		char letsgohomedir[128];
		snprintf(letsgohomedir, sizeof(letsgohomedir), "%s/.rubbybird100", getenv("HOME"));
		mkdir(letsgohomedir, 0755);
		snprintf(directory, sizeof(directory), "%s/rubbybird100.save", letsgohomedir);
	#elif defined(SAILFISHOS)
		/* Saves in the config directory */
		char letsgohomedir[256];
		snprintf(letsgohomedir, sizeof(letsgohomedir), "%s/.config/harbour-rubbybird100", getenv("HOME"));
		mkdir(letsgohomedir, 0755);
		snprintf(directory, sizeof(directory), "%s/rubbybird100.save", letsgohomedir);
	#elif defined(HOMEDIR)
		/* Saves in the config directory */
		char letsgohomedir[128];
		snprintf(letsgohomedir, sizeof(letsgohomedir), "%s/.config/rubbybird100", getenv("HOME"));
		mkdir(letsgohomedir, 0755);
		snprintf(directory, sizeof(directory), "%s/rubbybird100.save", letsgohomedir);
	#else
		/* Relative */
		snprintf(directory, sizeof(directory), "rubbybird100.save");
	#endif
		
	file = fopen(directory, "r+");
		
	if (file != NULL)
	{
		for(i=0;i<sizeof(savefile);i++)
		{
			savefile[i] = fgetc(file);
		}
		hour[HIGHSCORE] = savefile[0];
		minutes[HIGHSCORE] = savefile[1];
		seconds[HIGHSCORE] = savefile[2];
	}
	else
	{
		Save_Highscore();
	}
	
	if (file) fclose(file);	
}

void Save_Highscore()
{
	FILE* file;
	char directory[256];
	unsigned char savefile[3];
	unsigned char i;
		
	#if defined(ANDROID)
		/* Saves in the internal storage path for the game */
		snprintf(directory, sizeof(directory), "%s/rubbybird100.save", SDL_AndroidGetInternalStoragePath());
	#elif defined(GCW0)
		/* Saves in the home directory */
		char letsgohomedir[128];
		snprintf(letsgohomedir, sizeof(letsgohomedir), "%s/.rubbybird100", getenv("HOME"));
		mkdir(letsgohomedir, 0755);
		snprintf(directory, sizeof(directory), "%s/rubbybird100.save", letsgohomedir);
	#elif defined(SAILFISHOS)
		/* Saves in the config directory */
		char letsgohomedir[256];
		snprintf(letsgohomedir, sizeof(letsgohomedir), "%s/.config/harbour-rubbybird100", getenv("HOME"));
		mkdir(letsgohomedir, 0755);
		snprintf(directory, sizeof(directory), "%s/rubbybird100.save", letsgohomedir);
	#elif defined(HOMEDIR)
		/* Saves in the config directory */
		char letsgohomedir[128];
		snprintf(letsgohomedir, sizeof(letsgohomedir), "%s/.config/rubbybird100", getenv("HOME"));
		mkdir(letsgohomedir, 0755);
		snprintf(directory, sizeof(directory), "%s/rubbybird100.save", letsgohomedir);
	#else
		/* Relative */
		snprintf(directory, sizeof(directory), "rubbybird100.save");
	#endif

	savefile[0] = hour[HIGHSCORE];
	savefile[1] = minutes[HIGHSCORE];
	savefile[2] = seconds[HIGHSCORE];
		
	file = fopen(directory, "w+");
	
	for(i=0;i<3;i++)
	{
		fputc( savefile[i], file );
	}

	if (file) fclose(file);
}
