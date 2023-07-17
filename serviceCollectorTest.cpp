#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>

#include "serviceCollector.cpp"

class SystemInfoCollectorTest : public ::testing::Test {
protected:
    std::string triggerDir = "test_trigger_dir";
    std::string outputDir = "test_output_dir";

    void SetUp() override {
        std::filesystem::create_directories(triggerDir);
        std::filesystem::create_directories(outputDir);
    }

    void TearDown() override {
        std::filesystem::remove_all(triggerDir);
        std::filesystem::remove_all(outputDir);
    }
};

TEST_F(SystemInfoCollectorTest, CollectSystemInformation) {

    std::string filePath = triggerDir + "/core.ServiceName.3057.57dd721409bc4ab4b38a3c33a36a608a.3717.1647975805000000.lz4";
    std::ofstream file(filePath);
    file.close();


    collectSystemInformation(filePath);

    // Assert
    // Add your assertions here to verify the collected system information
    // Example: ASSERT_...(...);
}

TEST_F(SystemInfoCollectorTest, CreateTarArchive) {
    // Arrange
    std::string sourceDir = triggerDir;
    std::string outputFilePath = getOutputFilePath(outputDir);

    // Act
    createTarArchive(sourceDir, outputDir);

    // Assert
    // Add your assertions here to verify the creation of the tar archive
    // Example: ASSERT_...(...);
}

TEST_F(SystemInfoCollectorTest, IsTriggerFile) {

    std::string triggerFileName = "core.ServiceName.3057.57dd721409bc4ab4b38a3c33a36a608a.3717.1647975805000000.lz4";
    std::string nonTriggerFileName = "non_trigger_file.txt";

    bool isTrigger1 = isTriggerFile(triggerFileName);
    bool isTrigger2 = isTriggerFile(nonTriggerFileName);

    ASSERT_TRUE(isTrigger1);
    ASSERT_FALSE(isTrigger2);
}


TEST_F(SystemInfoCollectorTest, ObserveDirectory) {

    std::string filePath = triggerDir + "/core.ServiceName.3057.57dd721409bc4ab4b38a3c33a36a608a.3717.1647975805000000.lz4";
    std::ofstream file(filePath);
    file.close();

    std::thread observeThread([&]() {
        observeDirectory(triggerDir, outputDir);
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Create an assertion or validation here to verify the monitored event
    // Example: ASSERT_...(...);

    observeThread.detach();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

