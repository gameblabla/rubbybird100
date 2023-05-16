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

var diffilculty_birds_required = [1, 3, 5, 8, 10, 15, 20, 30, 40, 42];
var diffilculty_remove_life = [1, 1, 1, 1, 2, 3, 4, 4, 5, 8];
var diffilculty_give_life = [10, 10, 10, 10, 8, 8, 8, 6, 5, 4];

var img_memory = Array(13);
var precalculed_fps = 1000/60;
var background;
var first_layer;
var sound = Array(15);
var poss_x, poss_y;
var tapped;
var logo_y = -90;
var titlescreen_state = 0;

var birds__x = Array(100);
var birds__y = Array(100);
var birds__status = Array(100);
var birds__life = Array(100);
var birds__time = Array(100);
var birds__time2 = Array(100);
var birds__speed = Array(100);
var birds__direction = Array(100);
var birds__direction2 = Array(100);
var birds__scale = Array(100);
var birds__shadow = Array(100);

var bird_title_frame = 0;
var bird_title_time = 0;

var touch_state = 0;
var game_mode;
var appear_time = 0;

var hour = Array(2);
var minutes = Array(2);
var seconds = Array(2);
var microseconds = 0;
var touch_state_time = 0;
var vulnereable_time = 0;
var alert_time = 0;
var birds_limit_time = 0;

var poss_x_old = 0;
var poss_y_old = 0;
var gameover_x = - 272;

window.onload = function() 
{
	var i;
	first_layer = document.getElementById('canvas');
	background = first_layer.getContext('2d'); // A layer for the background
	mc = new Hammer(first_layer);
	
	first_layer.imageSmoothingEnabled = false;
	
	game_mode = 0;
	
	for (i=0;i<17;i++)
	{
		img_memory[i] = new Image();
	}

	hour[1] = 0;
	minutes[1] = 0;
	seconds[1] = 0;
	hour[1] = localStorage.getItem("rubbybird100-hour") || 0; 
	minutes[1] = localStorage.getItem("rubbybird100-minute") || 0; 
	seconds[1] = localStorage.getItem("rubbybird100-second") || 0; 
	hour[0] = 0; 
	minutes[0] = 0; 
	seconds[0] = 0; 
	
	sound[0] = new Howl({ src: ['data/rubby.ogg', 'data/rubby.mp3'], loop: true });
	sound[1] = new Howl({ src: ['data/dies.ogg', 'data/dies.mp3'] });
	sound[2] = new Howl({ src: ['data/tick.ogg', 'data/tick.mp3'] });
	sound[3] = new Howl({ src: ['data/alarm.ogg', 'data/alarm.mp3'] });
	
	sound[0].play();
	
	img_memory[0].src = 'data/fond.png';
	img_memory[1].src = 'data/bird.png';
	img_memory[2].src = 'data/logo.png';
	img_memory[3].src = 'data/score.png';
	img_memory[4].src = 'data/go.png';
	img_memory[5].src = 'data/ddot.png';
	img_memory[6].src = 'data/bird.png';
	img_memory[7].src = 'data/limit_score.png';
	img_memory[8].src = 'data/inst.png';
	img_memory[9].src = 'data/tap.png';
	img_memory[10].src = 'data/tweet.png';
	img_memory[11].src = 'data/copyright.png';
	img_memory[12].src = 'data/bird1x.png';
	img_memory[13].src = 'data/score2x.png';
	img_memory[14].src = 'data/score3x.png';
	
	logo_y = -90;
	titlescreen_state = 0;
	game_mode = 0;
	
	Init_birds();

	(function (window) 
	{
		function mainloop() 
		{
			touch();
			background.drawImage(img_memory[0],0,0);
			switch(game_mode)
			{
				case 0:
					Titlescreen();
				break;
				case 1:
					Instructions_screen();
				break;
				case 2:
					thegame_ingame();
				break;
				case 3:
					results_screen();
				break;
			}
			tapped = 0;
	    }
	    requestAnimationFrame(mainloop);
		window.setInterval(mainloop, precalculed_fps); // 60 fps	
		
	} (window));
	
	mc.on("tap", function(e) 
	{
		var currX, currY;
		currX = e.pointers[0].layerX;
		currY = e.pointers[0].layerY;
		poss_x = (currX*(320/$(first_layer).width())) | 0;
		poss_y = (currY*(480/$(first_layer).height())) | 0;
		tapped = 1;
	});

	mc.on("pan", function(e) 
	{
		var currX, currY;
		currX = e.pointers[0].layerX;
		currY = e.pointers[0].layerY;
		poss_x = (currX*(320/$(first_layer).width())) | 0;
		poss_y = (currY*(480/$(first_layer).height())) | 0;
		tapped = 1;
	});
	
};  


/*
 * Controls
*/

function touch()
{
	poss_x_old = poss_x;
	poss_y_old = poss_y;
	switch(touch_state)
	{
		case 0:
			if (tapped == 1)
			{
				touch_state = 1;
			}
		break;
		case 1:
			touch_state_time++;
			if (touch_state_time > 0)
			{
				touch_state = 2;
				touch_state_time = 0;
			}
		break;
		case 2:
			if (tapped == 0)
			{
				touch_state = 3;
				touch_state_time = 0;
			}
		break;
		case 3:
			touch_state_time++;
			if (touch_state_time > 1)
			{
				touch_state = 0;
				touch_state_time = 0;
			}
		break;
	}
}

function Put_image(a, b, c, d)
{
	background.drawImage(img_memory[a], b, c);
}

function Put_sprite(a, b, c, d, e, f, scale)
{
	background.drawImage(img_memory[a], d*f, 0, d, e, b, c, d, e);
}

function Init_birds()
{
	var i;
	
	for(i=0;i<100;i++)
	{
		birds__x[i] = rand_a_b(0, 320-36);
		birds__y[i] = rand_a_b(150, 480-48);
		birds__time[i] = 0;
		birds__time2[i] = 0;
		birds__speed[i] = 0;
		birds__direction[i] = rand_a_b(0, 2);
		birds__direction2[i] = rand_a_b(0, 2);
		birds__scale[i] = 1;
		birds__status[i] = 0;
		birds__life[i] = rand_a_b(248, 255);
		birds__shadow[i] = 0;
	}
}

function Titlescreen()
{
	Draw_birds_title();
	background.drawImage(img_memory[2],24,logo_y);
	background.drawImage(img_memory[11],32,400);
	Draw_time(390, 1);
	
	switch(titlescreen_state)
	{
		case 0:	
			if (logo_y < 30) logo_y+=2;
			
			if (tapped > 0)
			{
				titlescreen_state = 1;
			}
		break;
		case 1:
			if (logo_y > -90) 
				logo_y-=2;
			else 
			{
				game_mode = 1;
				birds_limit = 0;
				titlescreen_state = 0;
			}
		break;
	}
}


function Draw_time(y, mode)
{
	var minutes_toshow = Array(3)
	var sec_toshow = Array(3)
	var hour_toshow = Array(3);
	
	sec_toshow[0] = ((seconds[mode] / 10) % 10) | 0;
	sec_toshow[1] = ((seconds[mode]) % 10) | 0;
	
	minutes_toshow[0] = ((minutes[mode] / 10) % 10) | 0;
	minutes_toshow[1] = ((minutes[mode]) % 10) | 0;
	hour_toshow[0] = ((hour[mode] / 10) % 10) | 0;
	hour_toshow[1] = ((hour[mode]) % 10) | 0;
	
	Put_sprite(13, 95, 70+y, 20, 20, hour_toshow[0], 1);	
	Put_sprite(13, 114, 70+y, 20, 20, hour_toshow[1], 1);	
	Put_image(5,132,80+y);
	Put_sprite(13, 145, 70+y, 20, 20, minutes_toshow[0], 1);	
	Put_sprite(13, 164, 70+y, 20, 20, minutes_toshow[1], 1);	
	Put_image(5,183,80+y);
	Put_sprite(13, 195, 70+y, 20, 20, sec_toshow[0], 1);	
	Put_sprite(13, 215, 70+y, 20, 20, sec_toshow[1], 1);	
}


function Instructions_screen()
{
	Draw_birds_title();
	background.drawImage(img_memory[8],24,92);
	
	if (appear_time < 30)
	background.drawImage(img_memory[9],70,300);
	else if (appear_time > 60) appear_time = 0;
	appear_time++;
	
	if (tapped > 0)
	{
		game_mode = 2;
	}
}

function thegame_ingame()
{
	Draw_birds_ingame();
	Draw_scorebirds_onscreen();
	Draw_time(0, 0);

	birds_limit_time++;
	if (birds_limit_time > 300)
	{
		sound[2].play();
		if (birds_limit < 10) birds_limit++;
		birds_limit_time = 0;
	}
	
	microseconds = microseconds + 1;
	if (microseconds > 59)
	{
		seconds[0] = seconds[0] + 1;
		microseconds = 0;
		if (seconds[0] > 59)
		{
			minutes[0] = minutes[0] + 1;
			seconds[0] = 0;
			if (minutes[0] > 59)
			{
				hour[0] = hour[0] + 1;
				minutes[0] = 0;
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

function toTitlescreen()
{
	game_mode = 0;
	logo_y = -60;

	birds_dead = 0;
	birds_limit = 0;
	birds_limit_time = 0;
	
	/*
	 * Checks if the user beat the current highscore. 
	 * If so, it will also make sure to save it.
	*/
	if ((hour[0] > hour[1]) || (minutes[0] > minutes[1]) || seconds[0] > seconds[1])
	{
		hour[1] = hour[0];
		minutes[1] = minutes[0];
		seconds[1] = seconds[0];
		localStorage.setItem("rubbybird100-hour",parseInt(hour[1]));
		localStorage.setItem("rubbybird100-minute",parseInt(minutes[1]));
		localStorage.setItem("rubbybird100-second",parseInt(seconds[1]));
	}
	
	hour[0] = 0;
	minutes[0] = 0;
	seconds[0] = 0;
	tapped = 0;
	Init_birds();
	sound[0].play();
}


/*
 * The results screen, when you die.
 * You can either decide to share your highscore (via Twitter)
 * or go back to the titlescreen.
 * 
*/
function results_screen()
{
	Put_image(4,gameover_x,202,1);
	Put_image(10,gameover_x+48,320,1);
	
	if (gameover_x < 24) 
	{
		gameover_x+=4;
	}
	else
	{
		Draw_time(200, 0);	
		if (tapped == 1)
		{
			if (poss_x > 69 && poss_x < 264)
			{
				if (poss_y > 280 && poss_y < 350)
				{
					window.open('https://twitter.com/intent/tweet?text=I have rubbed 100 birds for for '+hour[0]+":"+minutes[0]+':'+seconds[0]+'. '+' #rubbybird100');	
				}
				else if (poss_y > 380 && poss_y < 444)
				{
					gameover_x = -272;
					toTitlescreen();
				}
			}
		}
	}
}


function Draw_scorebirds_onscreen()
{
	var bird_numb = Array(3), alive, bird_tokeep = Array(2), vulnereable = 0;
	var go = 0;
	alive = (100-birds_dead);
	bird_numb[0] = ((alive / 100) % 10) | 0;
	bird_numb[1] = ((alive / 10) % 10) | 0;
	bird_numb[2] = ((alive) % 10) | 0;
	bird_tokeep[0] = ((diffilculty_birds_required[birds_limit] / 10) % 10) | 0;
	bird_tokeep[1] = ((diffilculty_birds_required[birds_limit]) % 10) | 0;
	
	if (alive < (diffilculty_birds_required[birds_limit]))
	{
		go = Kill_all_birds();
	}
	else if (alive < (diffilculty_birds_required[birds_limit]+15))
	{
		vulnereable = 10;
		vulnereable_time++;
	}

	Put_sprite(12, 95, 36, 18, 24, bird_title_frame, 1);	
	Put_sprite(7, 210, 40, 20, 20, bird_tokeep[0], 1);	
	Put_sprite(7, 227, 40, 20, 20, bird_tokeep[1], 1);	
	
	if (vulnereable == 10)
	{
		alert_time++;
		if (alert_time > 45)
		{
			sound[3].play();
			alert_time = 0;
		}
		switch( (vulnereable_time < 15 ? 1 : 0) + (vulnereable_time > 30 ? 2 : 0) )
		{
			case 1:
				Put_sprite(14, 120, 30, 30, 30, bird_numb[0]+10, 1);	
				Put_sprite(14, 150, 30, 30, 30, bird_numb[1]+10, 1);	
				Put_sprite(14, 180, 30, 30, 30, bird_numb[2]+10, 1);	
			break;
			case 2:
				vulnereable_time = 0;
			break;
		}
	}
	else
	{
		Put_sprite(14, 120, 30, 30, 30, bird_numb[0], 1);	
		Put_sprite(14, 150, 30, 30, 30, bird_numb[1], 1);	
		Put_sprite(14, 180, 30, 30, 30, bird_numb[2], 1);
	}
	
	if (go == 1 && birds_dead == 100)
	{
		sound[0].stop();
		game_mode = 3;
	}
}

function Draw_birds_title()
{
	var i, il;
	animate_bird();
	
	
	for (i=0, il=100; i<il; i++)
	{
		background.drawImage(img_memory[1], 36*bird_title_frame, 0, 36, 48, birds__x[i], birds__y[i], 36, 48);
		
		birds__time[i]++;
		birds__time2[i]++;
					
		if (birds__direction[i] == 0)
		{
			if (birds__time[i] > birds__speed[i]) 
			{
				birds__x[i]--;
				birds__time[i] = 0;
			}
			if (birds__x[i] < 1) birds__direction[i] = 1;
		}
		else
		{
			if (birds__time[i] > birds__speed[i]) 
			{
				birds__x[i]++;
				birds__time[i] = 0;
			}
			if (birds__x[i] > 284) birds__direction[i] = 0;
		}
		if (birds__direction2[i] == 0)
		{
			if (birds__time2[i] > birds__speed[i]) 
			{
				birds__y[i]--;
				birds__time2[i] = 0;
			}
			if (birds__y[i] < 150) birds__direction2[i] = 1;
		}
		else
		{
			if (birds__time2[i] > birds__speed[i]) 
			{
				birds__y[i]++;
				birds__time2[i] = 0;
			}
			if (birds__y[i] > 432) birds__direction2[i] = 0;
		}
	}
}

function Draw_birds_ingame()
{
	var i, il;
	var realtouch_x;
	var realtouch_y;
	var realtouch_w;
	var realtouch_h;
	var percent;
	
	if (tapped == 1)
	{
		realtouch_x = poss_x - 72;
		realtouch_y = poss_y - 72;
		realtouch_w = 148;
		realtouch_h = 148;
	}
	
	birds_dead = 0;
	
	animate_bird();
	
	for (i=0, il=100; i<il; i++)
	{
		switch(birds__status[i])
		{
			case 0:
				background.drawImage(img_memory[1], (36*bird_title_frame)+birds__shadow[i], 0, 36, 48, birds__x[i], birds__y[i], 36, 48);

				birds__time[i]++;
				birds__time2[i]++;
				
				percent = birds__life[i]/255;
				
				switch (true) 
				{
					case percent < 0.2:
						birds__shadow[i] = 576;
						break;
					case percent < 0.4:
						birds__shadow[i] = 432;
						break;
					case percent < 0.6:
						birds__shadow[i] = 288;
						break;
					case percent < 0.8:
						birds__shadow[i] = 144;
						break;   
					default:
						birds__shadow[i] = 0;
					break;              
				}
					
				/*
				* Drain the bird's life if not touched
				*/
				if (birds__speed[i] == 0)
				{
					birds__life[i] = birds__life[i] - diffilculty_remove_life[birds_limit];
				}
					
				/*
				* direction and direction2 is used to know
				* whetever the bird should go up or down
				*/
					
				if (birds__direction[i] == 0)
				{
					if (birds__time[i] > birds__speed[i]) 
					{
						birds__x[i] = birds__x[i] - 1;
						birds__time[i] = 0;
					}
					if (birds__x[i] < 1) birds__direction[i] = 1;
				}
				else
				{
					if (birds__time[i] > birds__speed[i]) 
					{
						birds__x[i] = birds__x[i] + 1;
						birds__time[i] = 0;
					}
					if (birds__x[i] > 284) birds__direction[i] = 0;
				}
				if (birds__direction2[i] == 0)
				{
					if (birds__time2[i] > birds__speed[i]) 
					{
						birds__y[i] = birds__y[i] - 1;
						birds__time2[i] = 0;
					}
					if (birds__y[i] < 150) birds__direction2[i] = 1;
				}
				else
				{
					if (birds__time2[i] > birds__speed[i]) 
					{
						birds__y[i] = birds__y[i] + 1;
						birds__time2[i] = 0;
					}
					if (birds__y[i] > 432) birds__direction2[i] = 0;
				}

				/*
				* If bird is touched then slow him down and give it some life
				*/
				if (Collision(birds__x[i], birds__y[i], 18, 24, realtouch_x, realtouch_y, realtouch_w, realtouch_h))
				{
					birds__speed[i] = 2;
					if (birds__life[i] < 255-diffilculty_give_life[birds_limit]) birds__life[i]+=diffilculty_give_life[birds_limit];
				}
				else
				{
					birds__speed[i] = 0;
				}
					
				/*
				 * if Health down to 0 then bird dies
				*/
				if (birds__life[i] < 0) 
				{
					sound[1].play();
					birds__status[i] = 1;
				}
			break;
			case 1:
				background.drawImage(img_memory[1], (36*bird_title_frame)+birds__shadow[i], 0, 36, 48, birds__x[i], birds__y[i], 36*birds__scale[i], 48*birds__scale[i]);
				birds__scale[i] -= 0.05;
				if (birds__scale[i] < 0.2)
				{
					birds__status[i] = 2;
				}
			break;
			case 2:
				birds_dead++;
			break;
		}
	}
}

function Kill_all_birds()
{
	var i, il;
	for (i=0, il=100; i<il; i++)
	{
		birds__status[i] = 1;
	}
	return 1;
}

function Collision(x, y, w, h, x2, y2, w2, h2)
{
	if ((x + w > x2) && (x < x2 + w2) && (y + h > y2) && (y < y2 + h2))
	{
		return 1;
	}	
	return 0;
}

function animate_bird()
{
	bird_title_time++;
	if (bird_title_time > 8)
	{
		bird_title_frame++;
		if (bird_title_frame > 3) bird_title_frame = 0;
		bird_title_time = 0;
	}
}

function rand_a_b(a, b)
{
	a = Math.ceil(a);
	b = Math.floor(b);
	return Math.floor(Math.random() * (b - a + 1)) + a;
}
