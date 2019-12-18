/* This file contains function definitions and includes for the main program
*
* Author: Josh McIntyre
*/

#include <iostream>
#include "elm_device.h"

void main_menu(ElmDevice &elm_device);
void poll_loop(ElmDevice &elm_device, std::string cmd);
void dump_item(ElmDevice &elm_device, std::string item);
void dump_item_poll(ElmDevice &elm_device, std::string item);
void dump_all(ElmDevice &elm_device);
void dump_all_poll(ElmDevice &elm_device);
void show_help();

// Available UI modes
const std::string MODE_INTERACTIVE = "cmd";
const std::string MODE_POLL = "poll";

// Definitions for polling
const int POLL_INTERVAL = 1000;

// Create maps of available command data to Command::COMMANDS, units, and labels
const std::string COMMAND_ALL = "all";

const int AVAILABLE_COMMANDS_SIZE = 5;
std::string available_items[AVAILABLE_COMMANDS_SIZE] = { "dtc", "coo", "rpm", "spd", "thr" };

std::map<std::string, Command::COMMAND> cmd_items = {
	{ "dtc", Command::GET_DTCS },
	{ "coo", Command::GET_COOLANT_TEMP },
	{ "rpm", Command::GET_ENGINE_RPM },
	{ "spd", Command::GET_VEHICLE_SPEED },
	{ "thr", Command::GET_THROTTLE_POS }
};

std::map<std::string, std::string> cmd_units = {
	{ "dtc", "" },
	{ "coo", "\370C" },
	{ "rpm", " RPM" },
	{ "spd", " km/h" },
	{ "thr", "%" }
};

std::map<std::string, std::string> cmd_labels = {
	{ "dtc", "Diagnostic code(s): " },
	{ "coo", "Coolant temp: " },
	{ "rpm", "Engine RPM: " },
	{ "spd", "Vehicle speed: " },
	{ "thr", "Throttle position: " }
};
