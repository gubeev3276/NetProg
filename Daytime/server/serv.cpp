
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main(int argc, char* argv[]) {
struct sockaddr_in {
 short family;
 unsigned short port;
 struct in_addr addr;
 char sin_zero[8];
};
//Задаем структуры для определения адресов сокетов
struct in_addr {
 unsigned long addr;
};

int sok = socket(AF_INET, SOCK_DGRAM, 0); // Создаем сокет
if (sok == -1) {
    cout << "Ошибка с сокетом\n";
}
// Заполнение структуры 
sockaddr_in * self_addr = new (sockaddr_in);
self_addr->family = AF_INET;
self_addr->port = htons(44214);
self_addr->addr.s_addr = inet_addr("127.0.0.1");

//Указатель на структуру для привзяки сокета к порту и адресу
int b = bind(sok,(const sockaddr*) self_addr,sizeof(sockaddr_in)); //Связываем с помощью bind локальный адресс с сокетом
if(b == -1) {
    cout << "Не удалось связаться\n";
    return 1;
}
while(true) {
    char connect[50];
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string day, monday, hour, minute, second;
    string daytime;
    ltm->tm_mday < 10 ? day = '0' + to_string(ltm->tm_mday) : day = to_string(ltm->tm_mday);
    ltm->tm_mon < 10 ? monday = '0' + to_string(ltm->tm_mon + 1) : monday = to_string(ltm->tm_mon + 1);
    ltm->tm_hour < 10 ? hour = '0' + to_string(ltm->tm_hour) : hour = to_string(ltm->tm_hour);
    ltm->tm_min < 10 ? minute = '0' + to_string(ltm->tm_min) : minute = to_string(ltm->tm_min);
    ltm->tm_sec < 10 ? second = '0' + to_string(ltm->tm_sec) : second = to_string(ltm->tm_sec);
    daytime = day + '.' + monday + '.' + to_string(ltm->tm_year + 1900) + ' ' + hour + ':' + minute + ':' + second;
    char daytime_msg[sizeof(daytime)];
    strcpy(daytime_msg, daytime.c_str());
    sockaddr_in * client_addr = new sockaddr_in;
    socklen_t len = sizeof (sockaddr_in);
    if(recvfrom(sok, connect, sizeof(connect), 0, (sockaddr*)(client_addr), &len)) { // recvfrom для получения данных от клиента
        cout << "Успешный коннект с клиентом\n";
        sendto(sok, daytime_msg, sizeof(daytime_msg), 0, (sockaddr*)(client_addr), len); //  sendto для отправки строки с датой и временем обратно клиенту
        cout << "Daytime был отправлен клиенту\n";
    }
}
close(sok);
return 0;
}