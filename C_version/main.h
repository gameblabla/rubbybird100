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

void InitializeGame();
void GameLoop();
short rand_a_b(short a, short b);

void Titlescreen();
void Instructions_screen();
void thegame_ingame();
void results_screen();

inline void toTitlescreen();

void Init_birds();
inline void animate_bird();
void Draw_birds_title();
void Draw_birds_ingame();
unsigned char Kill_all_birds();

void Draw_scorebirds_onscreen();
void Draw_time(unsigned short y, unsigned char mode);


int Collision(unsigned short x, unsigned short y, unsigned char w, unsigned char h, unsigned short x2, unsigned short y2, unsigned char w2, unsigned char h2);

void share_url();
void Load_Highscore();
void Save_Highscore();
char* GetFile();
