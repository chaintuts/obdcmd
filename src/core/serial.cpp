/* This file contains code that handles communication with the OBDII device
* via a Windows COM serial port
*
* Author: Josh McIntyre
*/

#include "serial.h"

// This constructor initializes the OS-dependent serial connection
SerialConnection::SerialConnection(std::string port)
{
	#ifdef WINDOWS
		com_file_handle = connect_com(port.c_str());
	#endif
}

// This function closes the connection to the serial port
SerialConnection::~SerialConnection()
{
	#ifdef WINDOWS
		CloseHandle(com_file_handle);
	#endif
}

/* This function fetches the response to a command via the Windows serial port connection
* It takes a standard \r (carriage return) terminated, standard OBDII code.
* It returns the raw ASCII response. Any processing of the response into useful data will be performed separately.
*/
std::string SerialConnection::fetch_response(std::string command, unsigned long expected_response_size)
{
	#ifdef WINDOWS
		// First, send the command to the ELM327
		unsigned long bytes_sent_total = command.length();
		int write_success = WriteFile(com_file_handle, command.c_str(), command.length(), &bytes_sent_total, 0);
		
		if (! write_success)
		{
			std::cout << "Unable to send command to OBDII device\n";
			exit(1);
		}
		
		/* Now read the response from the serial port
		* We're doing non-overlapping IO so the ReadFile call will block until it times out
		* Put the response in a temporary buffer, then copy to the reponse returned in the function parameters
		*/
		char buffer[100];
		memset(buffer, 0, sizeof(buffer));
		unsigned long resp_size = expected_response_size;
		int read_success = ReadFile(com_file_handle, &buffer, 50, &resp_size, NULL);
		
		if (! read_success)
		{
			std::cout << "Unable to read response from OBDII device\n";
			exit(1);
		}
		
		std::string response_string(buffer);
		
		return std::string(buffer);
	#endif
}

// This function establishes a connection to the COM (serial) port hosting the OBDII reader
#ifdef WINDOWS
HANDLE SerialConnection::connect_com(const char* port_name)
{
	/* Use non-overlapping I/O for this COM port connection
	* This is limiting, but keeps things simple
	* Any read/write operations on the port will be blocking
	*/
	HANDLE com_file_handle = CreateFile(port_name, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if (com_file_handle == INVALID_HANDLE_VALUE)
	{
		std::cout << "Unable to access OBDII device via serial port\n";
		exit(1);
	}
	
	/* Before returning the file handle, let's set the DCB configuration
	* This structure sets the parameters the OS will use to communicate with the device
	*/
	DCB dcb;
	if (GetCommState(com_file_handle, &dcb))
	{
		dcb.BaudRate = CBR_38400;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.fBinary = TRUE;
		dcb.fParity = TRUE;
	}
	else
	{
			std::cout << "Unable to fetch communication parameters from OBDII device\n";
			exit(1);
	}
	
	/* We also need to set timeouts so reads don't block indefinitely
	* 3 seconds should be a good amount of time
	*/
	int timeoutMs = 3000;
	
	COMMTIMEOUTS timeouts;
	if (GetCommTimeouts(com_file_handle, &timeouts))
	{
		timeouts.ReadTotalTimeoutConstant = timeoutMs;
	}

	if (!SetCommState(com_file_handle, &dcb) || !SetCommTimeouts(com_file_handle, &timeouts))
	{
		std::cout << "Unable to update communication parameters with OBDII device\n";
	}
	
	return com_file_handle;
}
#endif
