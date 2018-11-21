#pragma once
#include "pole.hpp"
#include "pion.h"
pole::pole(float x, float y, int nr_poz, int nr_pion)
	: wspolrz_x(x+marginesx), wspolrz_y(y+marginesy), numer_pola_poziom(nr_poz), numer_pola_pion(nr_pion)
{
	pion_stoi_na_polu = NULL;
	if (numer_pola_pion % 2 != numer_pola_poziom % 2)//jedna wspolrz parzysta, a druga nieparzysta - pole biale
	{
		grafika_do_pol = "media/zolte_pole.png";
		kolor = BIALE_POLE;
	}
	else
	{
		grafika_do_pol = "media/czerwone_pole.png";
		kolor = CZARNE_POLE;
	}
}

void pole::ustaw_pion(pion*pionek)
{
	pion_stoi_na_polu = pionek;
	if(pionek!=NULL)
		pionek->set_pole_piona(this);
}

void pole::rysuj_pole()
{
	ALLEGRO_BITMAP  * kafelki = al_load_bitmap(grafika_do_pol.c_str());
	al_draw_bitmap_region(kafelki, 0, 0, ROZMIAR_POLA, ROZMIAR_POLA, wspolrz_x, wspolrz_y, 0);
}
