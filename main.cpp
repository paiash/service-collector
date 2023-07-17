#include "serviceCollector.cpp"

int main(int argc, char* argv[]) {
    std::string triggerDir;
    std::string outputDir;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], TRIGGER_DIR_FLAG.c_str()) == 0 && i + 1 < argc) {
            triggerDir = argv[i + 1];
        } else if (std::strcmp(argv[i], OUTPUT_DIR_FLAG.c_str()) == 0 && i + 1 < argc) {
            outputDir = argv[i + 1];
        }
    }
 
    if (triggerDir.empty() || outputDir.empty()) {
        std::cerr << "Usage: ./system_info_collector --trigger-dir <trigger directory> --output-dir <output directory>"
                  << std::endl;
        return 1;
    }

    if (!fs::exists(triggerDir)) {
        std::cerr << "Trigger directory does not exist: " << triggerDir << std::endl;
        return 1;
    }

    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    observeDirectory(triggerDir, outputDir);

    return 0;
}

