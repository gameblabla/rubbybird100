extern unsigned short done;
extern struct input BUTTON;
extern const unsigned short FPS;
extern char* game_name;
extern struct tt touch;

extern void Init_video();
extern void UnDarken(unsigned char a);

extern void Load_Image(const unsigned short a, const char* directory, const unsigned char transparency);
extern void Copy_Image(unsigned short a, unsigned short i);

extern void Put_image(unsigned short a, short x, short y, unsigned char scale);
extern void Put_image_alpha (unsigned short a, unsigned char alpha);

extern void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f, float scale);
extern void Put_sprite_darken(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f, float scale, short c);

extern void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B);

extern void Draw_Rect(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned char R, unsigned char G, unsigned char B, unsigned char alpha);

/* 
 * Could cause problems on some platforms
 * Report if that's the case
 * 
*/
extern void Clear_Image(unsigned short a);
extern void Clear_Images();

extern void Clear_screen();

extern void Sync_refresh();
extern void Update_video();

extern void Controls();
extern void Clearing(unsigned char error);

extern void Init_sound();

extern void Load_Music(const char* directory);
extern void Play_Music(char loop);
extern void Clean_Music();
extern void Stop_Music();

extern void Load_SFX(unsigned char i, const char* directory);
extern void Play_SFX(unsigned char i);
extern void Unload_SFX();

/*					
	* 	 
	WARNING : DANGEROUS !
    DON'T USE OR RELY ON THEM  
	IF YOU ARE HEAVILY USING 
	ALLEGRO 5.0 OR SDL 2.0 !
	* 
*/

/*
extern void Faster_clearing(short x, short y, unsigned short w, unsigned short h);
extern void Faster_update(short x, short y, short w, short h);
*/

