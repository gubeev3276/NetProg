#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
int main() {
struct sockaddr_in {//для хранения информации о сокете, включая семейство адресов, номер порта и IP-адрес.
 short family;
 unsigned short port;
 struct in_addr addr;
 char zero[8];
};
struct in_addr { // для хранения IP-адреса 
 unsigned long s_addr;
};

int soc = socket(AF_INET, SOCK_DGRAM, 0); // UDP
if(soc == -1) {
    std::cout << "Socket error\n";
    return 1;
}

sockaddr_in * self_addr = new (sockaddr_in); 
self_addr->family = AF_INET; 
self_addr->port = htons(44214); 
self_addr->addr.s_addr = inet_addr("127.0.0.1");

sockaddr_in * srv_addr = new (sockaddr_in); 
srv_addr->family = AF_INET; 
srv_addr->port = htons(44214);
srv_addr->addr.s_addr = inet_addr("127.0.0.1");

int rc = connect(soc,(const sockaddr*)srv_addr, sizeof(sockaddr_in)); // Соединение с сервером
if (rc == -1) {
    std::cout << "Ошибка соединения с сервером!.\n";
    return 1;
}
else {
    std::cout << "Подключаемся к серверу...\n";
    char connection_msg[20] = "Это клиент";
    send(soc, connection_msg, sizeof(connection_msg), 0);
    char daytime[30];
    recv(soc, daytime, sizeof(daytime), 0);
    std::cout << "Daytime поступивший с сервера:  " << daytime << std::endl;
    close(soc);
    return 0;
}
}