#include<winsock2.h>
#include<iostream>
#include<string>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[]) {

    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsdata;
    if (WSAStartup(sockVersion, &wsdata) != 0)
    {
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Socket error" << endl;
        return 1;
    }


    sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(8888);
    sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
        cout << "Bind error" << endl;
        return 1;
    }

    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        cout << "Listen error" << endl;
        return 1;
    }


    SOCKET clientSocket;
    sockaddr_in client_sin;
    char msg[100];//存储传送的消息
    int flag = 0;//是否已经连接上
    int len = sizeof(client_sin);
    while (true) {
        if (!flag)
            cout << "wait" << endl;
        clientSocket = accept(serverSocket, (sockaddr*)&client_sin, &len);
        if (clientSocket == INVALID_SOCKET) {
            cout << "Accept error" << endl;
            flag = 0;
            return 1;
        }
        if (!flag)
            cout << "receive" << endl;
        flag = 1;
        int num = recv(clientSocket, msg, 100, 0);
        if (num > 0)
        {
            msg[num] = '\0';
            cout << "Client say: " << msg << endl;

        }

        string data;
        getline(cin, data);
        const char* sendData;
        sendData = data.c_str();
        send(clientSocket, sendData, strlen(sendData), 0);
        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();



    return 0;
}