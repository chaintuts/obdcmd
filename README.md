## General
____________

### Author
* Josh McIntyre

### Website
* jmcintyre.net

### Overview
* ObdCmd provides a command-line utility for dumping OBDII diagnostic information

## Development
________________

### Git Workflow
* master for releases (merge development)
* development for bugfixes and new features

### Building
* make build
Build the utility
* make clean
Clean the build directory

### Features
* Dump all currently available OBDII diagnostic information
* NOTE: The utility may take a moment to initialize settings on startup

### Requirements
* Requires a connected ELM327 device on a virtual serial port

### Platforms
* Windows

## Usage
____________

### CLI Usage
* Run `obdcmd.exe` with the serial port number specified. Ex: `obdcmd.exe COM3`
* Enter `help` to show available commands
* Enter `dumpall` to fetch and display current diagnostic information
* Enter `quit` to exit the utility
