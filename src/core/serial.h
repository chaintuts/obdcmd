/* This file contains function declarations and includes for the serial connection code
*
*Author: Josh McIntyre
*/

#include <iostream>
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
	

/* This class will abstract away the serial connection details away
* from classes/functions that need to communicate to the ELM327 OBDII device 
* via a serial connection.
*/
class SerialConnection
{
	private:
		boost::asio::io_service io;
		boost::asio::serial_port* serial_port;
	
	/* The following functions are designed to provide a common API for serial code across operating systems
	* When the code is compiled, regardless of OS, the other code in this program should be able to call
	* these public functions
	*/
	public:
		SerialConnection(std::string port);
		~SerialConnection();
		std::string fetch_response(std::string command, unsigned long expected_response_size);

	private:
		void connect_asio_port(const char* port_name);
};


