/* This file contains code that handles communication with the OBDII device
* via a Windows COM serial port
*
* Author: Josh McIntyre
*/

#include "command.h"

// Define constants for the ELM327 command strings
const char Command::CMD_ECHO_OFF[] = "AT E0\r";

const char Command::CMD_GET_DTCS[] = "03\r";
const char Command::CMD_GET_COOLANT_TEMP[] = "0105\r";
const char Command::CMD_GET_ENGINE_RPM[] = "010C\r";
const char Command::CMD_GET_VEHICLE_SPEED[] = "010D\r";
const char Command::CMD_GET_THROTTLE_POS[] = "0111\r";

const char Command::RET_NO_DATA[] = "NO DATA";
const char Command::RET_EMPTY[] = "";

std::map<char, std::string> Command::dtc_prefixes = { 
			{ '0', "P0" },
			{ '1', "P1" },
			{ '2', "P2" },
			{ '3', "P3" },
			{ '4', "C0" },
			{ '5', "C1" },
			{ '6', "C2" },
			{ '7', "C3" },
			{ '8', "B0" },
			{ '9', "B1" },
			{ 'A', "B2" },
			{ 'B', "B3" },
			{ 'C', "U0" },
			{ 'D', "U1" },
			{ 'E', "U2" },
			{ 'F', "U3" }
		};

/* This function interprets the raw data returned from a command
* It returns the data in a human-readable format
*/
std::string Command::interpret_raw(std::string raw_data, COMMAND command)
{
	// If no data was returned, skip trimming and interpretation
	if (raw_data.find(std::string(RET_NO_DATA)) != std::string::npos)
	{
		return std::string(RET_NO_DATA);
	}
	
	// Do preliminary trimming
	std:: string trimmed_data = trim_raw(raw_data);
	
	// Call the appropriate interpretation function
	std::string readable_data;
	
	switch (command)
	{
		case GET_DTCS:
			readable_data = interpret_dtcs(trimmed_data);
			break;

		case GET_COOLANT_TEMP:
			readable_data = interpret_coolant_temp(trimmed_data);
			break;

		case GET_ENGINE_RPM: 
			readable_data = interpret_engine_rpm(trimmed_data);
			break;

		case GET_VEHICLE_SPEED:
			readable_data = interpret_vehicle_speed(trimmed_data);
			break;
		
		case GET_THROTTLE_POS:
			readable_data = interpret_throttle_pos(trimmed_data);
			break;
	}

	return readable_data;
}

/* This function does preliminary string trimming for data interpretation
* Return data for all commands should be run through this function
*/
std::string Command::trim_raw(std::string raw_data)
{
		/* First, remove extraneous characters and whitespace from the string
	* Removing whitespace with boost's string methods will save a significant
	* amount of time over turning of spaces with an ELM327 AT command (AT S0)
	*/
	boost::erase_all(raw_data, "\r");
	boost::erase_all(raw_data, "\n");
	boost::erase_all(raw_data, " ");
	boost::erase_all(raw_data, ">");
	
	/* Next, strip the first two return characters from the raw data string
	* They indicate a returned PID but are meaningless for us
	*/
	std::string trimmed_data = raw_data.substr(2);
	
	return trimmed_data;
}

/* This function takes trimmed data and interprets it so that
* human-readable diagnostic trouble codes (DTC's) can be returned
*/
std::string Command::interpret_dtcs(std::string trimmed_data)
{
	std::vector<std::string> raw_dtcs;
	std::vector<std::string> dtc_strings;

	if (trimmed_data.length() >= 12)
	{
		raw_dtcs.push_back(trimmed_data.substr(0,4));
		raw_dtcs.push_back(trimmed_data.substr(4,4));
		raw_dtcs.push_back(trimmed_data.substr(8,4));
	}
	else
	{
		return RET_EMPTY;
	}

	// Parse each raw DTC
	std::vector<std::string>::iterator it;
	for (it = raw_dtcs.begin(); it != raw_dtcs.end(); it++)
	{
		/* Up to 3 DTCs can be returned in one message, but the message is always 12 bytes
		* 0000 indicates an empty slot in the message
		*/
		if (*it != "0000")
		{
			char prefix_char = (*it)[0];
			std::string dtc = dtc_prefixes[prefix_char] + (*it).substr(1);
			
			dtc_strings.push_back(dtc);
		}
	}
	
	// Merge the parsed DTCs into a single return string
	std::string joined_dtcs = boost::algorithm::join(dtc_strings, ", ");
	
	return joined_dtcs;
}

/* This helper function takes an individual 2 byte raw hexadecimal DTC 
* (diagnostic trouble code) and converts it to a human-readable format
*/
std::string Command::parse_raw_dtc(std::string raw_dtc)
{
	std::string readable_dtc;
	
	char first = raw_dtc[0];
	
	
	
	return readable_dtc;
}

/* This function takes trimmed data and interprets it so that
* a human-readable coolant temperature can be returned
* The temperature will be returned in degrees Celcius
*/
std::string Command::interpret_coolant_temp(std::string trimmed_data)
{
	/* Further command-specific trimming - remove extraneous PID echo 05 if it's
	* at the beginning of the data
	*/
	if (boost::starts_with(trimmed_data, "05"));
	{
		trimmed_data = trimmed_data.substr(2);
	}
	
	// Convert the data to an integer format
	int int_data = hex_data_to_int(trimmed_data);
	
	/* Finally, perform the appropriate formula calculation specified by the OBDII standard
	* The formula is X - 40 = temperature in degrees Celcius
	*/
	int_data = int_data - 40;

	// Build the return string
	std::stringstream ss;
	ss << int_data;
	
	return ss.str();
}

/* This function takes trimmed data and interprets it so that
* a human-readable engine RPM can be returned
*/
std::string Command::interpret_engine_rpm(std::string trimmed_data)
{
	std::string readable_data;
	
	/* Further command-specific trimming - remove extraneous PID echo 0C if it's
	* at the beginning of the data
	*/
	if (boost::starts_with(trimmed_data, "0C"));
	{
		trimmed_data = trimmed_data.substr(2);
	}
	
	/* Next we have to split up the two response bytes
	* each byte is represented by 2 hex characters and comprises the
	* "X and Y" in the OBDII standard RPM formula
	*/
	std::string x_byte = trimmed_data.substr(0, 2);
	std::string y_byte = trimmed_data.substr(2);
	
	// Convert the data to an integer format
	int int_x = hex_data_to_int(x_byte);
	int int_y = hex_data_to_int(y_byte);
	
	/* Finally, perform the appropriate formula calculation specified by the OBDII standard
	* The formula is (256X + Y) / 4 = revolutions per minute
	*/
	int int_data = std::ceil( (256.0 * int_x + int_y) / 4.0 );

	// Build the return string
	std::stringstream ss;
	ss << int_data;
	
	return ss.str();

	readable_data = trimmed_data;
	return readable_data;

}

/* This function takes trimmed data and interprets it so that
* a human-readable vehicle speed can be returned
* The speed will returned in kilometers per hour
*/
std::string Command::interpret_vehicle_speed(std::string trimmed_data)
{
	std::string readable_data;
	
	/* Further command-specific trimming - remove extraneous PID echo 0D if it's
	* at the beginning of the data
	*/
	if (boost::starts_with(trimmed_data, "0D"));
	{
		trimmed_data = trimmed_data.substr(2);
	}
	
	// Convert the data to an integer format
	int int_data = hex_data_to_int(trimmed_data);

	// Build the return string
	std::stringstream ss;
	ss << int_data;

	return ss.str();
}

/* This function takes trimmed data and interprets it so that
* a human-readable throttle position can be returned
* The throttle position is returned as a percentage
*/
std::string Command::interpret_throttle_pos(std::string trimmed_data)
{
	std::string readable_data;
	
	/* Further command-specific trimming - remove extraneous PID echo 0C if it's
	* at the beginning of the data
	*/
	if (boost::starts_with(trimmed_data, "11"));
	{
		trimmed_data = trimmed_data.substr(2);
	}

	// Convert the data to an integer format
	int int_data = hex_data_to_int(trimmed_data);
	
	/* Finally, perform the appropriate formula calculation specified by the OBDII standard
	* The formula is (100/255)X = percentage of throttle used
	*/
	int_data = std::ceil( ( 100.0 / 255.0 ) * int_data);

	// Build the return string
	std::stringstream ss;
	ss << int_data;
	
	return ss.str();
}

// This function converts a hexadecimal string to a signed integer
int Command::hex_data_to_int(std::string hex_string)
{
	std::stringstream ss;
	ss << std::hex << hex_string;
	
	int int_data;
	ss >> int_data;

	return int_data;
}
