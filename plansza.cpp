#pragma once
#include "plansza.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <set>
plansza::plansza()
{
	potwierdz = new przycisk("media/button_wcisniety.png", "media/button_aktywny.png", "media/button_nieaktywny.png",
		40, 120, 400, 250);
	for (int i = 0; i < ROZMIAR_PLANSZY; i++)
	{
		for (int j = 0; j < ROZMIAR_PLANSZY; j++)
		{
			warcabnica[i][j] = new pole(i*ROZMIAR_POLA, j*ROZMIAR_POLA, i + 1, j + 1);
		}
	}
	ustaw_piony();
	al_flip_display();
	al_rest(0.005);
}
void plansza::rysuj_plansze()
{
	for (int i = 0; i < ROZMIAR_PLANSZY; i++)
	{
		for (int j = 0; j < ROZMIAR_PLANSZY; j++)
		{
			warcabnica[i][j]->rysuj_pole();
		}
	}
	potwierdz->rysuj_przycisk();
	wyswietl_piony();
	al_flip_display();
	al_rest(0.005);
}
pole* plansza::jakie_to_pole(float x, float y)
{
	for (int i = 0; i < ROZMIAR_PLANSZY; i++)
	{
		for (int j = 0; j < ROZMIAR_PLANSZY; j++)
		{
			float pole_x = warcabnica[i][j]->wspolrz_x;
			float pole_y = warcabnica[i][j]->wspolrz_y;
			if (pole_x<x&&pole_x + ROZMIAR_POLA>x&&pole_y<y&&pole_y + ROZMIAR_POLA>y)
			{
				printf("%d %d\n", i+1, j+1);
				return warcabnica[i][j];
			}
		}
	}
	printf("Poza plansza\n");
	return NULL;
}

pole* plansza::jakie_to_pole_numery(int x, int y)
{
	if (x <= 8 && x >= 1 && y <= 8 && y >= 1)
		return warcabnica[x-1][y-1];
	else
		return NULL;
}

void plansza::ustaw_piony()
{
	for (int i = 0, k=0; i < ROZMIAR_PLANSZY; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (warcabnica[i][j]->kolor == CZARNE_POLE)
			{
				float pole_x = warcabnica[i][j]->wspolrz_x;
				float pole_y = warcabnica[i][j]->wspolrz_y;
				biale[k] = new pion(warcabnica[i][j]);
				warcabnica[i][j]->ustaw_pion(biale[k]);
				k++;
			}
		}
	}
	for (int i = 0, k=0; i < ROZMIAR_PLANSZY; i++)
	{
		for (int j = ROZMIAR_PLANSZY-2; j < ROZMIAR_PLANSZY; j++)
		{
			if (warcabnica[i][j]->kolor == CZARNE_POLE)
			{
			float pole_x = warcabnica[i][j]->wspolrz_x;
			float pole_y = warcabnica[i][j]->wspolrz_y;
			czarne[k] = new pion(warcabnica[i][j]);
			warcabnica[i][j]->ustaw_pion(czarne[k]);
			k++;
			}
		}
	}
}

void plansza::wyswietl_piony()
{
	for (int i = 0; i < ROZMIAR_PLANSZY; i++)
	{
		for (int j = 0; j < ROZMIAR_PLANSZY; j++)
		{
			if ((warcabnica[i][j]->pion_stoi_na_polu) != NULL)
			{
				float pole_x = warcabnica[i][j]->wspolrz_x;
				float pole_y = warcabnica[i][j]->wspolrz_y;
				ALLEGRO_BITMAP  * kafelki = al_load_bitmap((warcabnica[i][j]->pion_stoi_na_polu->grafika_do_piona).c_str());
				al_draw_bitmap_region(kafelki, 0, 0, ROZMIAR_POLA, ROZMIAR_POLA, pole_x, pole_y, 0);
			}
		}
	}
}

bool plansza::przestaw_pion(pole* z_pola, pole* na_pole, gracz* grajacy)
{
	if (z_pola->pion_stoi_na_polu!=NULL&&na_pole->pion_stoi_na_polu==NULL&&
		z_pola->pion_stoi_na_polu->kolor==grajacy->kolor_pionow)
	{

		int z_pola_x = z_pola->numer_pola_poziom;
		int z_pola_y = z_pola->numer_pola_pion;
		int na_pole_x = na_pole->numer_pola_poziom;
		int na_pole_y = na_pole->numer_pola_pion;
		printf("Z pola %d %d", z_pola_x, z_pola_y);
		printf(" Na pole %d %d\n", na_pole_x, na_pole_y);
		if ((z_pola_y +1 == na_pole_y)&& (z_pola_x + 1 == na_pole_x || z_pola_x - 1 == na_pole_x))
		{
				na_pole->ustaw_pion(z_pola->pion_stoi_na_polu);
				z_pola->ustaw_pion(NULL);
				rysuj_plansze();
				grajacy->skok = false;
				grajacy->poprzednie_pole = NULL;
				return true;
		}
		else if ((z_pola_y - 1 == na_pole_y)&&( z_pola_x + 1 == na_pole_x || z_pola_x - 1 == na_pole_x))
		{
					na_pole->ustaw_pion(z_pola->pion_stoi_na_polu);
					z_pola->ustaw_pion(NULL);
					rysuj_plansze();
					grajacy->skok = false;
					grajacy->poprzednie_pole = NULL;
					return true;
		}
		else if ((z_pola_x + 2 == na_pole_x&&z_pola_y+2==na_pole_y&&
				jakie_to_pole_numery(z_pola_x+1, z_pola_y+1)!=NULL&&
				jakie_to_pole_numery(z_pola_x+1,z_pola_y+1)->pion_stoi_na_polu!=NULL)
				||( z_pola_x - 2 == na_pole_x&&z_pola_y-2==na_pole_y
					&&jakie_to_pole_numery(z_pola_x-1, z_pola_y-1) != NULL&&
					jakie_to_pole_numery(z_pola_x-1, z_pola_y-1)->pion_stoi_na_polu != NULL))
			{
				na_pole->ustaw_pion(z_pola->pion_stoi_na_polu);
				z_pola->ustaw_pion(NULL);
				rysuj_plansze();
				if (czy_jest_mozliwy_skok(z_pola, na_pole))
				{
					grajacy->poprzednie_pole = z_pola;
					grajacy->skok = true;
				}
				else
				{
					grajacy->poprzednie_pole = NULL;
					grajacy->skok = false;
				}
				return true;
			}
		else if ((z_pola_y + 2 == na_pole_y&& z_pola_x-2==na_pole_x&&
				jakie_to_pole_numery(z_pola_x-1, z_pola_y+1) != NULL&&
				jakie_to_pole_numery(z_pola_x-1, z_pola_y+1)->pion_stoi_na_polu != NULL)
				|| (z_pola_y - 2 == na_pole_y&&z_pola_x+2==na_pole_x&&
					jakie_to_pole_numery(z_pola_x+1, z_pola_y-1) != NULL&&
					jakie_to_pole_numery(z_pola_x+1, z_pola_y-1)->pion_stoi_na_polu != NULL))
			{
				na_pole->ustaw_pion(z_pola->pion_stoi_na_polu);
				z_pola->ustaw_pion(NULL);
				rysuj_plansze();
				if (czy_jest_mozliwy_skok(z_pola, na_pole))
				{
					grajacy->skok = true;
					grajacy->poprzednie_pole = z_pola;
				}
				else
				{
					grajacy->poprzednie_pole = NULL;
					grajacy->skok = false;
				}
				return true;
			}
		return false;
		}
	return false;
}

bool plansza::czy_koniec_gry()
{
	wygrana_czarnych = true;
	for (int i = 0; i < ROZMIAR_PLANSZY; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if ((warcabnica[i][j]->pion_stoi_na_polu == NULL&&
				warcabnica[i][j]->kolor==CZARNE_POLE)||
				(warcabnica[i][j]->pion_stoi_na_polu!=NULL&&
				warcabnica[i][j]->pion_stoi_na_polu->kolor == BIALY_PION))
				wygrana_czarnych = false;
		}
	}
	if (wygrana_czarnych == true)
		return true;
	for (int i = 0; i < ROZMIAR_PLANSZY; i++)
	{
		for (int j = ROZMIAR_PLANSZY - 2; j < ROZMIAR_PLANSZY; j++)
		{
			if ((warcabnica[i][j]->pion_stoi_na_polu == NULL &&
				warcabnica[i][j]->kolor == CZARNE_POLE )||
				(warcabnica[i][j]->pion_stoi_na_polu != NULL&&
				warcabnica[i][j]->pion_stoi_na_polu->kolor == CZARNY_PION))
				return false;
		}
	}
	return true;
}

bool plansza::ostateczne_zakonczenie(kolor_piona kolor_zwyciezcy)
{
	if (kolor_zwyciezcy == BIALY_PION)
	{
		for (int i = 0; i < ROZMIAR_PLANSZY; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if ((warcabnica[i][j]->pion_stoi_na_polu == NULL&&
					warcabnica[i][j]->kolor == CZARNE_POLE))
				{
					return false;
				}
			}
		}
		return true;
	}
	else
	{
		for (int i = 0; i < ROZMIAR_PLANSZY; i++)
		{
			for (int j = 6; j < ROZMIAR_PLANSZY; j++)
			{
				if ((warcabnica[i][j]->pion_stoi_na_polu == NULL&&
					warcabnica[i][j]->kolor == CZARNE_POLE))
				{
					return false;
				}
			}
		}
		return true;
	}
}

bool plansza::czy_jest_mozliwy_skok( pole*z_pola, pole*na_polu)
{
	int z_pola_y, z_pola_x;
	if (z_pola != NULL)
	{
		z_pola_x = z_pola->numer_pola_poziom;
		z_pola_y = z_pola->numer_pola_pion;
		int na_polu_x = na_polu->numer_pola_poziom;
		int na_polu_y = na_polu->numer_pola_pion;
		if (!(na_polu_x + 2 == z_pola_x && na_polu_y +2 == z_pola_y) &&
			jakie_to_pole_numery(na_polu_x + 2, na_polu_y+2) != NULL&&
			jakie_to_pole_numery(na_polu_x + 2, na_polu_y+2)->pion_stoi_na_polu == NULL&&
			jakie_to_pole_numery(na_polu_x + 1, na_polu_y+1) != NULL&&
			jakie_to_pole_numery(na_polu_x + 1, na_polu_y+1)->pion_stoi_na_polu != NULL)
			return true;
		if (!(na_polu_x - 2 == z_pola_x && na_polu_y-2 == z_pola_y) 
			&& jakie_to_pole_numery(na_polu_x - 2, na_polu_y-2) != NULL&&
			jakie_to_pole_numery(na_polu_x - 2, na_polu_y-2)->pion_stoi_na_polu == NULL&&
			jakie_to_pole_numery(na_polu_x - 1, na_polu_y-1) != NULL&&
			jakie_to_pole_numery(na_polu_x - 1, na_polu_y-1)->pion_stoi_na_polu != NULL)
			return true;
		if (!(na_polu_x -2== z_pola_x && na_polu_y + 2 == z_pola_y) 
			&& jakie_to_pole_numery(na_polu_x-2, na_polu_y + 2) != NULL&&
			jakie_to_pole_numery(na_polu_x-2, na_polu_y + 2)->pion_stoi_na_polu == NULL&&
			jakie_to_pole_numery(na_polu_x-1, na_polu_y + 1) != NULL&&
			jakie_to_pole_numery(na_polu_x-1, na_polu_y + 1)->pion_stoi_na_polu != NULL)
			return true;
		if (!(na_polu_x +2== z_pola_x && na_polu_y - 2 == z_pola_y) &&
			jakie_to_pole_numery(na_polu_x+2, na_polu_y - 2) != NULL&&
			jakie_to_pole_numery(na_polu_x+2, na_polu_y - 2)->pion_stoi_na_polu == NULL&&
			jakie_to_pole_numery(na_polu_x+1, na_polu_y - 1) != NULL&&
			jakie_to_pole_numery(na_polu_x+1, na_polu_y - 1)->pion_stoi_na_polu != NULL)
			return true;
	}
	else
	{
		int na_polu_x = na_polu->numer_pola_poziom;
		int na_polu_y = na_polu->numer_pola_pion;
		if (jakie_to_pole_numery(na_polu_x + 2, na_polu_y+2) != NULL&&
			jakie_to_pole_numery(na_polu_x + 2, na_polu_y+2)->pion_stoi_na_polu == NULL&&
			jakie_to_pole_numery(na_polu_x + 1, na_polu_y+1) != NULL&&
			jakie_to_pole_numery(na_polu_x + 1, na_polu_y+1)->pion_stoi_na_polu != NULL)
			return true;
		if (jakie_to_pole_numery(na_polu_x - 2, na_polu_y-2) != NULL&&
			jakie_to_pole_numery(na_polu_x - 2, na_polu_y-2)->pion_stoi_na_polu == NULL&&
			jakie_to_pole_numery(na_polu_x - 1, na_polu_y-1) != NULL&&
			jakie_to_pole_numery(na_polu_x - 1, na_polu_y-1)->pion_stoi_na_polu != NULL)
			return true;
		if (jakie_to_pole_numery(na_polu_x-2, na_polu_y + 2) != NULL&&
			jakie_to_pole_numery(na_polu_x-2, na_polu_y + 2)->pion_stoi_na_polu == NULL&&
			jakie_to_pole_numery(na_polu_x-1, na_polu_y + 1) != NULL&&
			jakie_to_pole_numery(na_polu_x-1, na_polu_y + 1)->pion_stoi_na_polu != NULL)
			return true;
		if (jakie_to_pole_numery(na_polu_x+2, na_polu_y - 2) != NULL&&
			jakie_to_pole_numery(na_polu_x+2, na_polu_y - 2)->pion_stoi_na_polu == NULL&&
			jakie_to_pole_numery(na_polu_x+1, na_polu_y - 1) != NULL&&
			jakie_to_pole_numery(na_polu_x+1, na_polu_y - 1)->pion_stoi_na_polu != NULL)
			return true;
	}
	return false;
}

bool plansza::skok_piona(pole* z_pola, pole* na_polu, pole* na_pole, gracz*grajacy)
{
	printf("PION SKACZE\n");
	if (na_polu->pion_stoi_na_polu != NULL&&z_pola!=NULL&&na_pole!=NULL&&na_pole->pion_stoi_na_polu == NULL&&
		na_polu->pion_stoi_na_polu->kolor == grajacy->kolor_pionow)
	{
		int z_pola_x = z_pola->numer_pola_poziom;
		int z_pola_y = z_pola->numer_pola_pion;
		int na_polu_x = na_polu->numer_pola_poziom;
		int na_polu_y = na_polu->numer_pola_pion;
		int na_pole_x = na_pole->numer_pola_poziom;
		int na_pole_y = na_pole->numer_pola_pion;
		if (!(na_polu_x + 2 == z_pola_x && na_polu_y+2 == z_pola_y)
			&&na_polu_x+2==na_pole_x&&na_polu_y+2==na_pole_y
			&& jakie_to_pole_numery(na_polu_x + 1, na_polu_y+1) != NULL&&
			jakie_to_pole_numery(na_polu_x + 1, na_polu_y+1)->pion_stoi_na_polu != NULL)
		{
			return przestaw_pion(na_polu, na_pole, grajacy);
		}
		if (!(na_polu_x - 2 == z_pola_x && na_polu_y -2 == z_pola_y)
			&& na_polu_x - 2 == na_pole_x&&na_polu_y -2 == na_pole_y
			&& jakie_to_pole_numery(na_polu_x - 1, na_polu_y-1) != NULL&&
			jakie_to_pole_numery(na_polu_x - 1, na_polu_y-1)->pion_stoi_na_polu != NULL)
		{
			return przestaw_pion(na_polu, na_pole, grajacy);
		}
		if (!(na_polu_x -2 == z_pola_x && na_polu_y + 2 == z_pola_y) 
			&&na_polu_x-2== na_pole_x&&na_polu_y +2== na_pole_y
			&&jakie_to_pole_numery(na_polu_x-1, na_polu_y + 1) != NULL&&
			jakie_to_pole_numery(na_polu_x-1, na_polu_y + 1)->pion_stoi_na_polu != NULL)
		{
			return przestaw_pion(na_polu, na_pole, grajacy);
		}
		if (!(na_polu_x +2 == z_pola_x && na_polu_y - 2 == z_pola_y) 
			&& na_polu_x+2== na_pole_x&&na_polu_y-2 == na_pole_y
			&&jakie_to_pole_numery(na_polu_x+1, na_polu_y - 1) != NULL&&
			jakie_to_pole_numery(na_polu_x+1, na_polu_y - 1)->pion_stoi_na_polu != NULL)
		{
			return przestaw_pion(na_polu, na_pole, grajacy);
		}
	}
	printf("I NIE SKOCZYL...\n");
	return false;
}

bool plansza::czy_jest_mozliwy_skok(pole* z_pola, pole*na_polu, pole*na_pole)
{
	if (czy_jest_mozliwy_skok(z_pola, na_polu) == false)
		return false;
	if (z_pola != na_pole&&na_pole->pion_stoi_na_polu == NULL)
	{
		int x_na_polu = na_polu->numer_pola_poziom;
		int y_na_polu = na_polu->numer_pola_pion;
		int x_na_pole = na_pole->numer_pola_poziom;
		int y_na_pole = na_pole->numer_pola_pion;
		if (x_na_polu + 2 == x_na_pole && ((y_na_polu + 2 == y_na_pole&&jakie_to_pole_numery(x_na_polu + 1, y_na_polu + 1) != NULL&&
			jakie_to_pole_numery(x_na_polu + 1, y_na_polu + 1)->pion_stoi_na_polu != NULL)
			|| y_na_polu - 2 == y_na_pole&&jakie_to_pole_numery(x_na_polu + 1, y_na_polu - 1) != NULL&&
			jakie_to_pole_numery(x_na_polu + 1, y_na_polu - 1)->pion_stoi_na_polu != NULL))
			return true;
		if (x_na_polu - 2 == x_na_pole && ((y_na_polu + 2 == y_na_pole&&jakie_to_pole_numery(x_na_polu - 1, y_na_polu + 1) != NULL&&
			jakie_to_pole_numery(x_na_polu - 1, y_na_polu + 1)->pion_stoi_na_polu != NULL)
			|| y_na_polu - 2 == y_na_pole&&jakie_to_pole_numery(x_na_polu - 1, y_na_polu - 1) != NULL&&
			jakie_to_pole_numery(x_na_polu - 1, y_na_polu - 1)->pion_stoi_na_polu != NULL))
			return true;
	}
	return false;
}

pole** plansza::mozliwosci_skoku(pole* na_polu,pole* z_pola=NULL)
{
	pole* tab[4]; //pion moze miec mozliwosc skoku w max. 4 kierunkach
	int i = 0;
	int x = na_polu->numer_pola_poziom;
	int y = na_polu->numer_pola_pion;
	if (jakie_to_pole_numery(x + 2, y + 2) != NULL&&czy_jest_mozliwy_skok(z_pola, na_polu, jakie_to_pole_numery(x + 2, y + 2)))
	{
		tab[i] = jakie_to_pole_numery(x + 2, y + 2);
		i++;
	}
	if (jakie_to_pole_numery(x - 2, y - 2) != NULL&&czy_jest_mozliwy_skok(z_pola, na_polu, jakie_to_pole_numery(x - 2, y - 2)))
	{
		tab[i] = jakie_to_pole_numery(x - 2, y - 2);
		i++;
	}
	if (jakie_to_pole_numery(x + 2, y - 2) != NULL&&czy_jest_mozliwy_skok(z_pola, na_polu, jakie_to_pole_numery(x + 2, y - 2)))
	{
		tab[i] = jakie_to_pole_numery(x + 2, y - 2);
		i++;
	}
	if (jakie_to_pole_numery(x - 2, y + 2) != NULL&&czy_jest_mozliwy_skok(z_pola, na_polu, jakie_to_pole_numery(x - 2, y + 2)))
	{
		tab[i] = jakie_to_pole_numery(x - 2, y + 2);
		i++;
	}
	for (; i < 4; i++)
		tab[i] = NULL;
	return tab;
}

pole** plansza::mozliwosci_ruchu(pole* na_polu)
{
	int i = 0;
	pole* tab[4];
	int x = na_polu->numer_pola_poziom;
	int y = na_polu->numer_pola_pion;
	if (na_polu->pion_stoi_na_polu != NULL)
	{
		if (jakie_to_pole_numery(x + 1, y + 1) != NULL&&jakie_to_pole_numery(x + 1, y + 1)->pion_stoi_na_polu == NULL)
		{
			tab[i] = jakie_to_pole_numery(x + 1, y + 1);
			i++;
		}
		if (jakie_to_pole_numery(x - 1, y - 1) != NULL&&jakie_to_pole_numery(x - 1, y - 1)->pion_stoi_na_polu == NULL)
		{
			tab[i] = jakie_to_pole_numery(x - 1, y - 1);
			i++;
		}
		if (jakie_to_pole_numery(x - 1, y + 1) != NULL&&jakie_to_pole_numery(x - 1, y + 1)->pion_stoi_na_polu == NULL)
		{
			tab[i] = jakie_to_pole_numery(x - 1, y + 1);
			i++;
		}
		if (jakie_to_pole_numery(x + 1, y - 1) != NULL&&jakie_to_pole_numery(x + 1, y - 1)->pion_stoi_na_polu == NULL)
		{
			tab[i] = jakie_to_pole_numery(x + 1, y - 1);
			i++;
		}
	}
	for (; i < 4; i++)
		tab[i] = NULL;
	return tab;
}


plansza::~plansza()
{
	delete warcabnica;
	delete biale;
	delete czarne;
	delete potwierdz;
}