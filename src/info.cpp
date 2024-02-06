#include "info.h"

void print_version()
{
    std::cout << "bckp ";
    std::cout << "version " << version::version_number;
    std::cout << "\n";
}

void print_program_info()
{
    std::cout << "bckp - A simple backup tool."<< "\n" 
                << "Usage: bckp -i /path/to/directory/ -o /path/to/backup/directory/" << "\n"
                << "bckp -h (--help) for additional usage information." << "\n\n";
}

void print_help()
{
    std::cout << "bckp help page.";
    //TODO: ADD help page information.
}

