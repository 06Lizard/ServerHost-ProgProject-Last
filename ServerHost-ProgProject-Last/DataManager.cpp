#include "DataManager.h"

// Messages sometimes not arranged in the txt file because of async
void DataManager::LogMessage(const std::string& data)
{
    if (logFile.is_open() == false)
    {
        // Out means that it is meant to write data to. Add stdd:ios:app if you don't want it to clear every time the program starts. 
        logFile.open(filename, std::ios::out);
        logFile << data << std::endl; // Saves the data.
    }
    else
    {
        logFile << data << std::endl;
    }
}