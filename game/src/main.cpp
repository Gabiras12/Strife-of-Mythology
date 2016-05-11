#include <iostream>
#include "../include/strife_of_mythology.h"
#include <ijengine/exception.h>

int main(int argc, char const *argv[])
{
    int rc;

    try
    {
        StrifeOfMythology game("SomTD", 640, 480);
        rc = game.run("red");
    } catch (Exception& ex)
    {
        cout << ex.what() << endl;
    }

    return rc;
}
