#include <iostream>
#include "server.h"


using namespace std;

int main()
{
    Server test = Server(Server::TCP);
    test.StartServer();
    
    return 0;
}
