#include <iostream>
#include "../include/game.h"
#include <ijengine/exception.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int rc;

    try
    {
        SoMTD::Game game("SomTD", 640, 480);
        rc = game.run("map001");
    } catch (ijengine::Exception& ex)
    {
        cout << ex.what() << endl;
    }

    return rc;
}
