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
* Linux
* Raspberry Pi

## Usage
____________

### CLI Usage
* Run `obdcmd.exe` with the serial port number specified. Ex: `obdcmd.exe COM3`
* Omit arguments after the port to enter the interactive menu
* Or, specify `<command>` or `all` after the port to enter polling mode
* Enter `help` to show available commands
* Enter `dumpall` to fetch and display current diagnostic information
* Enter `<command>` to dump just one diagnostic item
* Enter `quit` to exit the utility

### Diagnostic Information
* Modern vehicles use OBDII for diagnostic codes. 
* This tool should provide most standard OBDII codes.
* Pre-1996 cars may use OBDI standards that vary by manufacturer, not supported by this tool
* Many OBDI codes can be pulled by placing a jumper wire across specified ports and watching check engine light flashes
* Plugging these codes into a search engine or code search application for information pertaining to your project!
