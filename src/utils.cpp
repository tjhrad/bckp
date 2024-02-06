#include "utils.h"

int check_if_exists(const std::filesystem::path& path, 
                                std::filesystem::file_status status = std::filesystem::file_status{})
{
    if (std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(path))
        return 1;
    else
        return 0;
}

int unidirectional_directory_backup(std::string main_directory, std::string backup_directory)
{
    const std::filesystem::path main_directory_path = main_directory;
    const std::filesystem::path backup_directory_path = backup_directory;

    if (!check_if_exists(main_directory_path))
    {
        std::cout << main_directory << " does not exist!" << std::endl;
        return 0;
    }

    if (!check_if_exists(backup_directory_path))
    {
        std::filesystem::create_directory(backup_directory);
        return 0;
    }
    
    for (const auto & entry : std::filesystem::recursive_directory_iterator(main_directory))
    {
        if ( std::filesystem::is_directory(entry.path()) )
        {
            std::string dir = entry.path().string();
            std::filesystem::path temporary_stem = dir.erase(dir.find(main_directory),main_directory.length());
            std::filesystem::path temporary_backup_directory_path = backup_directory_path / temporary_stem.relative_path();
            if(!check_if_exists(temporary_backup_directory_path))
            {
                std::cout << "Creating directory: " << temporary_backup_directory_path.string() << std::endl;
                std::filesystem::create_directory(temporary_backup_directory_path);
                
            }
        }
        else if ( std::filesystem::is_regular_file(entry.path()) )
        {
            std::string file_path = entry.path().string();
            std::filesystem::path temporary_filename = file_path.erase(file_path.find(main_directory),main_directory.length());
            std::filesystem::path temporary_backup_file_path = backup_directory_path / temporary_filename.relative_path();

            if(!check_if_exists(temporary_backup_file_path))
            {
                std::cout << "Copying file: " << temporary_backup_file_path.string() << std::endl;
                std::filesystem::copy(entry.path(),temporary_backup_file_path);
            }
            else
            {
                std::uintmax_t main_file_size = std::filesystem::file_size(entry.path());
                std::uintmax_t backup_file_size = std::filesystem::file_size(temporary_backup_file_path);
                if (main_file_size != backup_file_size)
                {
                    std::filesystem::remove(temporary_backup_file_path);
                    std::filesystem::copy(entry.path(),temporary_backup_file_path);
                }
            }
        }
        else
        {
            std::cout << "Unknown error with the file: " << entry.path().string() << std::endl;
        }
        
    }

    for (const auto & entry : std::filesystem::recursive_directory_iterator(backup_directory))
    {
        if ( std::filesystem::is_directory(entry.path()) )
        {
            std::string dir = entry.path().string();
            std::filesystem::path temporary_stem = dir.erase(dir.find(backup_directory),backup_directory.length());
            std::filesystem::path temporary_main_directory_path = main_directory_path / temporary_stem.relative_path();
            if(!check_if_exists(temporary_main_directory_path))
            {
                std::cout << "Removing directory: " << entry.path() << std::endl;
                std::filesystem::remove_all(entry.path());
                
            }
        }
        else if ( std::filesystem::is_regular_file(entry.path()) )
        {
            std::string file_path = entry.path().string();
            std::filesystem::path temporary_filename = file_path.erase(file_path.find(backup_directory),backup_directory.length());
            std::filesystem::path temporary_main_file_path = main_directory_path / temporary_filename.relative_path();

            if(!check_if_exists(temporary_main_file_path))
            {
                std::cout << "Removing file: " << entry.path().string() << std::endl;
                std::filesystem::remove(entry.path());
            }
        }
        else
        {
            std::cout << "Unknown error with the file: " << entry.path().string() << std::endl;
        }
        
    }

    return 1;
}

std::string get_stem(const std::filesystem::path& p) 
{ 
    return p.stem().string(); 
}

std::string get_parent_path(const std::filesystem::path& p) 
{ 
    return p.parent_path().string(); 
}