#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "TransitNetwork.h"

int main()
{
	TransitNetwork network;
	bool stops_loaded = false, lines_loaded = false;

	std::cout << "Dobrodošli u simulator mreže gradskog prevoza. Molimo Vas, odaberite opciju:" << '\n';
	while(true)
	{
		//load block begin
		while (!stops_loaded || !lines_loaded) {
			int option = 0;

			std::cout << "1. Učitavanje podataka o mreži gradskog prevoza" << '\n';
			std::cout << "0. Kraj rada:" << '\n';

			std::cout << constants::console_yellow_text;
			std::cin >> option;
			std::cout << constants::console_default;

			switch (option)
			{
			case 1:
				while(!stops_loaded)
				{
					try
					{
						std::cout << "Molimo Vas, unesite putanju do fajla sa stajalištima:" << '\n';
						std::string filename;

						std::cout << constants::console_yellow_text;
						std::cin >> filename;
						std::cout << constants::console_default;

						network.load_stops_data(filename);
						stops_loaded = true;
					}
					catch (Exception& e)
					{
						std::cout << e.repr_string() << '\n';
					}
				}
				while(!lines_loaded)
				{
					try {
						std::cout << "Molimo Vas, unesite putanju do fajla sa linijama gradskog prevoza:" << '\n';
						std::string filename;

						std::cout << constants::console_yellow_text;
						std::cin >> filename;
						std::cout << constants::console_default;

						network.load_transit_lines_data(filename);
						lines_loaded = true;
					}
					catch (Exception& e)
					{
						std::cout << e.repr_string() << '\n';
					}
				}
				break;
			case 0:
				return 0;
			default:
				std::cout << constants::console_red_text << 
					"Nesipravan unos! Izaberite jednu od ponuđenih opcija."<< '\n' << constants::console_default;
			}
		}
		//load block end
		
		std::cout << "Mreža gradskog prevoza je uspešno učitana. ";
		while (true)
		{
			std::cout << "Molimo Vas, odaberite opciju:" << '\n';
			std::cout << "1. Prikaz informacija o stajalištu" << '\n';
			std::cout << "2. Prikaz osnovnih informacija o liniji gradskog prevoza" << '\n';
			std::cout << "3. Prikaz statističkih informacija o liniji gradskog prevoza" << '\n';
			std::cout << "4. Pronalazak putanje između dva stajališta" << '\n';
			std::cout << "0. Kraj rada" << '\n';

			int option;

			std::cout << constants::console_yellow_text;
			std::cin >> option;
			std::cout << constants::console_default;

			switch (option)
			{
			case 1:
				std::cout << "Molimo Vas, unesite oznaku stajališta čije informacije želite da prikažete." << '\n';
				try {
					int stop;

					std::cout << constants::console_yellow_text;
					std::cin >> stop;
					std::cout << constants::console_default;

					network.output_stop_info(stop);
					std::cout <<constants::console_green_text << "Generisan je fajl stajaliste_" << stop <<
						".txt sa informacijama o stajalištu " << stop << "." << '\n' << constants::console_default;
				}
				catch (Exception& e)
				{
					std::cout << e.repr_string() << '\n';
				}
				break;
			case 2:
				std::cout << "Molimo Vas, unesite oznaku linije čije osnovne informacije želite da prikažete." << '\n';
				try {
					std::string line;

					std::cout << constants::console_yellow_text;
					std::cin >> line;
					std::cout << constants::console_default;

					network.output_line_info(line);
					std::cout << constants::console_green_text << "Generisan je fajl linija_" << line <<
						".txt sa osnovnim informacijama o liniji " << line << "." << '\n' << constants::console_default;
				}
				catch (Exception& e)
				{
					std::cout << e.repr_string() << '\n';
				}
				break;
			case 3:
				std::cout << "Molimo Vas, unesite oznaku linije čije statističke informacije želite da prikažete." << '\n';
				try {
					std::string line;

					std::cout << constants::console_yellow_text;
					std::cin >> line;
					std::cout << constants::console_default;

					network.output_line_statistics(line);
					std::cout << constants::console_green_text << "Generisan je fajl statistika_" << line <<
						".txt sa statističkim informacijama o liniji " << line << "." << '\n' << constants::console_default;
				}
				catch (Exception& e)
				{
					std::cout << e.repr_string() << '\n';
				}
				break;
			case 4:
				try {
					int departure_stop, destination_stop;
					std::string departure_time;

					std::cout << "Molimo Vas, unesite oznaku početnog stajališta" << '\n';

					std::cout << constants::console_yellow_text;
					std::cin >> departure_stop;
					std::cout << constants::console_default;

					std::cout << "Molimo Vas, unesite oznaku krajnjeg stajališta" << '\n';

					std::cout << constants::console_yellow_text;
					std::cin >> destination_stop;
					std::cout << constants::console_default;

					std::cout << "Molimo Vas, unesite vreme polaska" << '\n';

					std::cout << constants::console_yellow_text;
					std::cin >> departure_time;
					std::cout << constants::console_default;

					bool loop = true;
					while (loop) {
						std::cout << "Molimo Vas, odaberite strategiju koju želite za traženje putanje." << '\n';
						std::cout << "1. Bilo koja putanja između početnog i krajnjeg stajališta." << '\n';
						std::cout << "2. Najbrža putanja između početnog i krajnjeg stajališta." << '\n';
						std::cout << "3. Najmanji broj presedanja između početnog i krajnjeg stajališta." << '\n';
						std::cout << "0. Kraj pretrage putanji." << '\n';

						int option;
						std::cout << constants::console_yellow_text;
						std::cin >> option;
						std::cout << constants::console_default;

						switch (option)
						{
						case 1:
						case 2:
							network.find_path(constants::strategy::FASTEST, departure_stop, destination_stop, departure_time);
							break;
						case 3:
							network.find_path(constants::strategy::LEAST_TRANSFERS, departure_stop, destination_stop, departure_time);
							break;
						case 0:
							loop = false;
							break;
						default:
							std::cout << constants::console_red_text <<
								"Nesipravan unos! Izaberite jednu od ponuđenih opcija." << '\n' << constants::console_default;
						}
					}
				}
				catch (Exception& e)
				{
					std::cout << e.repr_string() << '\n';
				}
				break;
			case 0:
				return 0;
			default:
				std::cout << constants::console_red_text <<
					"Nesipravan unos! Izaberite jednu od ponuđenih opcija." << '\n' << constants::console_default;
			}
		}

	}
}


