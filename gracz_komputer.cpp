#pragma once
#include "gracz_komputer.h"

void gracz_komputer::tura_gracza(ALLEGRO_MOUSE_STATE myszka, ALLEGRO_DISPLAY* okno)
{
	pl->rysuj_plansze();
	rysuj_ture();
	ALLEGRO_EVENT event;
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(okno));
	taktyka_skoku();
}

bool gracz_komputer:: taktyka_skoku()
{
	skok = false;
	//uruchomienie taktyki zakonczenia jesli jest aktywna
	if (taktyka_zakonczenie == true)
	{
		pole*temp = taktyka_zakonczenia2();
		//sprawdzenie czy taktyka zakoczenia sie powiodla
		if (temp!= NULL&&temp->pion_stoi_na_polu==NULL)
		{
			//przstawienie piona
			if (pl->przestaw_pion(wybrane_pole, nowe_pole, this) == true)
			{
				return true;
			}
		}
	}
	int dlugosc;
	//znalezienie najlepszego mozliwego ruchu i jego dlugosci
	pole* najlepsze=najdluzszy_ruch(dlugosc);
	//wyzerowanie pol odpowiedzialnych za przestawianie piona
	poprzednie_pole = NULL;
	nowe_pole = NULL;
	wybrane_pole = NULL;
	//aktualizacja wygladu planszy i zmiana informacji na temat tury
	pl->rysuj_plansze();
	rysuj_ture();
	if (dlugosc == 1 || dlugosc == -1)
	{
		//instrukcje wykonywane do przesuniecia piona
		wybrane_pole = najlepsze;
		//uzycie taktyki ostatnich ruchow
		pole* temp = ostatnie_ruchy(najlepsze);
		if (temp != NULL)
		{
			nowe_pole = temp;
		}
		else
			nowe_pole=najdluzszy_bez_skoku(najlepsze);
		if(wybrane_pole==NULL)
			wybrane_pole = najlepsze;
		//przestawienie piona
		pl->przestaw_pion(wybrane_pole, nowe_pole, this);
		//aktualizacja wygladu planszy
		pl->rysuj_plansze();
		rysuj_ture();
		return true;
	}
	else
	{
		pole* temp = NULL;
		//sprawdzenie czy takatyka pirwszych ruchow jest aktywna
		if (pierwsze_ruchy == true&&dlugosc<=2)
		{
			if (taktyka_pierwszych_ruchow() == true)
			{
				poprzednie_pole = wybrane_pole;
				wybrane_pole = nowe_pole;
				nowe_pole = NULL;
				temp = NULL;
				pl->rysuj_plansze();
				rysuj_ture();
				//znalezienie najdluzszej sekwencji skokow
				temp = najdluzszy_skok(poprzednie_pole, wybrane_pole, NULL);
			}
		}
		else
		{
			pierwsze_ruchy = false;
			wybrane_pole = najlepsze;
			temp = najdluzszy_skok(poprzednie_pole, wybrane_pole, NULL);
			nowe_pole = temp;
			//przestawienie piona
			pl->przestaw_pion(wybrane_pole, nowe_pole, this);
			poprzednie_pole = wybrane_pole;
			wybrane_pole = nowe_pole;
			nowe_pole = NULL;
			temp = NULL;
			//aktualizacja wygladu planszy
			pl->rysuj_plansze();
			rysuj_ture();
			temp = najdluzszy_skok(poprzednie_pole, wybrane_pole, NULL);
		}
		while (temp != NULL)
		{
			skok = true;
			nowe_pole = temp;
			//wykonanie skoku i sprawdzenie czy dalsze skakanie jest mozliwe
			if (!pl->skok_piona(poprzednie_pole, wybrane_pole, nowe_pole, this))
				return true;
			poprzednie_pole = wybrane_pole;
			wybrane_pole = nowe_pole;
			nowe_pole = NULL;
			temp = NULL;
			pl->rysuj_plansze();
			rysuj_ture();
			temp = najdluzszy_skok(poprzednie_pole, wybrane_pole, NULL);
		}
		return true;
	}
	return false;
}



int gracz_komputer::najdluszy_ruch_danego_piona(pole* z_pola,pole* na_polu, galaz*head)
{
	licznik_rekursji++;
	//printf("REKURSJA\n");
	int x = na_polu->numer_pola_poziom;
	int y = na_polu->numer_pola_pion;
	if (!(pl->czy_jest_mozliwy_skok(z_pola, na_polu)) && z_pola == NULL)
	{
		pole** tab = pl->mozliwosci_ruchu(na_polu);
		bool cofanie = false;
		for (int i = 0; i < 4 && tab[i] != NULL; i++)
		{
			if (y > tab[i]->numer_pola_pion)
			{
				return 1; //maksymalna dlugosc ruchu w pionie
			}
			else
			{
				cofanie = true;
			}
		}
		if (cofanie == true)
			return -1;
		return 0; //ruch niemozliwy
	}
	pole* na_pole[4];
	na_pole[0] = pl->jakie_to_pole_numery(x + 2, y + 2);
	na_pole[1] = pl->jakie_to_pole_numery(x - 2, y - 2);
	na_pole[2] = pl->jakie_to_pole_numery(x + 2, y - 2);
	na_pole[3] = pl->jakie_to_pole_numery(x - 2, y + 2);
	if (head == NULL)
	{
		licznik_rekursji = 0;
		pole_wyjsciowe = na_polu;
		//printf("NULL\n");
		suma_dlugosci.clear();
		head = new galaz;
		head->dlugosc_wzgledna_galezi = 0;
		head->na_pole = na_polu;
		head->poprz = NULL;
		head->rozgalezienie[0] = NULL;
		head->suma_dlugosci_poprz = 0;
		head->matka = true;
	}
		for(int i = 0, k=0; i < 4; i++)
		{
			//printf("head: suma dl:%d i:%d\n", head->suma_dlugosci_poprz,i);
			if (na_pole[i] != NULL&& pl->czy_jest_mozliwy_skok(z_pola, na_polu, na_pole[i]))
			{
				//printf("Na pole (%d,%d)\n", na_pole[i]->numer_pola_poziom, na_pole[i]->numer_pola_pion);
				//printf("w IF-ie head: suma dl:%d i:%d\n", head->suma_dlugosci_poprz, i);
				galaz *nowa = new galaz;
				head->rozgalezienie[k] = new galaz;
				head->rozgalezienie[k]->poprz = head;
				head->rozgalezienie[k]->suma_dlugosci_poprz += head->suma_dlugosci_poprz;
				head->rozgalezienie[k]->na_pole = na_pole[i];
				if (y > na_pole[i]->numer_pola_pion)
				{
					//printf("suma +2\n");
					head->rozgalezienie[k]->dlugosc_wzgledna_galezi = 2;
					head->rozgalezienie[k]->suma_dlugosci_poprz += 2;
				}
				else
				{
					head->rozgalezienie[k]->dlugosc_wzgledna_galezi = -2;
					head->rozgalezienie[k]->suma_dlugosci_poprz -= 2;
				}
				if (head->rozgalezienie[k]->suma_dlugosci_poprz != 0)
					suma_dlugosci.insert(head->rozgalezienie[k]->suma_dlugosci_poprz);
				//head->rozgalezienie[k] = nowa;
				if (na_pole[i] == pole_wyjsciowe||nieskonczona_rekursja==true||licznik_rekursji>=max_rekursja)
				{
					//printf("BREAK\n");
					nieskonczona_rekursja = true;
					break;
				}
				int z_rekursji= najdluszy_ruch_danego_piona(na_polu, na_pole[i], head->rozgalezienie[k]);
				//printf("\nZWROCONE Z REKURSJI:%d\n", z_rekursji);
				//head->rozgalezienie[k]->suma_dlugosci_poprz += z_rekursji;
				//printf("suma dl rozgalezienie[%d]=%d\n", k, head->rozgalezienie[k]->suma_dlugosci_poprz);
				if (head->rozgalezienie[k]->suma_dlugosci_poprz != 0)
				{
					suma_dlugosci.insert(head->rozgalezienie[k]->suma_dlugosci_poprz);
				}
				k++;
			}
		}
		if (head->matka == true)
		{
			nieskonczona_rekursja = false;
			if (suma_dlugosci.empty())
				return 0;
			if (*(suma_dlugosci.rbegin()) == -2)
			{
				pole** tab = pl->mozliwosci_ruchu(na_polu);
				bool cofanie = false;
				for (int i = 0; i < 4 && tab[i] != NULL; i++)
				{
					if (y > tab[i]->numer_pola_pion)
					{
						return 1; //maksymalna dlugosc ruchu w pionie
					}
					else
					{
						cofanie = true;
					}
				}
				if (cofanie == true)
					return -1;
				else
					return -2;
			}
			if (head->matka == true)
			{
				return *(suma_dlugosci.rbegin());
			}
			else
			{
				return head->suma_dlugosci_poprz;
			}
		}
		else
			return head->suma_dlugosci_poprz;
}


pole* gracz_komputer::najdluzszy_ruch(int& dlugosc)
{
	int i = 0;
	pole* najlepsze = NULL;
	pole* najlepsze_ruchy[ILOSC_PIONOW] = { NULL };
	for (int j = ROZMIAR_PLANSZY; j >0&&i<ILOSC_PIONOW; j--)
	{
		for (int k = ROZMIAR_PLANSZY; k >0&&i<ILOSC_PIONOW; k--)
		{

			if (pl->jakie_to_pole_numery(j, k) != NULL&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu != NULL
				&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu->kolor == kolor_pionow)
			{
				galaz*it = NULL;
				int dlugosc_ruchu = najdluszy_ruch_danego_piona(NULL, pl->jakie_to_pole_numery(j, k), it);
				printf("pion:(%d, %d) = %d", j, k, dlugosc_ruchu);
				if ((najlepsze==NULL||dlugosc < dlugosc_ruchu) && dlugosc_ruchu!=0)
				{
					dlugosc = dlugosc_ruchu;
					najlepsze = pl->jakie_to_pole_numery(j, k);
				}
				i++;
			}
		}
	}
	printf("\nnajlepsze %d, %d\n", najlepsze->numer_pola_poziom, najlepsze->numer_pola_pion);
	i = 0;
	for (int j = ROZMIAR_PLANSZY; j >0&&i<ILOSC_PIONOW; j--)
	{
		for (int k = ROZMIAR_PLANSZY; k >0&&i<ILOSC_PIONOW; k--)
		{

			if (pl->jakie_to_pole_numery(j, k) != NULL&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu != NULL
				&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu->kolor == kolor_pionow)
			{
				galaz*it = NULL;
				int dlugosc_ruchu = najdluszy_ruch_danego_piona(NULL, pl->jakie_to_pole_numery(j, k), it);
				printf("pion:(%d, %d) = %d", j, k, dlugosc_ruchu);
				if (dlugosc==dlugosc_ruchu)
				{
					najlepsze_ruchy[i] = pl->jakie_to_pole_numery(j, k);
					i++;
				}
			}
		}
	}
	najdluzszy_ruch_przeciwnika();
	if (!ruchy_przeciwnika.empty())
	{
		set<set<pole*>>::iterator IT = ruchy_przeciwnika.begin();
		set<pole*>::iterator it = (*IT).begin();
		while (IT != ruchy_przeciwnika.end())
		{
			it = (*IT).begin();
			for (int j = 0; j < i; j++)
			{
				if (najlepsze_ruchy[j] != NULL)
				{
					pole* docelowe = docelowe_pole(najlepsze_ruchy[j]);
					if (*it == docelowe&&najlepsze_ruchy[j] != NULL&&docelowe!=NULL)
					{
						printf("NAJLEPSZY RUCH!\n\n");
						return najlepsze_ruchy[j];
					}
				}
			}
			printf("TUTAJ\n\n");
			IT++;
		}
		/*IT = ruchy_przeciwnika.begin();
		it = (*IT).begin();
		while (IT != ruchy_przeciwnika.end())
		{
			it++;
			while (it != (*IT).end())
			{

				for (int j = 0; j < i; j++)
				{
					pole* docelowe = docelowe_pole(najlepsze_ruchy[j]);
					if (*it == docelowe&&najlepsze_ruchy[j]!=NULL)
					{
						return najlepsze_ruchy[j];
					}
				}
				it++;
			}
			IT++;
		}*/
	}
	return najlepsze;
}


pole* gracz_komputer::najdluzszy_skok(pole* z_pola, pole* na_polu, galaz* head)
{
	int x = na_polu->numer_pola_poziom;
	int y = na_polu->numer_pola_pion;
	printf("x,y: (%d, %d)\n", x, y);
	if (!(pl->czy_jest_mozliwy_skok(z_pola, na_polu)))
	{
		//printf("UPSSS!\n");
		return NULL;
	}
	pole* na_pole[4];
	na_pole[0] = pl->jakie_to_pole_numery(x + 2, y + 2);
	na_pole[1] = pl->jakie_to_pole_numery(x - 2, y - 2);
	na_pole[2] = pl->jakie_to_pole_numery(x + 2, y - 2);
	na_pole[3] = pl->jakie_to_pole_numery(x - 2, y + 2);
	int dlugosc_ruchu[4];
	if (head == NULL)
	{
		suma_dlugosci.clear();
		head = new galaz;
		head->dlugosc_wzgledna_galezi = 0;
		head->na_pole = na_polu;
		head->poprz = NULL;
		head->rozgalezienie[0] = NULL;
		head->suma_dlugosci_poprz = 0;
		head->matka = true;
	}
	for (int i = 0, k = 0; i < 4; i++)
	{
		if (na_pole[i] != NULL&&na_pole[i]!=z_pola&& pl->czy_jest_mozliwy_skok(z_pola, na_polu, na_pole[i]))
		{
			galaz *nowa = new galaz;
			nowa->poprz = head;
			nowa->na_pole = na_pole[i];
			if (y > na_pole[i]->numer_pola_pion)
			{
				nowa->dlugosc_wzgledna_galezi = 2;
				nowa->suma_dlugosci_poprz = head->suma_dlugosci_poprz + 2;
				suma_dlugosci.insert(nowa->suma_dlugosci_poprz);
			}
			else
			{
				nowa->dlugosc_wzgledna_galezi = -2;
				nowa->suma_dlugosci_poprz = head->suma_dlugosci_poprz - 2;
				suma_dlugosci.insert(nowa->suma_dlugosci_poprz);
			}
			head->rozgalezienie[k] = nowa;
			najdluszy_ruch_danego_piona(na_polu, na_pole[i], head->rozgalezienie[k]);
			k++;
		}
		if (!suma_dlugosci.empty())
		{
			dlugosc_ruchu[i] = *(suma_dlugosci.rbegin());
			suma_dlugosci.clear();
		}
		else
		{
			dlugosc_ruchu[i] = 0;
		}
	}
	int index_max = 0;
	int max = -2;
	for (int i =0;i<4; i++)
	{
		if (dlugosc_ruchu[i] >= max&&dlugosc_ruchu[i]!=0)
		{
			index_max = i;
			max = dlugosc_ruchu[i];
		}
	}
	printf("dl_ruchu[%d]=%d", index_max, max);
	if (dlugosc_ruchu[index_max] == -2 && skok)
	{
		//printf("UPSIK!\n");
		return NULL;
	}
	//if (max == 0||taktyka_zakonczenie==true)
	//{
	//	pole* zak = taktyka_zakonczenia();
	//	if (zak != NULL)
	//		return zak;
	//}
	return na_pole[index_max];
}

pole* gracz_komputer::najdluzszy_bez_skoku(pole* na_polu)
{
	int x = na_polu->numer_pola_poziom;
	int y = na_polu->numer_pola_pion;
	pole** tab = pl->mozliwosci_ruchu(na_polu);
	for (int i = 0; i < 4 && tab[i] != NULL; i++)
	{
		if (y > tab[i]->numer_pola_pion)
		{
			//printf("Kroczek w przod\n");
			return tab[i];
		}
	}
	pole* cofanie = NULL;
	if (tab[0] != NULL)   // W PRZYSZLOSCI OKNO DO UZYCIA INNEJ TAKTYKI
	{
		cofanie = tab[0];
	}
	//if ( taktyka_zakonczenie == true)
	//{
		pole* zak = taktyka_zakonczenia2();
		if (zak != NULL&&zak->pion_stoi_na_polu==NULL)
		{
			return zak;
		}
	//}
		if (cofanie->pion_stoi_na_polu == NULL)
			return cofanie;
		else
			return najdluzszy_skok(NULL, na_polu, NULL);
}

pole* gracz_komputer::taktyka_zakonczenia()
{
	printf("TAKTYKA ZAKONCZENIA\n\n");
	if (faza_zablokowania)
	{
		printf("FAZA ZABLOKOWANA\n");
		//Sprawdzenie ile pionow nie zajelo jeszcze swoich miejsc
		pole* wolne[8];
		int i = 0;
		for (int j = 1; j <= ROZMIAR_PLANSZY&&i < 8; j++)
		{
			for (int k = 2; k <= 2 && i < ILOSC_PIONOW; k++)
			{
				printf("Taktyka zakonczenia (%d,%d)\n", j,k);
				if (pl->jakie_to_pole_numery(j, k) != NULL&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu != NULL
					&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu->kolor != kolor_pionow)
				//pion przeciwnika posrod pionow komputera
				{
					taktyka_zakonczenia_false();
					return NULL;
				}
				if (pl->jakie_to_pole_numery(j, k) != NULL&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu == NULL
					&&pl->jakie_to_pole_numery(j, k)->kolor == CZARNE_POLE)
				{
					wolne[i] = pl->jakie_to_pole_numery(j, k);
					i++;
				}
			}
		}
		int ilosc_pionow_nie_na_miejscu = i;
		for (; i < 8; i++)
			wolne[i] = NULL;
		//przeszukiwanie poprzedniego wiersza
		pole* piony[8];
		i = 0;
		for (int j = 1, k = 3; j <= ROZMIAR_PLANSZY && i < ilosc_pionow_nie_na_miejscu; j++)
		{
			if (pl->jakie_to_pole_numery(j, k) != NULL&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu != NULL
				&&pl->jakie_to_pole_numery(j, k)->kolor == kolor_pionow)
			{
				piony[i] = pl->jakie_to_pole_numery(j, k);
				i++;
			}
		}
		for (int j=i; j < 8; j++)
			piony[j] = NULL;
		if (i == 0||ilosc_pionow_nie_na_miejscu==0)
		{
			taktyka_zakonczenia_false();
			return NULL;
		}
		if (ilosc_pionow_nie_na_miejscu != i)
		{
			taktyka_zakonczenia_false();
			return NULL;//brak sytuacji zablokowania
		}
		else if (ilosc_pionow_nie_na_miejscu == 0)
		{
			taktyka_zakonczenia_false();
			return NULL; //piony przeciwnika pomiedzy pionami
		}
		else
			taktyka_zakonczenie = true;
		printf("TUATAJ ZAKONCZENIA TAKTYKA\n");
		//ostatni pion
		/*if ((zakonczenie_w_lewo||zakonczenie_w_prawo)&&piony[0]!=NULL&&piony[1]==NULL)
		{
			int x = piony[0]->numer_pola_poziom;
			int y = piony[0]->numer_pola_pion;
				if (pl->jakie_to_pole_numery(x + 1, y - 1) != NULL&&pl->jakie_to_pole_numery(x + 1, y - 1)->pion_stoi_na_polu == NULL)
				{
					wybrane_pole = piony[0];
					nowe_pole = pl->jakie_to_pole_numery(x + 1, y - 1);
					if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
					{
						taktyka_zakonczenia_false();
						return NULL;
					}
					else
						return nowe_pole;
				}
				if (pl->jakie_to_pole_numery(x - 1, y - 1) != NULL&&pl->jakie_to_pole_numery(x - 1, y - 1)->pion_stoi_na_polu == NULL)
				{
					wybrane_pole = piony[0];
					nowe_pole = pl->jakie_to_pole_numery(x - 1, y - 1);
					if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
					{
						taktyka_zakonczenia_false();
						return NULL;
					}
					else
						return nowe_pole;
				}
		}*/
			//znalezenie piona, ktory jest "pomiedzy" zablokowanym pionem, a wolnym miejscem
			if (wolne[0]->numer_pola_poziom > piony[0]->numer_pola_poziom)//pion po lewej od wolnego pola
			{
				zakonczenie_w_lewo = true;
				int x = wolne[0]->numer_pola_poziom;
				int y = wolne[0]->numer_pola_pion;
				while (pl->jakie_to_pole_numery(x - 2, y) != NULL&&pl->jakie_to_pole_numery(x - 2, y)->pion_stoi_na_polu == NULL)
					x--;
				wybrane_pole = pl->jakie_to_pole_numery(x - 2, y);
				faza_zablokowania = false;
				nowe_pole = pl->jakie_to_pole_numery(x-1, y + 1);
				if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
				{
					taktyka_zakonczenia_false();
					return NULL;
				}
				else
					return nowe_pole;
			}
			else
			{
				zakonczenie_w_prawo = true;
				int x = wolne[0]->numer_pola_poziom;
				int y = wolne[0]->numer_pola_pion;
				while (pl->jakie_to_pole_numery(x + 2, y) != NULL&&pl->jakie_to_pole_numery(x + 2, y)->pion_stoi_na_polu == NULL)
					x++;
				wybrane_pole = pl->jakie_to_pole_numery(x + 2, y);
				faza_zablokowania = false;
				nowe_pole = pl->jakie_to_pole_numery(x+1, y + 1);
				if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
				{
					taktyka_zakonczenia_false();
					return NULL;
				}
				else
					return nowe_pole;
			}
	}
	else
	{
		if (zakonczenie_w_lewo)
		{
			for (int i = ROZMIAR_PLANSZY; i >0; i--)
			{
				if (pl->jakie_to_pole_numery(i, 3) != NULL&&pl->jakie_to_pole_numery(i, 3)->pion_stoi_na_polu!=NULL
					&&pl->jakie_to_pole_numery(i+1,2)!=NULL&&
					pl->jakie_to_pole_numery(i+1,2)->pion_stoi_na_polu==NULL)
				{
					faza_zablokowania = true;
					wybrane_pole = pl->jakie_to_pole_numery(i, 3);
					nowe_pole = pl->jakie_to_pole_numery(i + 1, 2);
					if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
					{
						taktyka_zakonczenia_false();
						return NULL;
					}
					else
						return nowe_pole;
				}
			}
		}
		else if (zakonczenie_w_prawo)
		{
			for (int i = 1; i <=ROZMIAR_PLANSZY; i++)
			{
				if (pl->jakie_to_pole_numery(i, 3) != NULL&&pl->jakie_to_pole_numery(i, 3)->pion_stoi_na_polu != NULL
					&&pl->jakie_to_pole_numery(i - 1, 2) != NULL&&
					pl->jakie_to_pole_numery(i - 1, 2)->pion_stoi_na_polu == NULL)
				{
					faza_zablokowania = true;
					wybrane_pole = pl->jakie_to_pole_numery(i, 3);
					nowe_pole = pl->jakie_to_pole_numery(i - 1, 2);
					if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
					{
						taktyka_zakonczenia_false();
						return NULL;
					}
					else
						return nowe_pole;
				}
			}
		}
	}
	taktyka_zakonczenia_false();
	return NULL;
}

pole* gracz_komputer::docelowe_pole(pole* najlepsze)
{
	printf("DOCELOWE POLE\n");
	galaz*it = NULL;
	int dlugosc = najdluszy_ruch_danego_piona(NULL, najlepsze, it);
	if (dlugosc == 0)
		return NULL;
	if (dlugosc != 1 && dlugosc != -1)
	{
		pole* temp = NULL;
		pole* wybrane = najlepsze;
		pole*poprzednie = NULL;
		temp = najdluzszy_skok(poprzednie, wybrane, NULL);
		pole* nowe = temp;
		poprzednie = wybrane;
		wybrane = nowe;
		nowe = NULL;
		temp = NULL;
		temp = najdluzszy_skok(poprzednie, wybrane, NULL);
		while (temp != NULL)
		{
			skok = true;
			nowe = temp;
			if (!pl->czy_jest_mozliwy_skok(poprzednie, wybrane, nowe))
				return wybrane;
			poprzednie = wybrane;
			wybrane = nowe;
			nowe = NULL;
			temp = NULL;
			temp = najdluzszy_skok(poprzednie, wybrane, NULL);
		}
		return wybrane;
	}
	else if (dlugosc != 0)
	{
		int x = najlepsze->numer_pola_poziom;
		int y = najlepsze->numer_pola_pion;
		pole** tab = pl->mozliwosci_ruchu(najlepsze);
		for (int i = 0; i < 4 && tab[i] != NULL; i++)
		{
			if (y > tab[i]->numer_pola_pion)
			{
				return tab[i];
			}
		}
		if (tab[0] != NULL)
		{
			return tab[0];
		}
	}
	else
		return NULL;
}

int gracz_komputer::najdluzszy_ruch_danego_piona_przeciwnika(pole* z_pola, pole* na_polu, galaz*head)
{
	printf("NAJDLUZSZY DANEGO PIONA PRZECIWNIKA\n");
	licznik_rekursji++;
	//printf("REKURSJA\n");
	int x = na_polu->numer_pola_poziom;
	int y = na_polu->numer_pola_pion;
	if (!(pl->czy_jest_mozliwy_skok(z_pola, na_polu)) && z_pola == NULL)
	{
		pole** tab = pl->mozliwosci_ruchu(na_polu);
		bool cofanie = false;
		for (int i = 0; i < 4 && tab[i] != NULL; i++)
		{
			if (y < tab[i]->numer_pola_pion)
			{
				return 1; //maksymalna dlugosc ruchu w pionie
			}
			else
			{
				cofanie = true;
			}
		}
		if (cofanie == true)
			return -1;
		return 0; //ruch niemozliwy
	}
	pole* na_pole[4];
	na_pole[0] = pl->jakie_to_pole_numery(x + 2, y + 2);
	na_pole[1] = pl->jakie_to_pole_numery(x - 2, y - 2);
	na_pole[2] = pl->jakie_to_pole_numery(x + 2, y - 2);
	na_pole[3] = pl->jakie_to_pole_numery(x - 2, y + 2);
	if (head == NULL)
	{
		licznik_rekursji = 0;
		pole_wyjsciowe = na_polu;
		//printf("NULL\n");
		suma_dlugosci.clear();
		head = new galaz;
		head->dlugosc_wzgledna_galezi = 0;
		head->na_pole = na_polu;
		head->poprz = NULL;
		head->rozgalezienie[0] = NULL;
		head->suma_dlugosci_poprz = 0;
		head->matka = true;
	}
	for (int i = 0, k = 0; i < 4; i++)
	{
		//printf("head: suma dl:%d i:%d\n", head->suma_dlugosci_poprz,i);
		if (na_pole[i] != NULL&& pl->czy_jest_mozliwy_skok(z_pola, na_polu, na_pole[i]))
		{
			//printf("Na pole (%d,%d)\n", na_pole[i]->numer_pola_poziom, na_pole[i]->numer_pola_pion);
			//printf("w IF-ie head: suma dl:%d i:%d\n", head->suma_dlugosci_poprz, i);
			galaz *nowa = new galaz;
			head->rozgalezienie[k] = new galaz;
			head->rozgalezienie[k]->poprz = head;
			head->rozgalezienie[k]->suma_dlugosci_poprz += head->suma_dlugosci_poprz;
			head->rozgalezienie[k]->na_pole = na_pole[i];
			if (y > na_pole[i]->numer_pola_pion)
			{
				//printf("suma +2\n");
				head->rozgalezienie[k]->dlugosc_wzgledna_galezi = -2;
				head->rozgalezienie[k]->suma_dlugosci_poprz -= 2;
			}
			else
			{
				head->rozgalezienie[k]->dlugosc_wzgledna_galezi = 2;
				head->rozgalezienie[k]->suma_dlugosci_poprz += 2;
			}
			if (head->rozgalezienie[k]->suma_dlugosci_poprz != 0)
				suma_dlugosci.insert(head->rozgalezienie[k]->suma_dlugosci_poprz);
			//head->rozgalezienie[k] = nowa;
			if (na_pole[i] == pole_wyjsciowe || nieskonczona_rekursja == true || licznik_rekursji >= max_rekursja)
			{
				//printf("BREAK\n");
				nieskonczona_rekursja = true;
				break;
			}
			int z_rekursji = najdluzszy_ruch_danego_piona_przeciwnika(na_polu, na_pole[i], head->rozgalezienie[k]);
			//printf("\nZWROCONE Z REKURSJI:%d\n", z_rekursji);
			//head->rozgalezienie[k]->suma_dlugosci_poprz += z_rekursji;
			//printf("suma dl rozgalezienie[%d]=%d\n", k, head->rozgalezienie[k]->suma_dlugosci_poprz);
			if (head->rozgalezienie[k]->suma_dlugosci_poprz != 0)
			{
				suma_dlugosci.insert(head->rozgalezienie[k]->suma_dlugosci_poprz);
			}
			k++;
		}
	}
	if (head->matka == true)
	{
		nieskonczona_rekursja = false;
		if (suma_dlugosci.empty())
			return 0;
		if (*(suma_dlugosci.rbegin()) == -2)
		{
			printf("DLUGOSC -2\n");
			pole** tab = pl->mozliwosci_ruchu(na_polu);
			bool cofanie = false;
			for (int i = 0; i < 4 && tab[i] != NULL; i++)
			{
				if (y < tab[i]->numer_pola_pion)
				{
					return 1; //maksymalna dlugosc ruchu w pionie
				}
				else
				{
					cofanie = true;
				}
			}
			if (cofanie == true)
				return -1;
			else
				return -2;
		}
		if (head->matka == true)
		{
			return *(suma_dlugosci.rbegin());
		}
		else
		{
			return head->suma_dlugosci_poprz;
		}
	}
	else
		return head->suma_dlugosci_poprz;
}

void gracz_komputer::najdluzszy_ruch_przeciwnika()
{
	printf("NAJDLUZSZY RUCH PRZECIWNIKA\n");
	int i = 0;
	int dlugosci[ILOSC_PIONOW] = { 0 };
	pole* najlepsze_ruchy[ILOSC_PIONOW] = { NULL };
	ruchy_przeciwnika.clear();
	for (int j = 1; j <= ROZMIAR_PLANSZY&&i<ILOSC_PIONOW; j++)
	{
		for (int k = 1; k <= ROZMIAR_PLANSZY&&i<ILOSC_PIONOW; k++)
		{
			if (pl->jakie_to_pole_numery(j, k) != NULL&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu != NULL
				&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu->kolor != kolor_pionow)
			{
				galaz*it = NULL;
				int dlugosc_ruchu = najdluzszy_ruch_danego_piona_przeciwnika(NULL, pl->jakie_to_pole_numery(j, k), it);
				printf("pion:(%d, %d) = %d", j, k, dlugosc_ruchu);
				if (dlugosc_ruchu>1)
				{
					dlugosci[i] = dlugosc_ruchu;
					najlepsze_ruchy[i] = pl->jakie_to_pole_numery(j, k);
					i++;
				}
			}
		}
	}
	bool posortowane = false;
	while (!posortowane)
	{
		posortowane = true;
		for (int j = 1; j < i; j++)
		{
			if (dlugosci[j - 1] < dlugosci[j])
			{
				posortowane = false;
				int temp = dlugosci[j - 1];
				dlugosci[j - 1] = dlugosci[j];
				dlugosci[j] = temp;
				pole* pol = najlepsze_ruchy[j - 1];
				najlepsze_ruchy[j - 1] = najlepsze_ruchy[j];
				najlepsze_ruchy[j] = pol;
			}
		}
	}
	for (int j = 0; j < i; j++)
	{
		pole** trasa = trasa_piona_przeciwnika(najlepsze_ruchy[j]);
		set<pole*> temp;
		int k = 0;
		if (trasa == NULL)
			break;
		while (trasa[k] != NULL)
		{
			temp.insert(trasa[k]);
			k++;
		}
		if(!temp.empty())
		ruchy_przeciwnika.insert(temp);
	}
}

pole* gracz_komputer::najdluzszy_skok_przeciwnika(pole* z_pola, pole* na_polu, galaz* head)
{
	printf("NAJLDUZSZY SKOK PRZECIWNIKA\n");
	int x = na_polu->numer_pola_poziom;
	int y = na_polu->numer_pola_pion;
	printf("x,y: (%d, %d)\n", x, y);
	if (!(pl->czy_jest_mozliwy_skok(z_pola, na_polu)))
	{
		//printf("UPSSS!\n");
		return NULL;
	}
	pole* na_pole[4];
	na_pole[0] = pl->jakie_to_pole_numery(x + 2, y + 2);
	na_pole[1] = pl->jakie_to_pole_numery(x - 2, y - 2);
	na_pole[2] = pl->jakie_to_pole_numery(x + 2, y - 2);
	na_pole[3] = pl->jakie_to_pole_numery(x - 2, y + 2);
	int dlugosc_ruchu[4];
	if (head == NULL)
	{
		suma_dlugosci.clear();
		head = new galaz;
		head->dlugosc_wzgledna_galezi = 0;
		head->na_pole = na_polu;
		head->poprz = NULL;
		head->rozgalezienie[0] = NULL;
		head->suma_dlugosci_poprz = 0;
		head->matka = true;
	}
	for (int i = 0, k = 0; i < 4; i++)
	{
		if (na_pole[i] != NULL&&na_pole[i] != z_pola&& pl->czy_jest_mozliwy_skok(z_pola, na_polu, na_pole[i]))
		{
			galaz *nowa = new galaz;
			nowa->poprz = head;
			nowa->na_pole = na_pole[i];
			if (y < na_pole[i]->numer_pola_pion)
			{
				nowa->dlugosc_wzgledna_galezi = 2;
				nowa->suma_dlugosci_poprz = head->suma_dlugosci_poprz + 2;
				suma_dlugosci.insert(nowa->suma_dlugosci_poprz);
			}
			else
			{
				nowa->dlugosc_wzgledna_galezi = -2;
				nowa->suma_dlugosci_poprz = head->suma_dlugosci_poprz - 2;
				suma_dlugosci.insert(nowa->suma_dlugosci_poprz);
			}
			head->rozgalezienie[k] = nowa;
			najdluzszy_ruch_danego_piona_przeciwnika(na_polu, na_pole[i], head->rozgalezienie[k]);
			k++;
		}
		if (!suma_dlugosci.empty())
		{
			dlugosc_ruchu[i] = *(suma_dlugosci.rbegin());
			suma_dlugosci.clear();
		}
		else
		{
			dlugosc_ruchu[i] = 0;
		}
	}
	int index_max = 0;
	int max = -2;
	for (int i = 0; i<4; i++)
	{
		if (dlugosc_ruchu[i] >= max&&dlugosc_ruchu[i]!=0)
		{
			index_max = i;
			max = dlugosc_ruchu[i];
		}
	}
	printf("dl_ruchu[%d]=%d", index_max, max);
	if ((dlugosc_ruchu[index_max] == -2 && skok)||dlugosc_ruchu[index_max]==0)
	{
		//printf("UPSIK!\n");
		return NULL;
	}
	//if (max == 0||taktyka_zakonczenie==true)
	//{
	//	pole* zak = taktyka_zakonczenia();
	//	if (zak != NULL)
	//		return zak;
	//}
	return na_pole[index_max];
}

pole** gracz_komputer::trasa_piona_przeciwnika(pole* najlepsze)
{
	skok = false;
	printf("TRASA PRZECIWNIKA\n");
	int dlugosc = najdluzszy_ruch_danego_piona_przeciwnika(NULL,najlepsze, NULL);
	if (dlugosc == 0)
		return NULL;
	if (dlugosc != 1 && dlugosc != -1)
	{
		int i = 0;
		trasa[ROZMIAR_PLANSZY * 2] = { NULL };
		pole* temp = NULL;
		pole* wybrane = najlepsze;
		pole*poprzednie = NULL;
		temp = najdluzszy_skok_przeciwnika(poprzednie, wybrane, NULL);
		pole* nowe = temp;
		if (nowe != NULL)
		{
			trasa[i] = nowe;
			i++;
		}
		poprzednie = wybrane;
		wybrane = nowe;
		nowe = NULL;
		temp = NULL;
		temp = najdluzszy_skok_przeciwnika(poprzednie, wybrane, NULL);
		while (temp != NULL)
		{
			skok = true;
			if (temp != NULL)
			{
				trasa[i] = nowe;
				i++;
			}
			nowe = temp;
			if (!pl->czy_jest_mozliwy_skok(poprzednie, wybrane, nowe))
			{
				return trasa;
			}
			printf("czy mozliwy skok z %d, %d\n", nowe->numer_pola_poziom, nowe->numer_pola_pion);
			poprzednie = wybrane;
			wybrane = nowe;
			nowe = NULL;
			temp = NULL;
			temp = najdluzszy_skok_przeciwnika(poprzednie, wybrane, NULL);
		}
		return trasa;
	}
	else if (dlugosc != 0)
	{
		int x = najlepsze->numer_pola_poziom;
		int y = najlepsze->numer_pola_pion;
		pole** tab = pl->mozliwosci_ruchu(najlepsze);
		pole* tablica[1] = { NULL };
		for (int i = 0; i < 4 && tab[i] != NULL; i++)
		{
			if (y < tab[i]->numer_pola_pion)
			{
				tablica[0] = tab[i];
				return tablica;
			}
		}
		if (tab[0] != NULL)
		{
			tablica[0] = tab[0];
			return tablica;
		}
	}
	else
		return NULL;
}

void gracz_komputer::taktyka_zakonczenia_false()
{
	printf("Taktyka zakonczenia false\n");
	//nowe_pole = NULL;
	//wybrane_pole = NULL;
	zakonczenie_w_lewo = false;
	zakonczenie_w_prawo = false;
	taktyka_zakonczenie = false;
	faza_zablokowania = true;
}

bool gracz_komputer::taktyka_pierwszych_ruchow()
{
	if (pl->jakie_to_pole_numery(8, 8)->pion_stoi_na_polu != NULL&&pl->jakie_to_pole_numery(8, 8)->pion_stoi_na_polu->kolor==kolor_pionow
		&&pl->jakie_to_pole_numery(6, 6)->pion_stoi_na_polu == NULL
		&&pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(8,8),pl->jakie_to_pole_numery(6,6)))
	{
		pl->przestaw_pion(wybrane_pole=pl->jakie_to_pole_numery(8, 8), nowe_pole=pl->jakie_to_pole_numery(6, 6), this);
		return true;
	}
	if (pl->jakie_to_pole_numery(4, 8)->pion_stoi_na_polu != NULL&&pl->jakie_to_pole_numery(4, 8)->pion_stoi_na_polu->kolor==kolor_pionow
		&&pl->jakie_to_pole_numery(2, 6)->pion_stoi_na_polu == NULL
		&&pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(4, 8), pl->jakie_to_pole_numery(2, 6)))
	{
		pl->przestaw_pion(wybrane_pole=pl->jakie_to_pole_numery(4, 8), nowe_pole=pl->jakie_to_pole_numery(2, 6), this);
		return true;
	}
	if (pl->jakie_to_pole_numery(6, 8)->pion_stoi_na_polu != NULL&&pl->jakie_to_pole_numery(6, 8)->pion_stoi_na_polu->kolor == kolor_pionow
		&&pl->jakie_to_pole_numery(8, 6)->pion_stoi_na_polu == NULL
		&&pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(6, 8), pl->jakie_to_pole_numery(8, 6)))
	{
		pl->przestaw_pion(wybrane_pole=pl->jakie_to_pole_numery(6, 8),nowe_pole= pl->jakie_to_pole_numery(8, 6), this);
		return true;
	}
	if (pl->jakie_to_pole_numery(2, 8)->pion_stoi_na_polu != NULL&&pl->jakie_to_pole_numery(2, 8)->pion_stoi_na_polu->kolor == kolor_pionow
		&&pl->jakie_to_pole_numery(4, 6)->pion_stoi_na_polu == NULL
		&&pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(2, 8), pl->jakie_to_pole_numery(4, 6)))
	{
		pl->przestaw_pion(wybrane_pole=pl->jakie_to_pole_numery(2, 8), nowe_pole =pl->jakie_to_pole_numery(4, 6), this);
		return true;
	}
	if (pl->jakie_to_pole_numery(1, 7)->pion_stoi_na_polu != NULL&&pl->jakie_to_pole_numery(1, 7)->pion_stoi_na_polu->kolor == kolor_pionow
		&&pl->jakie_to_pole_numery(3, 5)->pion_stoi_na_polu == NULL
		&&pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(1, 7), pl->jakie_to_pole_numery(3, 5)))
	{
		pl->przestaw_pion(wybrane_pole=pl->jakie_to_pole_numery(1, 7),nowe_pole= pl->jakie_to_pole_numery(3, 5), this);
		return true;
	}
	if (pl->jakie_to_pole_numery(7, 7)->pion_stoi_na_polu != NULL&&pl->jakie_to_pole_numery(7, 7)->pion_stoi_na_polu->kolor == kolor_pionow
		&&pl->jakie_to_pole_numery(5, 5)->pion_stoi_na_polu == NULL
		&&pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(7, 7), pl->jakie_to_pole_numery(5, 5)))
	{
		pl->przestaw_pion(wybrane_pole=pl->jakie_to_pole_numery(7, 7), nowe_pole= pl->jakie_to_pole_numery(5, 5), this);
		return true;
	}
	pierwsze_ruchy = false;
	return false;
}

pole* gracz_komputer::ostatnie_ruchy(pole* najlepsze)
{
	printf("\nOSTATNIE RUCHY\n\n");
	const int ostatnie = 2;
	//jesli zostana dwa ostatnie piony, ktore nie sa ustawione
	int licznik = 0;
	pole* wolne_pola[ostatnie];
	int k = 0;
	for (int i = 1; i <= ROZMIAR_PLANSZY; i++)
	{
		for (int j = 1; j <= 2; j++)
		{
			if (pl->jakie_to_pole_numery(i, j)->pion_stoi_na_polu == NULL&&pl->jakie_to_pole_numery(i,j)->kolor==CZARNE_POLE)
			{
				licznik++;
			}
			if (pl->jakie_to_pole_numery(i, j)->pion_stoi_na_polu == NULL&&k < ostatnie&&pl->jakie_to_pole_numery(i, j)->kolor == CZARNE_POLE)
			{
				wolne_pola[k] = pl->jakie_to_pole_numery(i, j);
				k++;
			}
		}
	}
	if (licznik >k+2)
	{
		printf("licznik!=k k:%d, licz: %d\n", k, licznik);
		return NULL;
	}
	int x = najlepsze->numer_pola_poziom;
	int y = najlepsze->numer_pola_pion;
	if (k == 1)
	{
		printf("\nJEDNO WOLNE POLE\n");
		if (wolne_pola[0]->numer_pola_poziom > x)
		{
			if (pl->jakie_to_pole_numery(x + 1, y - 1)!=NULL&&pl->jakie_to_pole_numery(x + 1, y - 1)->pion_stoi_na_polu == NULL)
				return pl->jakie_to_pole_numery(x + 1, y - 1);
		}
		else
		{
			if (pl->jakie_to_pole_numery(x - 1, y - 1)!=NULL&&pl->jakie_to_pole_numery(x - 1, y - 1)->pion_stoi_na_polu == NULL)
				return pl->jakie_to_pole_numery(x - 1, y - 1);
		}
	}
	else if (k == 2)
	{
		printf("\nDWA WOLNE POLA\n");
		if (std::abs(x - wolne_pola[0]->numer_pola_poziom) <= std::abs((x - wolne_pola[1]->numer_pola_poziom)))
		{
			if (wolne_pola[0]->numer_pola_poziom > x)
			{
				if (pl->jakie_to_pole_numery(x + 1, y - 1) != NULL&&pl->jakie_to_pole_numery(x + 1, y - 1)->pion_stoi_na_polu == NULL)
					return pl->jakie_to_pole_numery(x + 1, y - 1);
			}
			else
			{
				if (pl->jakie_to_pole_numery(x - 1, y - 1) != NULL&&pl->jakie_to_pole_numery(x - 1, y - 1)->pion_stoi_na_polu == NULL)
					return pl->jakie_to_pole_numery(x - 1, y - 1);
			}
		}
		else
		{
			if (wolne_pola[1]->numer_pola_poziom > x)
			{
				if (pl->jakie_to_pole_numery(x + 1, y - 1) != NULL&&pl->jakie_to_pole_numery(x + 1, y - 1)->pion_stoi_na_polu == NULL)
					return pl->jakie_to_pole_numery(x + 1, y - 1);
			}
			else
			{
				if (pl->jakie_to_pole_numery(x - 1, y - 1) != NULL&&pl->jakie_to_pole_numery(x - 1, y - 1)->pion_stoi_na_polu == NULL)
					return pl->jakie_to_pole_numery(x - 1, y - 1);
			}
		}
	}
	else
	{
		return NULL;
	}
	return NULL;
}

pole* gracz_komputer::taktyka_zakonczenia2()
{
	printf("TAKTYKA ZAKONCZENIA2\n\n");
	if (faza_zablokowania)
	{
		printf("FAZA ZABLOKOWANA\n");
		//Sprawdzenie ile pionow nie zajelo jeszcze swoich miejsc
		pole* wolne[8];
		int i = 0;
		for (int j = 1; j <= ROZMIAR_PLANSZY&&i < 8; j++)
		{
			for (int k = 1; k <= 2 && i < ILOSC_PIONOW; k++)
			{
				printf("Taktyka zakonczenia (%d,%d)\n", j, k);
				if (pl->jakie_to_pole_numery(j, k) != NULL&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu != NULL
					&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu->kolor != kolor_pionow)
					//pion przeciwnika posrod pionow komputera
				{

					//////////////////////////////////////////////////////////////////////////////////
					if ((j == 1 && k == 1 && pl->jakie_to_pole_numery(j + 2, k + 2)->pion_stoi_na_polu != NULL&&
						pl->jakie_to_pole_numery(j + 1, k + 1)->pion_stoi_na_polu != NULL&&
						pl->jakie_to_pole_numery(j + 1, k + 1)->kolor == kolor_pionow
						&&pl->jakie_to_pole_numery(j + 2, k + 2)->kolor == kolor_pionow))
					{
						pole* temp = NULL;
						if (pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(j + 1, k + 1), pl->jakie_to_pole_numery(j + 3, k + 3)))
						{
							temp = pl->jakie_to_pole_numery(j + 3, k + 3);
						}
						taktyka_zakonczenia_false();
						if (temp == NULL || temp->pion_stoi_na_polu != NULL)
							return NULL;
						wybrane_pole = pl->jakie_to_pole_numery(j + 1, k + 1);
						return nowe_pole=temp;
					}
					////////////////////////////////////////////////////////////////////////////////////
					else if ((j == ROZMIAR_PLANSZY && k == ROZMIAR_PLANSZY && pl->jakie_to_pole_numery(j - 2, k + 2)->pion_stoi_na_polu != NULL&&
						pl->jakie_to_pole_numery(j - 1, k + 1)->pion_stoi_na_polu != NULL&&
						pl->jakie_to_pole_numery(j - 1, k + 1)->kolor == kolor_pionow
						&&pl->jakie_to_pole_numery(j - 2, k + 2)->kolor == kolor_pionow))
					{
						pole* temp = NULL;
						if (pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(j - 1, k + 1), pl->jakie_to_pole_numery(j - 3, k + 3)))
						{
							temp = pl->jakie_to_pole_numery(j - 3, k + 3);
						}
						taktyka_zakonczenia_false();
						if (temp == NULL || temp->pion_stoi_na_polu != NULL)
							return NULL;
						wybrane_pole = pl->jakie_to_pole_numery(j - 1, k + 1);
						return nowe_pole=temp;
					}
					taktyka_zakonczenia_false();
					return NULL;
				}
				if (pl->jakie_to_pole_numery(j, k) != NULL&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu == NULL
					&&pl->jakie_to_pole_numery(j, k)->kolor == CZARNE_POLE)
				{
					wolne[i] = pl->jakie_to_pole_numery(j, k);
					i++;
				}
			}
		}
		int ilosc_pionow_nie_na_miejscu = i;
		if (ilosc_pionow_nie_na_miejscu <= 0 || ilosc_pionow_nie_na_miejscu > 2)
		{
			taktyka_zakonczenia_false();
			return NULL;
		}
		for (; i < 8; i++)
			wolne[i] = NULL;
		//przeszukiwanie poprzedniego wiersza
		pole* piony[8];
		i = 0;
		for (int j = 1, k = 3; j <= ROZMIAR_PLANSZY && i < ilosc_pionow_nie_na_miejscu; j++)
		{
			if (pl->jakie_to_pole_numery(j, k) != NULL&&pl->jakie_to_pole_numery(j, k)->pion_stoi_na_polu != NULL
				&&pl->jakie_to_pole_numery(j, k)->kolor == kolor_pionow)
			{
				piony[i] = pl->jakie_to_pole_numery(j, k);
				i++;
			}
		}
		for (int j = i; j < 8; j++)
			piony[j] = NULL;
		if (i == 0)
		{
			taktyka_zakonczenia_false();
			return NULL;
		}
		//if (ilosc_pionow_nie_na_miejscu != i)
		//{
		//	taktyka_zakonczenia_false();
		//	return NULL;//brak sytuacji zablokowania
		//}
		else
			taktyka_zakonczenie = true;
		printf("TUATAJ ZAKONCZENIA TAKTYKA\n");
		//znalezenie piona, ktory jest "pomiedzy" zablokowanym pionem, a wolnym miejscem
		//if (ilosc_pionow_nie_na_miejscu == 1)
		//{
			if (wolne[0]->numer_pola_poziom > piony[0]->numer_pola_poziom)//pion po lewej od wolnego pola
			{
				printf("zakonczenie w lewo\n");
				printf("numer pola poziom wolne: %d\n", (wolne[0]->numer_pola_poziom));
				printf("numer pola poziom pion: %d\n", (piony[0]->numer_pola_poziom));
				zakonczenie_w_lewo = true;
				int x = wolne[0]->numer_pola_poziom;
				int y = wolne[0]->numer_pola_pion;
				int xp = piony[0]->numer_pola_poziom;
				int yp = piony[0]->numer_pola_pion;
				//if(pl->jakie_to_pole_numery(x - 1, y-1) != NULL&&pl->jakie_to_pole_numery(x-1, y-1)->pion_stoi_na_polu != NULL&&x-xp!=5)
				//{
				//	wybrane_pole = pl->jakie_to_pole_numery(x - 1, y - 1);
				//	faza_zablokowania = false;
				//	nowe_pole = wolne[0];
				//}
				if (pl->jakie_to_pole_numery(xp + 2, yp - 2) != NULL&&pl->jakie_to_pole_numery(xp + 2, yp - 2)->pion_stoi_na_polu != NULL)
				{
					pole* temp_wybrane = pl->jakie_to_pole_numery(xp + 2, yp - 2);
					int xt = temp_wybrane->numer_pola_poziom;
					int yt = temp_wybrane->numer_pola_pion;
					if (pl->jakie_to_pole_numery(xt + 1, yt + 1) != NULL&&pl->jakie_to_pole_numery(xt + 1, yt + 1)->pion_stoi_na_polu == NULL)
					{
						faza_zablokowania = false;
						wybrane_pole = temp_wybrane;
						nowe_pole = pl->jakie_to_pole_numery(xt + 1, yt + 1);
					}
					else if (pl->jakie_to_pole_numery(xt + 2, yt + 2) != NULL&&pl->jakie_to_pole_numery(xt + 2, yt + 2)->pion_stoi_na_polu == NULL&&
						pl->czy_jest_mozliwy_skok(NULL,temp_wybrane,pl->jakie_to_pole_numery(xt+2,yt+2)))
					{
						faza_zablokowania = false;
						wybrane_pole = temp_wybrane;
						nowe_pole = pl->jakie_to_pole_numery(xt + 2, yt + 2);
					}
				}
				if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
				{
					taktyka_zakonczenia_false();
					return NULL;
				}
				else
					return nowe_pole;
			}
			else
			{
				zakonczenie_w_prawo = true;
				int x = wolne[0]->numer_pola_poziom;
				int y = wolne[0]->numer_pola_pion;
				int xp = piony[0]->numer_pola_poziom;
				int yp = piony[0]->numer_pola_pion;
				if (pl->jakie_to_pole_numery(xp - 2, yp - 2) != NULL&&pl->jakie_to_pole_numery(xp - 2, yp - 2)->pion_stoi_na_polu != NULL)
				{
					pole* temp_wybrane = pl->jakie_to_pole_numery(xp - 2, yp - 2);
					int xt = temp_wybrane->numer_pola_poziom;
					int yt = temp_wybrane->numer_pola_pion;
					if (pl->jakie_to_pole_numery(xt - 1, yt + 1) != NULL&&pl->jakie_to_pole_numery(xt - 1, yt + 1)->pion_stoi_na_polu == NULL)
					{
						wybrane_pole = temp_wybrane;
						nowe_pole = pl->jakie_to_pole_numery(xt - 1, yt + 1);
					}
					else if (pl->jakie_to_pole_numery(xt - 2, yt + 2) != NULL&&pl->jakie_to_pole_numery(xt - 2, yt + 2)->pion_stoi_na_polu == NULL&&
						pl->czy_jest_mozliwy_skok(NULL, temp_wybrane, pl->jakie_to_pole_numery(xt - 2, yt + 2)))
					{
						wybrane_pole = temp_wybrane;
						nowe_pole = pl->jakie_to_pole_numery(xt - 2, yt + 2);
					}
				}
				if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
				{
					taktyka_zakonczenia_false();
					return NULL;
				}
				else
					return nowe_pole;
			}
		//}
	}
	else
	{
		if (zakonczenie_w_lewo)
		{
			printf("zakonczenie w lewo odbl\n");
			for (int i = 1; i <=ROZMIAR_PLANSZY; i++)
			{
				if (pl->jakie_to_pole_numery(i, 3) != NULL&&pl->jakie_to_pole_numery(i, 3)->pion_stoi_na_polu != NULL
					&&pl->jakie_to_pole_numery(i + 2, 1) != NULL&&
					pl->jakie_to_pole_numery(i + 2, 1)->pion_stoi_na_polu == NULL&&
					pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(i, 3), pl->jakie_to_pole_numery(i + 2, 1)))
				{
					printf("wybrane w taktyce %d, %d\n", i, 3);
					faza_zablokowania = true;
					wybrane_pole = pl->jakie_to_pole_numery(i, 3);
					nowe_pole = pl->jakie_to_pole_numery(i + 2, 1);
					if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
					{
						printf("EHH...\n");
						taktyka_zakonczenia_false();
						return NULL;
					}
					else
					{
						taktyka_zakonczenia_false();
						return nowe_pole;
					}
				}
				//else if (pl->jakie_to_pole_numery(i, 3) != NULL&&pl->jakie_to_pole_numery(i, 3)->pion_stoi_na_polu != NULL
				//	&&pl->jakie_to_pole_numery(i + 2, 1) != NULL&&
				//	pl->jakie_to_pole_numery(i + 2, 1)->pion_stoi_na_polu != NULL&&
				//	pl->jakie_to_pole_numery(i+4,3)!=NULL&&pl->jakie_to_pole_numery(i+4,3)->pion_stoi_na_polu==NULL&&
				//	pl->czy_jest_mozliwy_skok(pl->jakie_to_pole_numery(i + 2, 1), pl->jakie_to_pole_numery(i + 4, 3)))
				//{
				//	faza_zablokowania = true;
				//	wybrane_pole = pl->jakie_to_pole_numery(i+2, 1);
				//	nowe_pole = pl->jakie_to_pole_numery(i + 4, 3);
				//	if (pl->jakie_to_pole_numery(i + 6, 1) != NULL&&pl->jakie_to_pole_numery(i + 6, 1)->pion_stoi_na_polu == NULL&&
				//		pl->jakie_to_pole_numery(i + 5, 2) != NULL&&pl->jakie_to_pole_numery(i + 5, 2)->pion_stoi_na_polu != NULL)
				//		skok = true;

				//	if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
				//	{
				//		taktyka_zakonczenia_false();
				//		return NULL;
				//	}
				//	else
				//	{
				//		taktyka_zakonczenia_false();
				//		return nowe_pole;
				//	}
				//}
			}
		}
		else if (zakonczenie_w_prawo)
		{
			for (int i = ROZMIAR_PLANSZY; i >0; i--)
			{
				if (pl->jakie_to_pole_numery(i, 3) != NULL&&pl->jakie_to_pole_numery(i, 3)->pion_stoi_na_polu != NULL
					&&pl->jakie_to_pole_numery(i - 2, 1) != NULL&&
					pl->jakie_to_pole_numery(i - 2, 1)->pion_stoi_na_polu == NULL&&
					pl->czy_jest_mozliwy_skok(NULL, pl->jakie_to_pole_numery(i, 3), pl->jakie_to_pole_numery(i - 2, 1)))
				{
					faza_zablokowania = true;
					wybrane_pole = pl->jakie_to_pole_numery(i, 3);
					nowe_pole = pl->jakie_to_pole_numery(i - 2, 1);
					if (nowe_pole == NULL || nowe_pole->pion_stoi_na_polu != NULL)
					{
						taktyka_zakonczenia_false();
						return NULL;
					}
					else
					{
						taktyka_zakonczenia_false();
						return nowe_pole;
					}
				}
			}
		}
	}
	taktyka_zakonczenia_false();
	return NULL;
}

gracz_komputer::~gracz_komputer()
{
	delete trasa;
}