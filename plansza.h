#pragma once
#include "pole.hpp"
#include "gracz.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include "pion.h"
#include "przycisk.h"
#define ROZMIAR_PLANSZY 8
class plansza
{
	friend class gracz_komputer;
	pole *warcabnica[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY];
	pion *biale[ROZMIAR_PLANSZY];
	pion *czarne[ROZMIAR_PLANSZY];
	void ustaw_piony();
	void wyswietl_piony();
public:
	bool wygrana_czarnych;
	plansza();
	przycisk* potwierdz;
	bool czy_koniec_gry();
	void rysuj_plansze();
	pole* jakie_to_pole(float x, float y);
	pole* jakie_to_pole_numery(int x, int y);
	bool przestaw_pion(pole* z_pola, pole* na_pole, gracz*);
	bool czy_jest_mozliwy_skok( pole*, pole*);
	bool czy_jest_mozliwy_skok(pole*, pole*, pole*);
	pole** mozliwosci_skoku(pole*,pole*);
	pole** mozliwosci_ruchu(pole*);
	bool skok_piona(pole* z_pola, pole* na_polu, pole*na_pole, gracz*);
	bool ostateczne_zakonczenie(kolor_piona);
	int licznik_zakonczenia = 0;
	~plansza();
};
