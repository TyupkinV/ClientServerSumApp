#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <vector>
#include <string>


using namespace std;

class Server{
private:
    int _serverSocket;
    struct sockaddr_in _sin;
    struct in_addr _inaddr;
    int _epollFD;
    struct epoll_event _ev;

public:
    enum MODE {UDP, TCP};
    Server(MODE mode);
    [[noreturn]]void StartServer();
    void ParseString(vector<string>&, char*, long);
    int SortAndCalculateSum(vector<string>&);
    void ExchangeData(const int& fd);
    void SendMessage(const int&, const char *, int);
};

#endif // SERVER_H
