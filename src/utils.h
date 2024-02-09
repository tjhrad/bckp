#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int check_if_directory_exists(const std::filesystem::path& path, std::filesystem::file_status status);
int unidirectional_directory_backup(std::string main_directory, std::string backup_directory, std::string log_file, bool ignore_hidden);
void save_vector_to_file(std::string file_name,std::vector<std::string> file_data);

#endif 
