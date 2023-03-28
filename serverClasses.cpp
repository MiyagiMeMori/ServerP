#include "ServerClasses.h"

Device::Device(std::string Name, char *UuidData)
    : _Name(Name), _UuidData(UuidData) {}
std::string Device::GetStrFields() { //Device device
    std::string retStr;
    std::string strUuid;

    retStr = this->_Name;
    retStr.append(" ");
    strUuid = this->_UuidData;
    retStr.append(strUuid);

    return retStr;
}
std::string Device::getName() {
    return this->_Name;
}
char* Device::getUuid() {
    return this->_UuidData;
}
/**
* Класс биометрического сканера
*/
BiometricScanner::BiometricScanner(std::string Name, char* UuidData) {// : Device(_Name, _UuidData) { }
    this->_Name = Name;
    this->_UuidData = UuidData;
}
Sensor::Sensor(std::string Name, char* UuidData) {
    this->_Name = Name;
    this->_UuidData = UuidData;
}
bool BiometricScanner::BioTest(char* BioStandart, char* BioData) {
    return rand() % 1;
}
//BiometricScanner::BiometricScanner()
//    //: BioStandart_(BioStandart), BioData_(BioData)
//{
//    for (size_t i = 0; i != 9; ++i) {
//        _BioStandart[i] = rand();
//        _BioData[i] = rand();
//    }
//}
//bool BiometricScanner::BioTest() {
//    if (_BioStandart == _BioData) { return true; }
//    else { return false; }
//}


/**
* Класс датчика
*/
bool Sensor::Anxiety() { return (rand() % 1); } //тревога 

//template <typename T>
std::pair<std::vector<BiometricScanner>, std::vector<Sensor>> Server::Info() {
    std::pair<std::vector<BiometricScanner>, std::vector<Sensor>> retPair;
    retPair.first = bScanners;
    retPair.second = Sensors;
    return retPair;
}

void Server::addScanner(BiometricScanner Scan) { //
    bScanners.push_back(Scan);
}
void Server::addSensor(Sensor Sens) {
    Sensors.push_back(Sens);
}
std::vector<BiometricScanner> Server::getScanners() {
    return this->bScanners;
}
std::vector<Sensor> Server::getSensors() {
    return this->Sensors;
}
//void Server::deleteScanner(BiometricScanner Scan) { //
//    for (int i = 0; i < bScanners.size(); ++i) {
//        if (bScanners[i].Name_ == Scan.Name_) {
//            bScanners.erase(bScanners.begin() + i);
//        }
//    }
//}
//void Server::deleteSensor(Sensor Sens) {
//    for (int i = 0; i < Sensors.size(); ++i) {
//        if (Sensors[i].Name_ == Sens.Name_) {
//            bScanners.erase(bScanners.begin() + i);
//        }
//    }
//}
