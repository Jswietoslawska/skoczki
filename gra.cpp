#pragma once
#include "gra.h"
#define MENU_ODSTEPY 10
#include <iostream>

gra::gra(plansza*pl, ALLEGRO_DISPLAY *okienko) : warcaby(pl), okno(okienko)
{
	koniec = false;
	/*ALLEGRO_DISPLAY * okno = al_create_display(720, 420);*/
	/*ALLEGRO_MOUSE_STATE myszka;*/
	event_queue = al_create_event_queue();
	al_install_mouse();
	al_register_event_source(event_queue, al_get_display_event_source(okno));
	font_ttf_30 = al_load_ttf_font("media/courbd.ttf", 30, 4);
	printf("OK");
};

int gra::wybor_rozgrywki()
{
	przycisk*dwoch_graczy = new przycisk("media/dwoch_graczy_wcisniety.png", "media/dwoch_graczy_aktywny.png",
		"media/dwoch_graczy_aktywny.png", WYSOKOSC_PRZYCISKU, SZEROKOSC_PRZYCISKU, menu_wspolrz_x, menu_wspolrz_y);
	przycisk*z_komputerem = new przycisk("media/z_komputerem_wcisniety.png", "media/z_komputerem_aktywny.png",
		"media/z_komputerem_aktywny.png", WYSOKOSC_PRZYCISKU, SZEROKOSC_PRZYCISKU,
		menu_wspolrz_x, menu_wspolrz_y + WYSOKOSC_PRZYCISKU + MENU_ODSTEPY);
	while (1)
	{
		al_get_next_event(event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			return 0;
		}
		al_clear_to_color(al_map_rgb(150, 80, 80));
		al_draw_textf(font_ttf_30, al_map_rgb(255, 255, 255), 160, 100, 0, "WITAJ W GRZE! SKOCZKI");
		dwoch_graczy->zmien_stan(AKTYWNY);
		dwoch_graczy->rysuj_przycisk();
		z_komputerem->zmien_stan(AKTYWNY);
		z_komputerem->rysuj_przycisk();
		al_flip_display();
		al_get_mouse_state(&myszka);
		if (al_mouse_button_down(&myszka, 1))
		{
			al_get_mouse_state(&myszka);
			float x = myszka.x;
			float y = myszka.y;
			if (dwoch_graczy->czy_wcisnieto(x, y))
			{
				dwoch_graczy->wcisniecie_przycisku_animacja();
				gracz_bialy = new gracz((string)"Bialy", BIALY_PION, warcaby);
				gracz_czarny = new gracz((string)"Czarny", CZARNY_PION, warcaby);
				return 1;
			}
			else if (z_komputerem->czy_wcisnieto(x, y))
			{
				z_komputerem->wcisniecie_przycisku_animacja();
				gracz_bialy = new gracz((string)"Bialy", BIALY_PION, warcaby);
				gracz_czarny = new gracz_komputer((string)"Czarny", CZARNY_PION, warcaby);
				return 0;
			}
		}
		al_rest(0.1);//pauza
	}

}

int gra::rozgrywka()
{
	//czytaj_slowo();


	//al_init();
	//al_init_image_addon();
	//al_init_font_addon();
	//al_init_ttf_addon(); // pamiêtaj !
	//al_install_keyboard();
	///*ALLEGRO_DISPLAY * okno = al_create_display(720, 420);*/
	////ALLEGRO_BITMAP  * kafelki = al_load_bitmap("media/warcaby.png");
	//ALLEGRO_MOUSE_STATE myszka;
	//ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	//al_install_mouse();
	//ALLEGRO_KEYBOARD_STATE klawiatura;
	//al_register_event_source(event_queue, al_get_display_event_source(okno));
	//ALLEGRO_EVENT event;
	//ALLEGRO_FONT * font_ttf_30 = al_load_ttf_font("media/courbd.ttf", 30, 4);
	//bool koniec = false;
	while (1)
	{
		al_get_next_event(event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			return 0;
		}
		al_clear_to_color(al_map_rgb(150, 80, 80));
		warcaby->rysuj_plansze();
		al_get_mouse_state(&myszka);
		//ja->rysuj_ture();
		gracz_bialy->tura_gracza(myszka, okno);
		if (koniec = warcaby->czy_koniec_gry())
		{
			printf("KONIEC GRY");
			dogrywka();
			return ogloszenie_zwyciezcy();
		}
		printf("TU JESTEM/n");
		//pl->rysuj_plansze();
		//on->rysuj_ture();
		al_clear_to_color(al_map_rgb(150, 80, 80));
		gracz_czarny->tura_gracza(myszka, okno);
		if (koniec = warcaby->czy_koniec_gry())
		{
			printf("KONIEC GRY");
			dogrywka();
			return ogloszenie_zwyciezcy();
		}
		//if (al_mouse_button_down(&myszka, 1))
		//{
		//	printf("KLIK\n");
		//	al_get_mouse_state(&myszka);
		//	pl->jakie_to_pole(myszka.x, myszka.y);
		//}
		al_rest(0.003);//pauza
	}
	//al_destroy_bitmap(kafelki);
	al_destroy_display(okno);
	al_destroy_event_queue(event_queue);
	return 0;
}

bool gra::dogrywka()
{
	koniec = false;
	if (warcaby->wygrana_czarnych)
	{
		while (1)
		{
			al_get_next_event(event_queue, &event);
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				return 0;
			}
			al_clear_to_color(al_map_rgb(150, 80, 80));
			warcaby->rysuj_plansze();
			al_get_mouse_state(&myszka);
			gracz_bialy->tura_gracza(myszka, okno);
			warcaby->licznik_zakonczenia++;
			if (koniec = warcaby->ostateczne_zakonczenie(CZARNY_PION))
			{
				printf("KONIEC GRY");
				return true;
			}
			al_clear_to_color(al_map_rgb(150, 80, 80));
			al_rest(0.003);//pauza
		}
	}
	else
	{
		while (1)
		{
			al_get_next_event(event_queue, &event);
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				return 0;
			}
			al_clear_to_color(al_map_rgb(150, 80, 80));
			warcaby->rysuj_plansze();
			al_get_mouse_state(&myszka);
			gracz_czarny->tura_gracza(myszka, okno);
			warcaby->licznik_zakonczenia++;
			if (koniec = warcaby->ostateczne_zakonczenie(BIALY_PION))
			{
				printf("KONIEC GRY");
				return true;
			}
			al_clear_to_color(al_map_rgb(150, 80, 80));
			al_rest(0.003);//pauza
		}
	}
}

int gra :: ekran_powitalny()
{
	przycisk*nowa_gra = new przycisk("media/nowa_gra_wcisniety.png", "media/nowa_gra_aktywny.png",
		"media/nowa_gra_niekatywny.png", WYSOKOSC_PRZYCISKU, SZEROKOSC_PRZYCISKU, menu_wspolrz_x, menu_wspolrz_y);
	przycisk*zakoncz = new przycisk("media/zakoncz_wcisniety.png", "media/zakoncz_aktywny.png",
		"media/zakoncz_niekatywny.png", WYSOKOSC_PRZYCISKU, SZEROKOSC_PRZYCISKU,
		menu_wspolrz_x, menu_wspolrz_y + WYSOKOSC_PRZYCISKU + MENU_ODSTEPY);
	while (1)
	{
		al_get_next_event(event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			return 0;
		}
		al_clear_to_color(al_map_rgb(150, 80, 80));
		al_draw_textf(font_ttf_30, al_map_rgb(255, 255, 255), 160, 100, 0, "WITAJ W GRZE! SKOCZKI");
		nowa_gra->zmien_stan(AKTYWNY);
		nowa_gra->rysuj_przycisk();
		zakoncz->zmien_stan(AKTYWNY);
		zakoncz->rysuj_przycisk();
		al_flip_display();
		al_get_mouse_state(&myszka);
		if (al_mouse_button_down(&myszka, 1))
		{
			al_get_mouse_state(&myszka);
			float x = myszka.x;
			float y = myszka.y;
			if (nowa_gra->czy_wcisnieto(x, y))
			{
				nowa_gra->wcisniecie_przycisku_animacja();
				wybor_rozgrywki();
				return 1;
			}
			else if (zakoncz->czy_wcisnieto(x, y))
			{
				zakoncz->wcisniecie_przycisku_animacja();
				return 0;
			}
		}
		al_rest(0.1);//pauza
	}
	//al_destroy_display(okno);
	//al_destroy_event_queue(event_queue);
}

using namespace std;

int gra::ogloszenie_zwyciezcy()
{
	przycisk*powrot = new przycisk("media/powrot_wcisniety.png", "media/powrot_aktywny.png", "media/zakoncz_niekatywny.png", WYSOKOSC_PRZYCISKU, SZEROKOSC_PRZYCISKU, 300, 350);
		al_clear_to_color(al_map_rgb(150, 80, 80));
		powrot->zmien_stan(AKTYWNY);
		powrot->rysuj_przycisk();
		if (warcaby->wygrana_czarnych == true)
		{
			al_draw_textf(font_ttf_30, al_map_rgb(255, 255, 255), 160, 100, 0, "WYGRAL GRACZ %s", gracz_czarny->nick_gracza.c_str());
			al_draw_textf(font_ttf_30, al_map_rgb(255, 255, 255), 30, 140, 0, "Gracz %s potrzebowal %d ruchow,", gracz_bialy->nick_gracza.c_str(), warcaby->licznik_zakonczenia);
			al_draw_textf(font_ttf_30, al_map_rgb(255, 255, 255), 160, 180, 0, " aby dogonic zwyciezce.");
		}
		else
		{
			al_draw_textf(font_ttf_30, al_map_rgb(255, 255, 255), 160, 100, 0, "WYGRAL GRACZ %s", gracz_bialy->nick_gracza.c_str());
			al_draw_textf(font_ttf_30, al_map_rgb(255, 255, 255), 30, 140, 0, "Gracz %s potrzebowal %d ruchow,", gracz_czarny->nick_gracza.c_str(), warcaby->licznik_zakonczenia);
			al_draw_textf(font_ttf_30, al_map_rgb(255, 255, 255), 160, 180, 0, " aby dogonic zwyciezce.");
		}
		al_flip_display();
		while (true)
		{
			al_get_next_event(event_queue, &event);
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				return 0;
			}
		al_get_next_event(event_queue, &event);
		al_get_mouse_state(&myszka);
		if (al_mouse_button_down(&myszka, 1))
		{
			al_get_mouse_state(&myszka);
			float x = myszka.x;
			float y = myszka.y;
			if (powrot->czy_wcisnieto(x, y))
			{
				powrot->wcisniecie_przycisku_animacja();
				return 1;
			}
		}
	}
}

gra::~gra()
{
	delete warcaby;
	delete gracz_bialy;
	delete gracz_czarny;
}