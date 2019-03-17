/* This file contains code that handles communication with the OBDII device
* via a Windows COM serial port
*
* Author: Josh McIntyre
*/

#include "elm_device.h"

/* This constructor will initialize a serial connection -> and
* then initialize the ELM327 device with our desired settings
*/
ElmDevice::ElmDevice(std::string port)
{
	// Initialize the connection -> and then the desired device settings
	connection = new SerialConnection(port);
	init_settings();
}

// This destructor will free the serial connection's memory
ElmDevice::~ElmDevice()
{
	delete connection;
}

// This function process an OBDII command and returns the response data
std::string ElmDevice::get_data(Command::COMMAND cmd)
{
	// Fetch the raw response via the command object
	std::string raw_data;
	switch (cmd)
	{
		case Command::GET_DTCS:
			raw_data = connection -> fetch_response(std::string(Command::CMD_GET_DTCS), 20);
			break;

		case Command::GET_COOLANT_TEMP:
			raw_data = connection -> fetch_response(std::string(Command::CMD_GET_COOLANT_TEMP), 20);
			break;

		case Command::GET_ENGINE_RPM: 
			raw_data = connection -> fetch_response(std::string(Command::CMD_GET_ENGINE_RPM), 20);
			break;

		case Command::GET_VEHICLE_SPEED:
			raw_data = connection -> fetch_response(std::string(Command::CMD_GET_VEHICLE_SPEED), 20);
			break;
		
		case Command::GET_THROTTLE_POS:
			raw_data = connection -> fetch_response(std::string(Command::CMD_GET_THROTTLE_POS), 20);
			break;
	}
	
	// Convert the raw data into a human-readable format again via the command object
	std::string data = Command::interpret_raw(raw_data, cmd);

	return data;
}

void ElmDevice::init_settings()
{	
	connection -> fetch_response(std::string(Command::CMD_ECHO_OFF), 25);
}
