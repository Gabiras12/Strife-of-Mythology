#include <iostream>
#include "../include/game.h"
#include <ijengine/exception.h>

using namespace std;

int main()
{
    int rc;

    try
    {
        SoMTD::Game game("SomTD", 1024, 700);
        rc = game.run("mainmenu");
    } catch (ijengine::Exception& ex)
    {
        cout << ex.what() << endl;
    }

    return rc;
}
