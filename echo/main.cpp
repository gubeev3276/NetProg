#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main() {
	struct sockaddr_in {
 		short family;
 		unsigned short port;
 		struct in_addr addr;
 		char zero[8];
	};
	struct in_addr {
 		unsigned long s_addr;
	};
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Создаем сокет  использование IPv4 протокола,  использование TCP протокола
    if (serverSocket == -1) {
        cerr << "Ошибка создания сокета\n";
        return 1;
    }

    
    struct sockaddr_in serverAddr; // Структура для настройки сервера
    serverAddr.family = AF_INET;
    serverAddr.port = htons(7777);
    serverAddr.addr.s_addr = 0;

   
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) { // Привязка сокета к адрессу
        cerr << "Ошибка соединения сокета с адрессом\n";
        close(serverSocket);
        return 1;
    }

    
    if (listen(serverSocket, 10) == -1) { // Сокет переводится в режим прослушивания
                                          // максимальное количество входящих соединений в очереди - 10
        cerr << "Ошибка сокета\n";
        close(serverSocket);
        return 1;
    }

    cout << "Чтение с порта 7777...\n";

    while (true) {
        // Принятие входящего соединения
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            cerr << "Ошибка соединения\n";
            continue;
        }

        cout << "Клиент подключен " << inet_ntoa(clientAddr.addr) << ":" << ntohs(clientAddr.port) << endl;

        // Обработка данных от клиента
        char buffer[1024];
        ssize_t bytesRead;

        while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
            cout << "Получено: " << buffer;
            // Отправка обратно клиенту
            send(clientSocket, buffer, bytesRead, 0);
            memset(buffer, 0, sizeof(buffer));
        }

        if (bytesRead == 0) {
            // Соединение было закрыто клиентом
            cout << "Клиент отключен\n";
        } else if (bytesRead == -1) {
            cerr << "Ошибка приема\n";
        }

        // Закрытие сокета клиента
        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}
