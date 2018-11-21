#pragma once
#include "gracz.h"
#include "pole.hpp"
#include <math.h>
#include <set>
#include <map>
#include <algorithm>
#include <vector>
#include <iterator>
#include <math.h>
#include <cmath>
#define ILOSC_PIONOW 8
class galaz
{
public:
	galaz * poprz = NULL;
	int dlugosc_wzgledna_galezi = 0;
	int suma_dlugosci_poprz = 0;
	galaz * rozgalezienie[4];
	pole* na_pole=NULL;
	bool matka = false;
	galaz()
	{
		int i = 0;
		for (; i < 4; i++)
			rozgalezienie[i] = NULL;
	}
};
class gracz_komputer : public gracz
{
	friend class przycisk;
	friend class plansza;
	friend class gra;
	bool taktyka_zakonczenie = false;
	bool zakonczenie_w_lewo = false;
	bool zakonczenie_w_prawo = false;
	bool faza_zablokowania = true;
	pole*trasa[ROZMIAR_PLANSZY * 2];
	bool taktyka_skoku();
	bool taktyka_pierwszych_ruchow();
	pole* ostatnie_ruchy(pole*);
	bool pierwsze_ruchy = true;
	static const int ilosc_pionow = 8;
	int najdluszy_ruch_danego_piona(pole*, pole*, galaz*);
	pole* najdluzszy_ruch(int&);
	pole* najdluzszy_skok(pole*, pole*, galaz*head);
	pole* najdluzszy_bez_skoku(pole*);
	pole* taktyka_zakonczenia();//bezposrednio zapisuje wybrane_pole i nowe_pole, zwraca nowe_pole
	pole* taktyka_zakonczenia2();
	void taktyka_zakonczenia_false();
	pole* pole_wyjsciowe = NULL;
	pole* docelowe_pole(pole*);
	int najdluzszy_ruch_danego_piona_przeciwnika(pole* z_pola, pole* na_polu, galaz*head);
	void najdluzszy_ruch_przeciwnika();
	pole** trasa_piona_przeciwnika(pole* najlepsze);
	pole* najdluzszy_skok_przeciwnika(pole* z_pola, pole* na_polu, galaz* head);
	bool nieskonczona_rekursja = false;
	int licznik_rekursji=0;
	const int max_rekursja = ROZMIAR_PLANSZY*ROZMIAR_PLANSZY/2;
	set<int> suma_dlugosci;
	set<set<pole*>> ruchy_przeciwnika;
public:
	gracz_komputer(string& nick, kolor_piona kolor, plansza* plansz) :
		gracz(nick, kolor, plansz)
	{
		faza_zablokowania = true;
		for (int i = 0; i < ROZMIAR_PLANSZY * 2; i++)
			trasa[i] = NULL;
	};
	virtual void tura_gracza(ALLEGRO_MOUSE_STATE myszka, ALLEGRO_DISPLAY* okno);
	~gracz_komputer();
};