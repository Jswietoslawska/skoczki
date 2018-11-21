#pragma once
#include <string>
#include "pion.h"
#include "plansza.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
using namespace std;
class gracz
{
protected:
	friend class przycisk;
	friend class plansza;
	friend class gra;
	string nick_gracza;
	kolor_piona kolor_pionow;
	plansza* pl;
	pole*poprzednie_pole;
	pole*wybrane_pole;
	pole*nowe_pole;
	bool skok;
public:
	gracz(string& nick, kolor_piona kolor, plansza* plansz);
	virtual void tura_gracza(ALLEGRO_MOUSE_STATE myszka, ALLEGRO_DISPLAY* okno);
	void rysuj_ture();
	~gracz();
};