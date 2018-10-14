#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <string>

using namespace std;

class Client
{
    static const int MAX_LEN_MSG = 1024;
private:
    int _socket;
    string _textMessage;
    struct sockaddr_in _data_addr;
    struct in_addr _ip_addr;
public:
    enum MODE {UDP, TCP};
    Client(MODE mode);
    void CreateConnection();
    void SendMessage(char *);
    void AnswerServer();
    void ReceiveMessage(const int&);
    void CloseConn();
};

#endif
