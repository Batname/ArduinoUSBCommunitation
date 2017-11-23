#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "SerialPort.h"

using namespace std;

/*Portname must contain these backslashes, and remember to
replace the following com port*/
char *PortName = "\\\\.\\COM3";

//String for incoming data
char IncomingData[MAX_DATA_LENGTH];

//String for getting the output from arduino
char Output[MAX_DATA_LENGTH];


int main(int argc, char * argv[])
{
	SerialPort Arduino(PortName);

	if (Arduino.IsConnected())
	{
		cout << "Connection Established" << endl;
	}
	else
	{
		cout << "ERROR, check port name" << endl;
	}

	while (Arduino.IsConnected())
	{
		cout << "Write ON or OFF: \n";

		std::string InputString;
		getline(cin, InputString);
		char *c_string = new char[InputString.size() + 1];
		std::copy(InputString.begin(), InputString.end(), c_string);
		c_string[InputString.size()] = '\n';

		//Writing string to arduino
		Arduino.WriteSerialPort(c_string, MAX_DATA_LENGTH);

		// Read from arduino
		int ReadResult = Arduino.ReadSerialPort(IncomingData, MAX_DATA_LENGTH);
		IncomingData[ReadResult] = 0;
		printf("%s", IncomingData);

		//freeing c_string memory
		delete[] c_string;

		//wait a bit
		Sleep(500);
	}

	return 0;
}