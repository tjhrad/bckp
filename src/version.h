#ifndef VERSION_H
#define VERSION_H

#include <string>

namespace version
{

const int major = 0;
const int minor = 1;
const int revision = 0;

const std::string version_number = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision);

}

#endif 
