#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "pole.hpp"
#include "plansza.h"
#include "gracz.h"
#include "przycisk.h"
#include "gra.h"
#include "gracz_komputer.h"
using namespace std;
int main()
{
	al_init();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	ALLEGRO_DISPLAY *okno= al_create_display(720, 420);
	//plansza *pl=new plansza();
	//gra *gierka = new gra(pl);
	while (1)
	{
		plansza *pl = new plansza();
		gra *gierka = new gra(pl, okno);
		int decyzja = gierka->ekran_powitalny();
		if (decyzja == 0)
			return 0;
		else if (decyzja == 1)
		{
			if (gierka->rozgrywka() == 0)
				return 0;
		}
	}
}