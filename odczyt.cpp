#include "odczyt.h"

string odczyt::czytaj_slowo(ALLEGRO_KEYBOARD_STATE klawiatura)
{
	char znaki1[20];
	event.keyboard.unichar = NULL;

	while (1)
	{
		al_get_next_event(queue, &event);
		if (event.keyboard.unichar != 0 && !key_state_down)
		{
			if (event.keyboard.unichar == 8)
			{
				i--;
			}
			else
			{
				znaki1[i] = char(event.keyboard.unichar);
				i++;
			}
			key_state_down = true;
		}
		if (!event.keyboard.unichar) key_state_down = false;

		for (int j = 0; j < i; j++)
		{
			std::cout << znaki1[j];
		}
		std::cout << std::endl;
	}
}