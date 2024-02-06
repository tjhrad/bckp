#include "utils.h"

int check_if_directory_exists(const std::filesystem::path& path, 
                                std::filesystem::file_status status = std::filesystem::file_status{})
{
    if (std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(path))
        return 1;
    else
        return 0;
}

void compare_directories(std::string directory1, std::string directory2)
{
    const std::filesystem::path path1 = directory1;
    const std::filesystem::path path2 = directory2;

    if (!check_if_directory_exists(path1))
    {
        std::cout << directory1 << " does not exist!\n";
        return;
    }

    if (!check_if_directory_exists(path2))
    {
        std::filesystem::create_directory(directory2);
    }
    
    for (const auto & entry : std::filesystem::recursive_directory_iterator(directory1))
    {
        std::cout << get_parent_path(entry.path()) << std::endl;
    }

    /* else
        std::cout << directory1 << " does not exist\n";

    if (check_if_directory_exists(path2))
        std::cout << directory2 << " exists!\n";
    else
        std::cout << directory2 << " does not exist\n"; */
}

std::string get_stem(const std::filesystem::path& p) 
{ 
    return p.stem().string(); 
}

std::string get_parent_path(const std::filesystem::path& p) 
{ 
    return p.parent_path().string(); 
}