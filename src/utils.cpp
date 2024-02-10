#include "utils.h"

int check_if_exists(const std::filesystem::path& path, 
                                std::filesystem::file_status status = std::filesystem::file_status{})
{
    if (std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(path))
        return 1;
    else
        return 0;
}

void save_vector_to_file(std::string file_name,std::vector<std::string> file_data)
{
    std::ofstream file(file_name);
    for (std::string s: file_data)
    {
        file << s << std::endl;
    }
    file.close();
}

int unidirectional_directory_backup(std::string main_directory, std::string backup_directory, std::string log_file="", bool ignore_hidden=false)
{
    const std::filesystem::path main_directory_path = main_directory;
    const std::filesystem::path backup_directory_path = backup_directory;
    //const std::filesystem::path log_file_path = log_file;

    std::vector<std::string> log_file_data;

    if(log_file != "")
        log_file_data.push_back(main_directory + " >>> " + backup_directory);

    std::cout << std::endl <<"Starting backup" << std::endl;
    std::cout << main_directory << " >>> ";
    std::cout << backup_directory << std::endl;
    std::cout << "Log file: " << log_file << std::endl;
    std::cout << "Please wait..." << std::endl;

    int number_files_created = 0;
    int number_directories_created = 0;
    int number_files_updated = 0;
    int number_files_deleted = 0;
    int number_directories_deleted = 0;
    int number_files_compared = 0;

    if (!check_if_exists(main_directory_path))
    {
        std::cout << main_directory << " does not exist!" << std::endl;
        std::cout << "Please make sure the path is correct and try again.";
        return 0;
    }

    if (!check_if_exists(backup_directory_path))
    {
        std::filesystem::create_directory(backup_directory);
    }
    
    auto iter = std::filesystem::recursive_directory_iterator(main_directory, std::filesystem::directory_options::skip_permission_denied);
    auto end_iter = std::filesystem::end(iter);
    auto ec = std::error_code();

    int total_files = 0;
    int current_count = 0;
    for (; iter != end_iter; iter.increment(ec))
        total_files += 1;

    iter = std::filesystem::recursive_directory_iterator(main_directory, std::filesystem::directory_options::skip_permission_denied);
    end_iter = std::filesystem::end(iter);

    for (; iter != end_iter; iter.increment(ec))
    {
        std::cout << (current_count) << " of " << (total_files) << " files." << '\r';
        auto const& entry = iter.operator*();

        if(ignore_hidden && entry.path().string().find("/.") != std::string::npos)
            continue;

        //auto ec = std::error_code();
        if ( std::filesystem::is_directory(entry.path(),ec) )
        {
            std::string dir = entry.path().string();
            std::filesystem::path temporary_stem = dir.erase(dir.find(main_directory),main_directory.length());
            std::filesystem::path temporary_backup_directory_path = backup_directory_path / temporary_stem.relative_path();
            if(!check_if_exists(temporary_backup_directory_path))
            {
                ec.clear();
                std::filesystem::create_directory(temporary_backup_directory_path,ec);
                if(ec.message() == "Success")
                {
                    if(log_file != "")
                        log_file_data.push_back("Created directory: " + temporary_backup_directory_path.string());
                    //std::cout << "Created directory: " << temporary_backup_directory_path.string() << std::endl;
                    number_directories_created += 1;
                    ec.clear();
                }
                
            }
        }
        else if ( entry.is_regular_file() && !entry.is_symlink())
        {
            number_files_compared += 1;
            std::string file_path = entry.path().string();
            std::filesystem::path temporary_filename = file_path.erase(file_path.find(main_directory),main_directory.length());
            std::filesystem::path temporary_backup_file_path = backup_directory_path / temporary_filename.relative_path();

            if(!check_if_exists(temporary_backup_file_path))
            {
                ec.clear();
                std::filesystem::copy_file(entry.path(),temporary_backup_file_path,ec);
                if(ec.message() == "Success")
                {
                    if(log_file != "")
                        log_file_data.push_back("Created file: " + temporary_backup_file_path.string());
                    //std::cout << "Created file: " << temporary_backup_file_path.string() << std::endl;
                    number_files_created += 1;
                    ec.clear();
                }
                
            }
            else
            {
                std::uintmax_t main_file_size = std::filesystem::file_size(entry.path());
                std::uintmax_t backup_file_size = std::filesystem::file_size(temporary_backup_file_path);
                if (main_file_size != backup_file_size)
                {
                    //std::cout << "Updating file: " << entry.path().string() << std::endl;
                    ec.clear();
                    std::filesystem::remove(temporary_backup_file_path,ec);
                    std::filesystem::copy_file(entry.path(),temporary_backup_file_path,ec);
                    if(ec.message() == "Success")
                    {
                        if(log_file != "")
                            log_file_data.push_back("Updated file: " + temporary_backup_file_path.string());
                        //std::cout << "Updated file: " << temporary_backup_file_path.string() << std::endl;
                        number_files_updated += 1;
                        ec.clear();
                    }
                    
                }
            }
        }
        
        if(ec.message() != "Success" && ec.message() != "No such file or directory")
        {
            if(log_file != "")
            {
                log_file_data.push_back("Warning! Something went wrong. error_code: " + ec.message());
                log_file_data.push_back(entry.path().string());
            } 
            //std::cout << "\nWarning! Something went wrong. error_code: " << ec.message() << std::endl;
            //std::cout << entry.path().string() << std::endl;
            ec.clear();
        }
            
        current_count += 1;
    }

    auto iter2 = std::filesystem::recursive_directory_iterator(backup_directory, std::filesystem::directory_options::skip_permission_denied);
    auto end_iter2 = std::filesystem::end(iter2);
    auto ec2 = std::error_code();

    //for (auto const& entry : std::filesystem::recursive_directory_iterator(backup_directory,std::filesystem::directory_options::skip_permission_denied))
    for (; iter2 != end_iter2; iter2.increment(ec2))
    {
        auto const& entry = iter2.operator*();

        if(ignore_hidden && entry.path().string().find("/.") != std::string::npos)
            continue;

        //auto ec = std::error_code();
        if ( std::filesystem::is_directory(entry.path(),ec2) )
        {
            std::string dir = entry.path().string();
            std::filesystem::path temporary_stem = dir.erase(dir.find(backup_directory),backup_directory.length());
            std::filesystem::path temporary_main_directory_path = main_directory_path / temporary_stem.relative_path();
            if(!check_if_exists(temporary_main_directory_path))
            {
                //std::cout << "Removing directory: " << entry.path() << std::endl;
                //std::filesystem::remove_all(entry.path());
                ec2.clear();
                std::filesystem::remove_all(entry.path(),ec2);
                if(ec2.message() == "Success")
                {
                    if(log_file != "")
                        log_file_data.push_back("Removed directory: " + entry.path().string());

                    //std::cout << "Removed directory: " << entry.path() << std::endl;
                    number_directories_deleted += 1;
                    ec2.clear();
                }

            }
        }
        else if ( entry.is_regular_file() && !entry.is_symlink() )
        {
            std::string file_path = entry.path().string();
            std::filesystem::path temporary_filename = file_path.erase(file_path.find(backup_directory),backup_directory.length());
            std::filesystem::path temporary_main_file_path = main_directory_path / temporary_filename.relative_path();

            if(!check_if_exists(temporary_main_file_path))
            {
                number_files_compared += 1;
                //std::filesystem::remove(entry.path());
                ec2.clear();
                std::filesystem::remove(entry.path(),ec2);
                if(ec2.message() == "Success")
                {
                    if(log_file != "")
                        log_file_data.push_back("Removed file: " + entry.path().string());
                    //std::cout << "Removed file: " << entry.path().string() << std::endl;
                    number_files_deleted += 1;
                    ec2.clear();
                }
                
            }
        }
        if(ec2.message() != "Success" && ec2.message() != "No such file or directory")
        {
            if(log_file != "")
            {
                log_file_data.push_back("Warning! Something went wrong. error_code: " + ec2.message());
                log_file_data.push_back(entry.path().string());
            } 
            ec2.clear();
        }

        
    }

    std::cout << "Total # of files: " << number_files_compared << std::endl;
    std::cout << "Files created: " << number_files_created << std::endl;
    std::cout << "Files updated: " <<  number_files_updated << std::endl;
    std::cout << "Files deleted: " <<  number_files_deleted << std::endl;
    std::cout << "Directories created: " <<  number_directories_created << std::endl;
    std::cout << "Directories deleted: " <<  number_directories_deleted << std::endl;

    std::cout << std::endl << "Backup complete!" << std::endl;

    if(log_file != "")
        save_vector_to_file(log_file, log_file_data);

    return 1;
}


