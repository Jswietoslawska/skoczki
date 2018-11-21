#pragma once
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "pole.hpp"
#include "plansza.h"
#include "gracz.h"
#include "przycisk.h"
#include "gracz_komputer.h"
class gra
{
	plansza* warcaby;
	gracz* gracz_czarny;
	gracz* gracz_bialy;
	bool koniec;
	ALLEGRO_DISPLAY * okno;
	ALLEGRO_MOUSE_STATE myszka;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_KEYBOARD_STATE klawiatura;
	ALLEGRO_EVENT event;
	ALLEGRO_FONT * font_ttf_30;
	const float  menu_wspolrz_x = 300;
	const float menu_wspolrz_y = 180;
public:
	gra(plansza*pl, ALLEGRO_DISPLAY * okienko);
	int rozgrywka();
	int wybor_rozgrywki();
	int ekran_powitalny(); //0-zakniecie gry, 1-nowa gra
	int ogloszenie_zwyciezcy();
	bool dogrywka();
	~gra();
};