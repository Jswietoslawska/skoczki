#pragma once
#include <string>
#include "pole.hpp"
using namespace std;
enum kolor_piona{BIALY_PION, CZARNY_PION};
class pion
{
	friend class plansza;
	friend class gracz;
	friend class gracz_komputer;
	kolor_piona kolor;
	string grafika_do_piona;
	pole* pole_pion;
public:
	pion(pole* pol);
	void set_pole_piona(pole* pol) { pole_pion = pol; };
};