# This file contains a basic python OBD script
#
# Author: Josh McIntyre
#

import logging
import io
import math
import serial

# Set log level
logging.basicConfig(level=logging.INFO)

# Constants for the serial connection
PORT = "COM5"
BAUD = "38400"

# Available commands
class Command():

    def __init__(self, plaintext, elm, parse_func):

        self.plaintext = plaintext
        self.elm = elm
        self.parse_func = parse_func

def parse_echo_off():
    pass # do nothing

def parse_rpm(resp):

    # Parse the int from the trimmed hexadecimal response
    int_x = int(resp[:2], 16)
    int_y = int(resp[2:], 16)

    # Finally, perform the appropriate formula calculation specified by the OBDII standard
	# The formula is (256X + Y) / 4 = revolutions per minute
    resp  = math.ceil( (256.0 * int_x + int_y) / 4.0 )
    
    return resp

CMD_ECHO_OFF = Command("Echo off", b"AT E0\r", parse_echo_off)
CMD_GET_ENGINE_RPM = Command("RPM", b"010C\r", parse_rpm)

# Setup the serial connection
def setup(port, baud):

    # Set up the connection
    conn = serial.Serial(port, baud, timeout=1)
    if not conn:
        logging.error("Unable to set up serial connection to ELM327 device")
    logging.debug("Set up basic serial connection object")
    
    # Set echo off for all commands
    run_cmd(conn, CMD_ECHO_OFF)
    
    # Let the user know we are set up
    logging.info("Set up ELM327 OBDII connection")
    
    return conn

# Write a command and get the response
def run_cmd(conn,  cmd):

    logging.debug(f"Writing command {cmd.plaintext}: {cmd.elm}")
    conn.write(cmd.elm)
    conn.flush()
    
    resp = conn.read(100)
    logging.debug(f"Raw response: {resp}")

    resp = trim_resp(resp)
    logging.debug(f"Trimmed response {resp}")

    return resp

# Trim raw data returned by the ELM327 device
def trim_resp(resp):

    # Decode the bytes into a string
    resp = resp.decode()

    # Remove extraneous characters from the ELM327 response
    resp = resp.replace("\n", "")
    resp = resp.replace("\r", "")
    resp = resp.replace(" ", "")
    resp = resp.replace(">", "")
    
    # Next, strip the first two return characters from the raw data string
	# They indicate a returned PID but are meaningless for us
    resp = resp[4:]

    return resp

# Print a response
def output(resp):

    logging.info(resp)

    
# A more public-facing function for running a command and printing the response
def run_output(conn, cmd):

    resp = run_cmd(conn, cmd)
    resp = cmd.parse_func(resp)
    output(f"{cmd.plaintext}: {resp}")

# The main entry point for the program
def main():

    conn = setup(PORT, BAUD)

    run_output(conn, CMD_GET_ENGINE_RPM)

    conn.close()

if __name__ == "__main__":
    main()