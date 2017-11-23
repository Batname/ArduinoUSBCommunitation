#include "SerialPort.h"



SerialPort::SerialPort(char* PortName)
	: Connected(false)
{
	Handler = CreateFileA(static_cast<LPCSTR>(PortName),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (Handler == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("ERROR: Handle was not attached. Reason: %s not available\n", PortName);
		}
		else
		{
			printf("Unknown error, port %s\n", PortName);
		}
	}
	else
	{
		DCB DCBSerialParameters = { 0 };
		if (!GetCommState(Handler, &DCBSerialParameters))
		{
			printf("failed to get current serial parameters");
		}
		else
		{
			DCBSerialParameters.BaudRate = CBR_9600;
			DCBSerialParameters.ByteSize = 8;
			DCBSerialParameters.StopBits = ONESTOPBIT;
			DCBSerialParameters.Parity = NOPARITY;
			DCBSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(Handler, &DCBSerialParameters))
			{
				printf("ALERT: could not set Serial port parameters\n");
			}
			else
			{
				Connected = true;
				PurgeComm(Handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
}


SerialPort::~SerialPort()
{
	if (Connected)
	{
		Connected = false;
		CloseHandle(Handler);
	}
}

int SerialPort::ReadSerialPort(char * Buffer, unsigned int BufSize)
{
	DWORD BytesRead;
	unsigned int ToRead;

	ClearCommError(Handler, &Errors, &Status);

	if (Status.cbInQue > 0)
	{
		if (Status.cbInQue > BufSize)
		{
			ToRead = BufSize;
		}
		else
		{
			ToRead = Status.cbInQue;
		}

		//Try to read the require number of chars, and return the number of read bytes on success
		if (ReadFile(Handler, Buffer, ToRead, &BytesRead, NULL))
		{
			return BytesRead;
		}
	}

	return 0;
}

bool SerialPort::WriteSerialPort(char * Buffer, unsigned int BufSize)
{
	DWORD BytesSend;

	if (!WriteFile(Handler, (void*)Buffer, BufSize, &BytesSend, 0))
	{
		ClearCommError(Handler, &Errors, &Status);
		return false;
	}
	else
	{
		return true;
	}
}

bool SerialPort::IsConnected()
{
	return Connected;
}
