#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <iostream>

int check_if_directory_exists(const std::filesystem::path& path, std::filesystem::file_status status);
void compare_directories(std::string directory1, std::string directory2);
std::string get_stem(const std::filesystem::path& p);
std::string get_parent_path(const std::filesystem::path& p); 


#endif 
