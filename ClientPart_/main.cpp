#include <iostream>
#include "client.h"

using namespace std;

int main(int argc, char* args[])
{
    Client user = Client(Client::TCP);
    user.CreateConnection();
    user.SendMessage(args[1]);
    user.AnswerServer();
    return 0;
}
