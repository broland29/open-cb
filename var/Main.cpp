#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sys/stat.h>

#include "Attack.h"
#include "Board.h"
#include "Move.h"

#pragma warning(disable : 4996)



#include "Server.h"
#include "Validator.h"

int main()
{
    Server server;
    server.connect();

    Validator validator;

    while (true)
    {
        char board[8][8], cmd, reply[SEND_BUFFER_SIZE];

        int ret = server.receiveMessage(board, cmd);
        if (ret != 0)
        {
            std::cout << "Quitting switch case" << std::endl;
            goto _over;
        }

        switch (cmd)
        {
        case 'b':  // board
            validator.validateBoard(board, reply);
            server.sendMessage(reply);
            break;
        case 'e':  // end
            strcpy(reply, "I[VAR] Got end command, quitting");
            server.sendMessage(reply);
            goto _over;
        default:
            strcpy(reply, "I[VAR] Unknown command");
            server.sendMessage(reply);
            std::cout << "Unknown command " << cmd << std::endl;
        }
    }
_over:
    server.close();
    return 0;
}
