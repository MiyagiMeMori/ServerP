#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>


/**
 * Класс устройства
 */
class Device {
public:
	Device() = default;
	Device(std::string _Name, char *_UuidData);
	std::string GetStrFields();//Device device
	std::string getName();
	char* getUuid();
protected:
	std::string _Name;
	char *_UuidData; //UUID UuidData_;
};

/**
* Класс биометрического сканера
*/
class BiometricScanner : public virtual Device {
public:
	BiometricScanner(std::string Name, char *UuidData);
	BiometricScanner() = default;
	bool BioTest(char* BioStandart, char* BioData);

private:
	//BioData BioStandart_, BioData_;
	char _BioStandart[9];
	char _BioData[9];
};

/**
* Класс датчика
*/
class Sensor : public virtual Device {
public:
	Sensor(std::string Name, char* UuidData);
	bool Anxiety();
};

/**
* Класс сервера
*/
class Server {
public:

	void addScanner(BiometricScanner Scnnr);
	void addSensor(Sensor Snsr);

	//template <typename T>
	std::pair<std::vector<BiometricScanner>, std::vector<Sensor>> Info();

	void deleteScanner(BiometricScanner Scan);
	void deleteSensor(Sensor Sens);

	std::vector<BiometricScanner> getScanners();
	std::vector<Sensor> getSensors();

protected:
	std::vector<BiometricScanner> bScanners;
	std::vector<Sensor> Sensors;
};
