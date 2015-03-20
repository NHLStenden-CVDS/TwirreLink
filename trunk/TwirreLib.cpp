/*
 * TwirreLib.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "TwirreLib.h"

int main()
{
	std::cout << "Hello World!" << std::endl;
	return 0;
}

namespace twirre
{

	TwirreLib::TwirreLib ()
	{
	}

	std::string TwirreLib::init (char * device)
	{
		TwirreLib::soiw.Initialize(device, 115200);

		sleep(4);
		TwirreLib::soiw.flush();

		initActuators();

		return "Twirre Library initialized!";
	}

	void TwirreLib::initActuators ()
	{
		unsigned char* pchar = new unsigned char[2] { 'I', 'A' };
		TwirreLib::soiw.writeBytes(pchar, 2);
		std::string s = TwirreLib::soiw.readString();

		if (s.length() > 1)
		{
			char a = s[0];
			if (a == 'O')
			{
				std::string b = s.substr(1);

				std::vector<std::string> v;
				Helper::split(b, ';', v);

				for (int i = 0; i < v.size(); ++i)
				{
					std::cout << v[i] << std::endl;

					std::vector<std::string> x;
					Helper::split(v[i], '|', x);

					Actuator a;

					a.ID = i;
					a.Name = x[0];
					a.Description[1];

					TwirreLib::actuatorList.push_back(a);
				}
			}
			else
			{
				std::cout << "LEEG" << std::endl;
			}
		}
		else
		{
			std::cout << "LEEG" << std::endl;
		}

		//-------------------------------------------------------------

		delete pchar;
	}

	std::vector<Actuator> TwirreLib::GetActuatorList ()
	{
		return TwirreLib::actuatorList;
	}

	std::vector<Sensor> TwirreLib::GetSensorList ()
	{
		std::vector<Sensor> lijst;

		return lijst;
	}

	Actuator TwirreLib::GetActuator (int n)
	{
		Actuator a;
		return a;
	}

	Sensor TwirreLib::GetSensor (int n)
	{
		Sensor s;
		return s;
	}

	char TwirreLib::Ping ()
	{
		TwirreLib::soiw.Write('P');
		return TwirreLib::soiw.Read<char>();
	}

	TwirreLib::~TwirreLib ()
	{
	}

} /* namespace twirre */
