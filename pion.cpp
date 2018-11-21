#pragma once
#include "pion.h"
pion::pion(pole*pole_piona)
	:pole_pion(pole_piona)
{
	if (pole_piona->numer_pola_pion == 1 || pole_piona->numer_pola_pion == 2)
	{
		grafika_do_piona = "media/pion_bialy.png";
		kolor = BIALY_PION;
	}
	else
	{
		grafika_do_piona = "media/pion_czarny.png";
		kolor = CZARNY_PION;
	}

}