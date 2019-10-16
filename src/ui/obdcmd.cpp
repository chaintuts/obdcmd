/* This file contains code for connecting to a serial OBDII reader
* and printing current information such as trouble codes and ECU stats
* This file contains the main entry point for the program
* 
* Author: Josh McIntyre
*/

#include "obdcmd.h"

// This function is the main entry point for the program
int main(int argc, char* argv[])
{
	// Retrieve the serial port from the command line
	std::string port = "";

	if (argc > 1)
	{
		port = std::string(argv[1]);
	}
	else
	{
		std::cout << "Usage obdcmd [required: <serial port>]\n";
		exit(EXIT_FAILURE);
	}

	// Declare an ElmDevice instance that will initialize the connection via its constructor
	std::cout << "Initializing settings (this may take a moment)...";
	ElmDevice elm_device(port);
	std::cout << "Done!" << std::endl;
	
	// Enter the main menu
	main_menu(elm_device);
	
	return 0;
}

void main_menu(ElmDevice &elm_device)
{
	std::cout << "Welcome! Enter 'help' for a list of commands\n";
	while (true)
	{
		std::cout << "> ";

		std::string menu_cmd;
		std::getline(std::cin, menu_cmd);
		
		if (menu_cmd == "dumpall" || menu_cmd == "da")
		{
			dump_all(elm_device);
		}
		else if (menu_cmd == "help" || menu_cmd == "h")
		{
			show_help();
		}
		else if (menu_cmd == "quit" || menu_cmd == "q")
		{
			exit(0);
		}
		else if (cmd_items.find(menu_cmd) != cmd_items.end())
		{
			dump_item(elm_device, menu_cmd);
		}
		else
		{
			std::cout << "Invalid command. Enter 'help' for a list of valid commands" << std::endl;
		}
	}
}

void dump_item(ElmDevice &elm_device, std::string item)
{
	std::cout << "Dumping requested OBDII data...\n";

	std::string data = elm_device.get_data(cmd_items[item]);
	std::cout << cmd_labels[item] << data << cmd_units[item] << std::endl;
}

void dump_all(ElmDevice &elm_device)
{
	std::cout << "Dumping all currently available OBDII data...\n";

	for (int i = 0; i < AVAILABLE_COMMANDS_SIZE; i++)
	{
		std::string item = available_items[i];
		std::string data = elm_device.get_data(cmd_items[item]);
		std::cout << cmd_labels[item] << data << cmd_units[item] << std::endl;
	}
}

void show_help()
{
	std::cout << "'dumpall'\t\tDump all available OBDII data:\n";
	std::cout << "'<datapoint>'\t\tDump a specific item\n";
	std::cout << "\t\t\t(dtc) : First 3 diagnostic trouble code (DTCs)\n";
	std::cout << "\t\t\t(coo) : Current coolant temperature in degrees Celcius\n";
	std::cout << "\t\t\t(rpm) : Current engine RPM\n";
	std::cout << "\t\t\t(spd) : Current vehicle speed in kilometers per hour\n";
	std::cout << "\t\t\t(thr) : Current throttle position as percentage of throttle used\n";
	
	std::cout << "'help'\t\t\tShow this help text\n";
	
	std::cout << "'quit'\t\t\tQuit the OBDII utility\n";
}

