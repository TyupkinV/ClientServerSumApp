#include "server.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>

using namespace std;

void NonBlockSocket(const int& socketfd){
    int flags = fcntl(socketfd, F_GETFL, 0);
    fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
}

Server::Server(Server::MODE mode){
    _inaddr.s_addr = htonl(INADDR_LOOPBACK);
    if (mode == Server::UDP){
        _serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
        _sin = {AF_INET, htons(1337), _inaddr, {0}};

    } else{
        _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        _sin = {AF_INET, htons(1488), _inaddr, {0}};
    }
    if(_serverSocket < 0){
        perror("SOCKET");
        shutdown(_serverSocket, SHUT_RDWR);
        cout << errno << endl;
        exit(EXIT_FAILURE);
    }
    //NonBlockSocket(_serverSocket);
    if(bind(_serverSocket, reinterpret_cast<struct sockaddr *>(&_sin), sizeof(sockaddr))){
        perror("BIND");
        shutdown(_serverSocket, SHUT_RDWR);

        exit(EXIT_FAILURE);
    }
    if(listen(_serverSocket, SOMAXCONN)){
        perror("LISTEN");
        shutdown(_serverSocket, SHUT_RDWR);
        exit(EXIT_FAILURE);
    }
}

void Server::StartServer(){
    int countfd, clientSocket;
    struct epoll_event events[20];
    _epollFD = epoll_create(100);
    _ev.data.fd = _serverSocket;
    _ev.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(_epollFD, EPOLL_CTL_ADD, _serverSocket, &_ev) == -1){
        perror("EPOLL_CTL:serverSocket");
        exit(EXIT_FAILURE);
    }
    while (true) {
        countfd = epoll_wait(_epollFD, events, 20, -1);
        if(countfd == -1){
            perror("COUNTFD");
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i < countfd; i++){
            if((events[i].events & EPOLLERR) | (events[i].events & EPOLLHUP)){
                cout << "Error or Freeze conn";
                exit(EXIT_FAILURE);
            }

            if(events[i].data.fd == _serverSocket){
                clientSocket = accept(_serverSocket, NULL, NULL);
                if(clientSocket == -1){
                    perror("ACCEPT");
                    exit(EXIT_FAILURE);
                }
                NonBlockSocket(clientSocket);
                _ev.events = EPOLLIN | EPOLLET;
                _ev.data.fd = clientSocket;
                int b = epoll_ctl(_epollFD, EPOLL_CTL_ADD, clientSocket, &_ev);
                if(b < 0){
                    perror("B");
                    exit(EXIT_FAILURE);
                }
            }else{
                ExchangeData(events[i].data.fd);
            }
        }
    }
}

void Server::ExchangeData(const int& fd){
    char recvBuff[1024];
    string sendBuff;
    long resultRecv = recv(fd, recvBuff, 1024, 0);
    if(resultRecv < 0){
        perror("RECV");
        exit(EXIT_FAILURE);
        }
    if(resultRecv == 0 && errno != EAGAIN){
        shutdown(fd, SHUT_RDWR);
        close(fd);
        return;
    }

    vector<string> extractNumbers;
    ParseString(extractNumbers, recvBuff, resultRecv);
    int resultSum = SortAndCalculateSum(extractNumbers);
    for(size_t i = 0; i < extractNumbers.size(); i++){
        sendBuff = sendBuff + extractNumbers[i] + " ";
    }

    SendMessage(fd, sendBuff.c_str(), sendBuff.length() + 1);
    string temp = to_string(resultSum);
    SendMessage(fd, temp.c_str(), temp.length() + 1);

}

void Server::SendMessage(const int& fd, const char * msg, int size){
    if(send(fd, msg, size, 0) < 0){
        perror("SEND");
        exit(EXIT_FAILURE);
    }
}

void Server::ParseString(vector<string>& numbers, char* text, long sizeStr){
    string currNumber;
    char prevChar = '@';
    for(int i = 0; i < sizeStr; i++){
        if(isdigit(*(text + i))) {
            if(prevChar == '-')
                currNumber += prevChar;
            currNumber += *(text + i);
            continue;
        }
        if(*(text + i) == '-'){
            prevChar = *(text + i);
            continue;
        }
        if (currNumber.length()){
            numbers.push_back(currNumber);
            currNumber.clear();
            prevChar = '@';
        }
    }
    if (currNumber.length()){
        numbers.push_back(currNumber);
        currNumber.clear();
        }
    return;
}

int Server::SortAndCalculateSum(vector<string>& numbers){
    int sum = 0;
    vector<int> intNumbers;
    for(size_t i = 0; i < numbers.size(); i++){
        intNumbers.push_back(stoi(numbers[i]));
    }
    sort(intNumbers.begin(), intNumbers.end(), [](int& a, int& b) -> bool {return a < b;});
    for(int& i : intNumbers){
        sum += i;
    }
    for(size_t i = 0; i < numbers.size(); i++){
        numbers[i] = to_string(intNumbers[i]);
    }
    return sum;
}
