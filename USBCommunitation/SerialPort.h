#pragma once

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

class SerialPort
{
public:
	SerialPort(char* PortName);
	~SerialPort();

	int ReadSerialPort(char* Buffer, unsigned int BufSize);
	bool WriteSerialPort(char* Buffer, unsigned int BufSize);
	bool IsConnected();

private:
	HANDLE Handler;
	bool Connected;
	COMSTAT Status;
	DWORD Errors;
};

