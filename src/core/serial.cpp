/* This file contains code that handles communication with the OBDII device
* via a Windows COM serial port
*
* Author: Josh McIntyre
*/

#include "serial.h"

// This constructor initializes the OS-dependent serial connection
SerialConnection::SerialConnection(std::string port)
{
	serial_port = new boost::asio::serial_port(io);
	connect_asio_port(port.c_str());
}

// This function closes the connection to the serial port
SerialConnection::~SerialConnection()
{
	serial_port -> close();
	delete serial_port;
}

/* This function fetches the response to a command via the Windows serial port connection
* It takes a standard \r (carriage return) terminated, standard OBDII code.
* It returns the raw ASCII response. Any processing of the response into useful data will be performed separately.
*/
std::string SerialConnection::fetch_response(std::string command, unsigned long expected_response_size)
{
	// First, send the command to the ELM327
	unsigned long bytes_sent_total = command.length();
	boost::asio::write(*serial_port, boost::asio::buffer(command.c_str(), bytes_sent_total));

	char buffer[100];
	memset(buffer, 0, sizeof(buffer));
	
	// Read the response
	int return_count = 0;
	for (int i = 0; i < 100; i++)
	{
		char buffer_char;
		boost::asio::read(*serial_port, boost::asio::buffer(&buffer_char, 1));
		buffer[i] = buffer_char;

		if (buffer_char == '>')
		{
			break;
		}
	}
	
	return std::string(buffer);
}

// This function establishes a connection to the serial port hosting the OBDII reader
void SerialConnection::connect_asio_port(const char* port)
{
	try
	{
		serial_port -> open(port);
		
		serial_port -> set_option(boost::asio::serial_port_base::baud_rate(38400));
		serial_port -> set_option(boost::asio::serial_port_base::character_size(8));
		
		boost::asio::serial_port_base::parity parity(boost::asio::serial_port_base::parity::none);
		boost::asio::serial_port_base::stop_bits stop_bits(boost::asio::serial_port_base::stop_bits::one);

		serial_port -> set_option(parity);
		serial_port -> set_option(stop_bits);
	}
	catch (int e)
	{
		std::cout << "Unable to access OBDII device via serial port\n";
		exit(1);
	}
}	
