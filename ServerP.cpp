//server
#include <conio.h> // _getch
#pragma comment(lib, "ws2_32.lib") //Линкуем динамическую библиотеку ядра ОС
#include <winsock2.h>//для работы с сокетами
#include <WS2tcpip.h> //интерфейсы для работы с TCP/IP
#define _WINSOCK_DEPRECATED_NO_WARNINGS //что бы небыло некоторых ошибок
#include <iostream>
#include <vector>
#include <algorithm>
#include "serverClasses.h"
//#include <algorithm>

#pragma warning(disable: 4996)

std::string rtrim(const std::string &s) {
    const std::string trash = "\n\0";
    std::string retStr;
    //auto it = find_first_of(s.begin(), s.end(), trash.begin(), trash.end());

    for (int i = 0; i < s.size(); ++i) {
        if (s[i] != '\n') {
            retStr.push_back(s[i]);
        } else {
            return retStr;
        }
    }

    return retStr;
}
//recv(ClientConn, vecUUID.data(), BUFF_SIZE, 0);
//send(ClientConn, strToClient.data(), strToClient.length(), 0);
char write(SOCKET ClientConn, std::string str) {
    std::string element;
    for (int i = 0; i < str.length() || str[i] != '\0'; ++i) {
        element = str[i];
        send(ClientConn, element.data() , 1, 0);
    }
    return str.length();
}
std::vector<char> read(SOCKET ClientConn) {
    std::vector<char> element(1);
    std::vector<char> retVec;
    do {
        recv(ClientConn, element.data(), 1, 0);
        retVec.push_back(element[0]);
    } while (element[0] != '\0');
}


int messageInterpretator(SOCKET ServSock, SOCKET ClientConn) {
    struct timeval timeout;
    timeout.tv_sec = 5000;
    timeout.tv_usec = 0;
    setsockopt(ClientConn, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof timeout);
    const short BUFF_SIZE = 1024;
    std::vector<char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE); //может возниткнуть проблема в сравненни векторов разных размеров
    Server server;
    short packet_size = 0;

    while (true) {
        std::vector<char> vecMessage;
        //vecMessage = read(ClientConn);
        packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
        if (packet_size <= 0) continue;
        std::string message(servBuff.begin(), servBuff.end());
        message = rtrim(message);
        std::cout << "Client's message: " << servBuff.data() << std::endl;

        //TODO многопоточно обрабатывать сообщения 
        //std::cout << "Your (host) message: ";
        //fgets(clientBuff.data(), clientBuff.size(), stdin);

        std::string patternStr = "exit";
        if (message == patternStr) {
            shutdown(ClientConn, SD_BOTH);
            closesocket(ServSock);
            closesocket(ClientConn);
            WSACleanup();
            return 0;
        }
        patternStr = "cmd info";
        if (message == patternStr) {
            std::string strClientBuff = "cmd info\nexit\nadd biometric scanner\nadd sensor\ndelete biometrical scanner\ndelete sensor\ndevises info\nanxiety query\nbiometric test";
            packet_size = send(ClientConn, strClientBuff.data(), BUFF_SIZE, 0); //strClientBuff.size()
            //std::cout << "cmd info\n" <<
            //             "exit\n" <<
            //             "add biometric scanner\n" <<
            //             "add sensor\n" <<
            //             "delete biometrical scanner\n" <<
            //             "delete sensor\n" <<
            //             "devises info\n" <<
            //             "anxiety query\n" << 
            //             "biometric test\n" << std::endl;
        }
        patternStr = "add biometric scanner";
        if (message == patternStr) { //каждый раз проверять ан ошибку packet_size???
            //TODO пофиксить мусорные значения в принимаемом сообщении
            char* uuid = new char[16];
            std::vector<char> vecUUID(BUFF_SIZE);
            std::vector<char> vecName(BUFF_SIZE);
            //char* cName = new char[20];
            std::string strToClient = "Enter device name: ";
            //std::vector<char> vecClientBuff(BUFF_SIZE);
            //char cFromClient[];
            packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0);
            //vecName.clear();
            packet_size = recv(ClientConn, vecName.data(), BUFF_SIZE, 0);
            vecName.resize(packet_size);
            do {
                strToClient = "device UUID must contain exactly 16 characters\n Enter UUID of device: ";
                send(ClientConn, strToClient.data(), strToClient.length(), 0); //TODO vector
                //vecUUID.clear();
                packet_size = recv(ClientConn, vecUUID.data(), BUFF_SIZE, 0);
            } while (packet_size != 17);
            vecUUID.resize(16);
            //std::string sName = std::string(cName);
            std::string sName(vecName.begin(), std::find(vecName.begin(), vecName.end(), '\n'));
            for (int i = 0; i < vecUUID.size(); ++i) { uuid[i] = vecUUID[i]; }
            BiometricScanner bScan(sName, uuid);
            server.addScanner(bScan);
            strToClient = "biometric scanner added successfuly";
            packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0);
        }

        patternStr = "add sensor";
        if (message == patternStr) { //каждый раз проверять ан ошибку packet_size???
            //TODO пофиксить мусорные значения в принимаемом сообщении
            char* uuid = new char[16];
            std::vector<char> vecUUID(BUFF_SIZE);
            std::vector<char> vecName(BUFF_SIZE);
            //char* cName = new char[20];
            std::string strToClient = "Enter device name: ";
            //std::vector<char> vecClientBuff(BUFF_SIZE);
            //char cFromClient[];

            packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0);
            //vecName.clear();
            packet_size = recv(ClientConn, vecName.data(), BUFF_SIZE, 0);
            do {
                strToClient = "device UUID must contain exactly 16 characters\n Enter UUID of device: ";
                packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0); //TODO vector
                //vecUUID.clear();
                packet_size = recv(ClientConn, vecUUID.data(), BUFF_SIZE, 0);
            } while (vecUUID.size() != 16);
            //std::string sName = std::string(cName);

            std::string sName(vecName.begin(), std::find(vecName.begin(), vecName.end(), '\n'));
            for (int i = 0; i < vecUUID.size(); ++i) { uuid[i] = vecUUID[i]; }
            Sensor sens(sName, uuid);
            server.addSensor(sens);
            strToClient = "Sensor added successfuly";
            packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0);
        }

        patternStr = "anxiety query";
        if (message == patternStr) {
            std::vector<char> vecName(BUFF_SIZE);
            std::string strToClient = "Enter device name: ";

            packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0);
            packet_size = recv(ClientConn, vecName.data(), BUFF_SIZE, 0);

            bool axiety;
            std::vector<Sensor> vecSens = server.getSensors();
            std::string strName(vecName.begin(), std::find(vecName.begin(), vecName.end(), '\n'));
            strToClient = "There is no sensor with this name";
            for (int i = 0; i < vecSens.size(); ++i) {
                if (strName == vecSens[i].getName()) {
                    axiety = vecSens[i].Anxiety();
                    strToClient = axiety;
                    continue;
                }
            }
            packet_size = send(ClientConn, strToClient.data(), BUFF_SIZE, 0);
        }

        patternStr = "devises info";
        if (message == patternStr) {
            std::pair<std::vector<BiometricScanner>, std::vector<Sensor>> infoPair = server.Info();
            std::string strToClient;

            for (int i = 0; i < infoPair.first.size(); ++i) {
                strToClient.append(infoPair.first[i].GetStrFields());
                strToClient.append("\n"); //'\n'
            }

            packet_size = send(ClientConn, strToClient.data(), BUFF_SIZE, 0);
        }
        //Устройство «Биометрический сканер» поддерживает метод проверки биометрии.На входе - два массива байт : биометрические данные(9
        //    байт) и биометрический эталон(9 байт), на выходе - результат проверки : true или false.Значение выбирается случайным образом.
        patternStr = "biometric test";
        if (message == patternStr) {
            std::vector<char> vecName(BUFF_SIZE);
            char* bioStandart = new char[9];
            char* bioData = new char[9];
            std::string strToClient = "Enter device name: ";

            packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0);
            packet_size = recv(ClientConn, vecName.data(), BUFF_SIZE, 0);

            bool result;
            std::vector<BiometricScanner> vecScans = server.getScanners();
            std::string strName(vecName.begin(), std::find(vecName.begin(), vecName.end(), '\n'));
            strToClient = "There is no sensor with this name";
            for (int i = 0; i < vecScans.size(); ++i) {
                if (strName == vecScans[i].getName()) {
                    result = vecScans[i].BioTest(bioStandart, bioData);
                    strToClient = result;
                    continue;
                }
            }
            packet_size = send(ClientConn, strToClient.data(), BUFF_SIZE, 0);
        }

        patternStr = "delete biometrical scanner";
        if (message == patternStr) {
            std::vector<char> vecName(BUFF_SIZE);
            std::string strToClient = "Enter device name: ";

            packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0);
            packet_size = recv(ClientConn, vecName.data(), BUFF_SIZE, 0);

            std::vector<BiometricScanner> vecScans = server.getScanners();
            std::string strName(vecName.begin(), std::find(vecName.begin(), vecName.end(), '\n'));
            strToClient = "There is no sensor with this name";
            for (int i = 0; i < vecScans.size(); ++i) {
                if (strName == vecScans[i].getName()) {
                    strToClient = "device removed";
                    vecScans.erase(vecScans.begin() + i);
                    continue;
                }
            }
            packet_size = send(ClientConn, strToClient.data(), BUFF_SIZE, 0);
        }

        patternStr = "delete sensor";
        if (message == patternStr) {
            std::vector<char> vecName(BUFF_SIZE);
            std::string strToClient = "Enter device name: ";

            packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0);
            packet_size = recv(ClientConn, vecName.data(), BUFF_SIZE, 0);

            std::vector<Sensor> vecSens = server.getSensors();
            std::string strName(vecName.begin(), std::find(vecName.begin(), vecName.end(), '\n'));
            strToClient = "There is no sensor with this name";
            for (int i = 0; i < vecSens.size(); ++i) {
                if (strName == vecSens[i].getName()) {
                    strToClient = "device removed";
                    vecSens.erase(vecSens.begin() + i);
                    continue;
                }
            }
            packet_size = send(ClientConn, strToClient.data(), strToClient.length(), 0);
        }
        packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);

        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
            closesocket(ServSock);
            closesocket(ClientConn);
            WSACleanup();
            return 1;
        }
    }
    //===
    return 0;
}

int main() {
    const short BUFF_SIZE = 1024;

    //Инициализация сокетных интерфейсов
    WSADATA wsData;

    int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (erStat != 0) {
        std::cout << "Error WinSock version initializaion #";
        std::cout << WSAGetLastError();
        return 1;
    }
    else {
        std::cout << "WinSock initialization is OK" << std::endl;
    }
    //===
    //Инициализация сокета
    SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

    if (ServSock == INVALID_SOCKET) {
        std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
        std::cout << "Server socket initialization is OK" << std::endl;
    //===
    //Привязка сокета к паре IP-адрес/Порт
    in_addr ip_to_num;
    erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
    if (erStat <= 0) {
        std::cout << "Error in IP translation to special numeric format" << std::endl;
        return 1;
    }

    sockaddr_in servInfo;
    ZeroMemory(&servInfo, sizeof(servInfo));

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(1234);

    erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));
    if (erStat != 0) {
        std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else {
        std::cout << "Binding socket to Server info is OK" << std::endl;
    }
    //===
    //Прослушивание привязанного порта для идентификации подключений
    erStat = listen(ServSock, SOMAXCONN);

    if (erStat != 0) {
        std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else { std::cout << "Listening..." << std::endl; }
    //===
    //Подтверждение подключения
    sockaddr_in clientInfo;

    ZeroMemory(&clientInfo, sizeof(clientInfo));

    int clientInfo_size = sizeof(clientInfo);

    SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

    if (ClientConn == INVALID_SOCKET) {
        std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        closesocket(ClientConn);
        WSACleanup();
        return 1;
    }
    else {
         std::cout << "Connection to a client established successfully" << std::endl;
    }
    //===
    //Передача между клиентом и сервером
    messageInterpretator(ServSock, ClientConn);
    //===
    return 0;
}

/*SOCKET Connections[100]; //массив сокетов
int Counter = 0; //индекс соединения

enum Packet {
    P_ChatMessage,
    P_Test
};

char* GetInfo(Packet packettype, int index) {
    //char* NumberOfCommand;
    //int recv(Connections[index], (char*)&NumberOfCommand, NULL);
    int DataSize;
    recv(Connections[index], (char*)&DataSize, sizeof(int), NULL);
    char* Data = new char[DataSize+1]; //+1
    Data[DataSize] = '\0';
    recv(Connections[index], Data, DataSize, NULL);
    //int recv(SOCKET <сокет акцептованного соединения>, char[] <буфер для приёма информации с другой стороны>, int <размер буфера>, <флаги>)
    std::cout << *Data << std::endl;
    return Data;
}
bool SendInfo(Packet packettype) {
    //int send(SOCKET <сокет акцептованного соединения>, char[] <буфер хранящий отсылаемую информацию>, int <размер буфера>, <флаги>)
    [&packettype](size_t index) {return; };
    return true;
}


void ClientHandler(int index) {
    Packet packettype;
    while (true) {
        recv(Connections[index], (char*)&packettype, sizeof(Packet), NULL); //принимаем пакет и записываем в переменную

        if (!GetInfo(packettype, index)) {//передаем на обработку
            break;
        }
    }
    closesocket(Connections[index]); //закрываем соке в случае чего
}

int main(int argc, char* argv[]) {
    //WSAStartup
    WSAData wsaData; //структура wsadata
    WORD DLLVersion = MAKEWORD(2, 1); //запрашиваемая версия библиотеки winsoc
    if (WSAStartup(DLLVersion, &wsaData) != 0) { //загружаем библиотеку
        std::cout << "Error" << std::endl;
        exit(1);
    }

//информация об адресе сокета
    SOCKADDR_IN addr; //структура для хранения адреса
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //sin_addr структура для хранения адреса 
    addr.sin_port = htons(1111); // sin_port порт для идентефикации программы
    addr.sin_family = AF_INET; //семейство интернет протоколов

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //привязываем адрес сокету 
    listen(sListen, SOMAXCONN); //прослушивание порта в ожидании клиента

 //сокет для удержания соединения с клиентами
    SOCKET newConnection;
    for (int i = 0; i < 100; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); //приняли соединение

        if (newConnection == 0) {
            std::cout << "Error #2\n";
        }
        else {
            std::cout << "Client Connected!\n";
            std::string msg = "You're connected";
                int msg_size = msg.size(); 
            Packet msgtype = P_ChatMessage;
            send(newConnection, (char*)&msgtype, sizeof(Packet), NULL); //обязательно ли передавать все?
            send(newConnection, (char*)&msg_size, sizeof(int), NULL);
            send(newConnection, msg.c_str(), msg_size, NULL);

            Connections[i] = newConnection; //записываем соединение в массив
            Counter++; //инкриментируем индекс
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL); //поток для принятия новых сообщений

            //char NumberOfCommand;
            
            //recv(newConnection, )
        }
    }
    _getch();
    return 0;
}*/

//bool ProcessPacket(int index, Packet packettype) { //обработка пакета
//    switch (packettype) {
//    case P_ChatMessage:
//    {
//        int msg_size;
//        recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
//        char* msg = new char[msg_size + 1];
//        msg[msg_size] = '\0';
//        recv(Connections[index], msg, msg_size, NULL);
//        for (int i = 0; i < Counter; i++) {
//            if (i == index) {
//                continue;
//            }
//
//            Packet msgtype = P_ChatMessage;
//            send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);//отправляем тип пакета
//            send(Connections[i], (char*)&msg_size, sizeof(int), NULL); //сообщения клиентам
//            send(Connections[i], msg, msg_size, NULL);
//        }
//        delete[] msg;
//        break;
//    }
//    default:
//        std::cout << "Unrecognized packet: " << packettype << std::endl;
//        break;
//    }
//
//    return true;
//}

/*
Разработать программы сервера и клиента для передачи данных из сервера клиенту

Сервер содержит данные по двум типам устройств: «Биометрический сканер» и «Датчик».

Устройство типа «Датчик» поддерживает метод запроса, текущего состояние по тревоге со значениями: «Да» или «Нет». Значение выбирается случайным образом.

Устройство «Биометрический сканер» поддерживает метод проверки биометрии. На входе - два массива байт: биометрические данные (9
байт) и биометрический эталон (9 байт), на выходе - результат проверки: true или false. Значение выбирается случайным образом.

Все типы устройств должны иметь: идентификатор (UUID), имя  (текстовая строка).

В сервере предусмотреть создание нескольких экземпляров указанных типов устройств.

Программа клиента должна по сети подключаться к серверу,  запрашивать данные по всем экземплярам устройств, выводить
информацию на экран о текущем перечне устройств, вызывать удаленные методы в зависимости от типа устройства и выводить результаты их выполнения на экран.
*/