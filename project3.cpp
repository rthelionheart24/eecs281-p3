// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#include <iostream>
#include <string>

#include "Manager.h"

using namespace std;

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    /**
     * Set up variables needed to run the simulation
     */
    int a = argc;
    a++;

    if (string(argv[1]) == "--help" || string(argv[1]) == "h")
    {
        cout << "help\n";
        exit(0);
    }

    string filename = argv[1];
    ifstream in(filename);

    Manager log_manager;

    log_manager.read_logs(in);

    log_manager.process_CMD();

    return 0;
}
