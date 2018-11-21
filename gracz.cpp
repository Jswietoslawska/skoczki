#pragma once
#include "gracz.h"
gracz::gracz(string& nick, kolor_piona kolor, plansza* plansz)
	:kolor_pionow(kolor), pl(plansz), wybrane_pole(NULL), nowe_pole(NULL),
	skok(false), poprzednie_pole(NULL)
{
	printf("%s", nick);
	nick_gracza = nick;
	printf("%s", nick_gracza);
}

void gracz::tura_gracza(ALLEGRO_MOUSE_STATE myszka, ALLEGRO_DISPLAY* okno)
{
	pl->rysuj_plansze();
	rysuj_ture();
	ALLEGRO_EVENT event;
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(okno));
	if (skok == false)
	{
		pl->potwierdz->zmien_stan(NIEAKTYWNY);
		wybrane_pole = NULL;
	}
	else
	{
		pl->potwierdz->zmien_stan(AKTYWNY);
	}
	nowe_pole = NULL;
	bool przestaw = false;
		while (!przestaw)
		{
			al_get_mouse_state(&myszka);
			if (wybrane_pole == NULL&&al_mouse_button_down(&myszka, 1))
			{
				printf("KLIK1\n");
				al_get_mouse_state(&myszka);
				float x = myszka.x;
				float y = myszka.y;
				wybrane_pole = pl->jakie_to_pole(x,y);
				if (skok == true && pl->potwierdz->czy_wcisnieto(x,y))
				{
					pl->potwierdz->wcisniecie_przycisku_animacja();
					skok = false;
					break;
				}
				if (wybrane_pole != NULL&&wybrane_pole->pion_stoi_na_polu == NULL)
					wybrane_pole = NULL;
			}
			al_rest(0.1);
			al_get_mouse_state(&myszka);
			if (wybrane_pole != NULL&&al_mouse_button_down(&myszka, 1))
			{
				printf("KLIK2\n");
				al_get_mouse_state(&myszka);
				float x = myszka.x;
				float y = myszka.y;
				if (pl->jakie_to_pole(x,y)!=NULL&&
					pl->jakie_to_pole(x, y)->pion_stoi_na_polu != NULL&&			//jesli drugie klikniecie bedzie na pole z pionem to
					pl->jakie_to_pole(x, y)->pion_stoi_na_polu->kolor == kolor_pionow)//pion do przeniesienia zostanie zmieniony na inny
				{
					wybrane_pole = pl->jakie_to_pole(x, y);
				}
				else
					nowe_pole = pl->jakie_to_pole(x, y);
				if (skok == true && pl->potwierdz->czy_wcisnieto(x,y))
				{
					pl->potwierdz->wcisniecie_przycisku_animacja();
					skok = false;
					break;
				}
				if (nowe_pole != NULL&&nowe_pole->pion_stoi_na_polu != NULL)
					nowe_pole = NULL;
			}
			if (wybrane_pole != NULL&&nowe_pole != NULL&&skok==false)
			{
				if (!(przestaw = pl->przestaw_pion(wybrane_pole, nowe_pole, this)))
				{
					nowe_pole = NULL;
				}
			}
			else if (wybrane_pole != NULL&&nowe_pole != NULL&&skok == true)
			{
				pl->potwierdz->stan = AKTYWNY;
				printf("SKOK");
				if (!(przestaw = pl->skok_piona(poprzednie_pole,wybrane_pole, nowe_pole, this)))
				{
					nowe_pole = NULL;
				}
			}
		}
		if (skok == true)
		{
			printf("HOHO");
			pl->potwierdz->stan = AKTYWNY;
			pl->rysuj_plansze();
			wybrane_pole = nowe_pole;
			nowe_pole = NULL;
			this->tura_gracza(myszka, okno);
			pl->potwierdz->stan = NIEAKTYWNY;
		}
}

void gracz::rysuj_ture()
{
	ALLEGRO_FONT * font_ttf_30 = al_load_ttf_font("media/courbd.ttf", 30, 4);
	al_draw_textf(font_ttf_30, al_map_rgb(255, 255, 255), 360, 100, 0, "Tura gracza %s",nick_gracza.c_str());
	printf("%s", nick_gracza);
	al_flip_display();
}

gracz::~gracz()
{
	delete wybrane_pole;
	delete nowe_pole;
	delete poprzednie_pole;
	delete pl;
}