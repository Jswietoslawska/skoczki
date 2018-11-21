#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>

using namespace std;
class odczyt
{
	string slowo;
public:
	string czytaj_slowo(ALLEGRO_KEYBOARD_STATE klawiatura);
};