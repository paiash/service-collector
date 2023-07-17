#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <thread>

namespace fs = std::filesystem;

const std::string COLLECTED_DATA_DIR = "collected_data";
const std::string TRIGGER_DIR_FLAG = "--trigger-dir";
const std::string OUTPUT_DIR_FLAG = "--output-dir";

void collectSystemInformation(const std::string& path) {
    // System Information is printed here
    std::cout << "\nCollecting system information for file: " << path << std::endl;
    std::ofstream file("collected_data.txt", std::ios::app);
    if (file.is_open()) {
        file << path;
        file << "\n";
        file.close();
        std::cout << "String appended to file "<< std::endl;
    } else {
        std::cerr << "Failed to open file"<< std::endl;
    }
}

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", std::localtime(&time));
    return std::string(buffer);
}

std::string generateUniqueFileName() {
    return getCurrentTime() + ".tar";
}

std::string getOutputFilePath(const std::string& outputDir) {
    fs::path outputPath(outputDir);
    outputPath /= generateUniqueFileName();
    return outputPath.string();
}

void createTarArchive(const std::string& sourceDir, const std::string& outputDir) {
    std::string outputFilePath = getOutputFilePath(outputDir);
    std::string command = "tar -cf " + outputFilePath + " -C " + sourceDir + " .";
    std::system(command.c_str());
    std::cout << "\nCreated tar archive: " << outputFilePath << std::endl;
}

void handleTriggerEvent(const std::string& filePath, const std::string& outputDir) {
    std::cout << "\nTrigger event detected for file: " << filePath << std::endl;
    collectSystemInformation(filePath);
    createTarArchive(filePath, outputDir);
}

bool isTriggerFile(const std::string& filePath) {
    static const std::regex pattern(R"(core\.[^.]+\.[[:xdigit:]]+(\.[[:xdigit:]]+)*\.lz4)");
    std::smatch match;
    return std::regex_match(filePath, match, pattern);
}

void observeDirectory(const std::string& directory, const std::string& outputDir) {
    fs::path path(directory);
    fs::directory_iterator endIterator;

    while (true) {
        for (fs::directory_iterator it(path); it != endIterator; ++it) {
            const fs::path& filePath = it->path();
            if (fs::is_regular_file(filePath) && isTriggerFile(filePath.filename().string())) {
                handleTriggerEvent(filePath.string(), outputDir);
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

