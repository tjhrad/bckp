
#include "bckp.h"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        print_program_info();
        return 0;
    }

    InputParser input(argc, argv);
    if(input.cmd_option_exists("-h") || input.cmd_option_exists("--help"))
    {
        print_help();
    }

    if(input.cmd_option_exists("-v") || input.cmd_option_exists("--version"))
    {
        print_version();
        return 0;
    }

    if (input.cmd_option_exists("-i") && input.cmd_option_exists("-o"))
    {

        const std::string input_directory = input.get_cmd_option("-i");
        const std::string backup_directory = input.get_cmd_option("-o");

        unidirectional_directory_backup(input_directory, backup_directory);
    }
    else 
    {
        print_program_info();
        return 0;
    }

    return 0;
}

//TODO: Add -m for modes
// C comparison mode.
// U unidirectional backup mode.
// M multidirectional backup mode (sync). 