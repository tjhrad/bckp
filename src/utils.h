#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <iostream>

int check_if_directory_exists(const std::filesystem::path& path, std::filesystem::file_status status);
int unidirectional_directory_backup(std::string directory1, std::string directory2);

#endif 
