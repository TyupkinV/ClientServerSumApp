#include "client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;

Client::Client(Client::MODE mode)
{
    _ip_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (mode == Client::TCP){
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        _data_addr = {AF_INET, htons(1488), _ip_addr, {0}};
    }else {
        _socket = socket(AF_INET, SOCK_DGRAM, 0);
        _data_addr = {AF_INET, htons(1337), _ip_addr, {0}};
    }
    if (_socket <= 0){
        perror("CREATE SOCKET ERROR");
        cout << endl << "Error code:" << errno << endl;
        exit(EXIT_FAILURE);
    }
}

void Client::CreateConnection(){
    if(connect(_socket, reinterpret_cast<sockaddr *>(&_data_addr), sizeof(_data_addr)) == -1){
        perror("CREATE CONNECTION ERROR");
        cout << endl << "Error code:" << errno << endl;
        exit(EXIT_FAILURE);
    }
}

void Client::SendMessage(char * sendMessage){

    int test = send(_socket, sendMessage, strlen(sendMessage), 0);
    if(test < 0){
        perror("SEND");
        exit(EXIT_FAILURE);
    }
    return;
}

void Client::AnswerServer(){

    ReceiveMessage(_socket);
    ReceiveMessage(_socket);
    }

void Client::ReceiveMessage(const int& fd){
    string result;
    char buff[2] = "-";
    while ((*buff) != '\0') {
        if(recv(fd, buff, 1, 0) < 0){
            perror("RECV");
            exit(EXIT_FAILURE);
        }
        result += buff[0];
    }
    cout << result << endl;
}

