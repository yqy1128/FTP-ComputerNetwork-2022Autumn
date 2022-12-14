#include<winsock2.h>
#include<iostream>
#include<string>
#include<fstream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER_SIZE 1024

SOCKET clientSocket;
//char recvdata[1024];

typedef struct Packet {
    char buffer[BUFFER_SIZE];
};

void sendInstr(string instr) {
    send(clientSocket, (char*)&instr, sizeof(instr), 0);
}


void pwd() {
    struct Packet p;
    if (int num = recv(clientSocket, (char*)&p, sizeof(struct Packet), 0) > 0) {
        cout << p.buffer << endl;
        return;
    }
}

void readAndSendFile(string filename) {
    cout << "Choose how to transmit your file (enter 'ascii' or 'binary')..." << endl;
    string tran;
    cin >> tran;
    ifstream ifile;
    if (tran == "ascii") {
        ifile.open(filename);
    }
    else if (tran == "binary") {
        ifile.open(filename, ios::binary | ios::in);
    }
    if (ifile.fail()) {
        cout << "Open file error..." << endl;
        return;
    }

    while (ifile) {
        struct Packet p;
        memset(p.buffer, BUFFER_SIZE, 0);

        ifile.read(p.buffer, BUFFER_SIZE);
        if (ifile.gcount() < BUFFER_SIZE) {
            p.buffer[ifile.gcount()] = '\0';
        }
        //p.instr = "put";
        send(clientSocket, (char*)&p, sizeof(struct Packet), 0);

    }
    cout << "File closing..." << endl;
    return;

}

int main()
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0)
    {
        return 1;
    }

    while (true){
        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET){
            cout << "Socket error" << endl;
            return 1;
        }
        sockaddr_in sock_in;
        sock_in.sin_family = AF_INET;
        sock_in.sin_port = htons(8888);
        sock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        if (connect(clientSocket, (sockaddr*)&sock_in, sizeof(sock_in) )== SOCKET_ERROR){
            cout << "Connect error" << endl;
            return 1;
        }
        cout << "Please enter the command: " << endl;
        
        string str,cmd,params;
        getline(cin, str);
        cmd = str.substr(0, str.find(" "));
        params = str.substr(str.find(" ") + 1, str.length() - 1);
        //????????????
        sendInstr(cmd);

        if (cmd =="quit") {
            closesocket(clientSocket);
            cout << "quit!" << endl;
            break;
        }
        else if (cmd == "pwd") {
            pwd();
        }
        else if (cmd == "put") {
            readAndSendFile(params);
        }


        closesocket(clientSocket);

    }

    WSACleanup();

    return 0;
}