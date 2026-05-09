#include <iostream>
#include <string>
#include <filesystem>
#include "json.hpp"
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;
using json = nlohmann::json;

struct Config {
    std::string scan_path;
    int interval_seconds;
};

Config parse_args(int argc, char* argv[]);

void scan_and_save(const std::string& str_home_file_path);

void show_iteration_info(const json& iter_res);

int main(int argc, char* argv[]) {
    Config cfg = parse_args(argc, argv);

    std::cout << "=== Media Scanner Started ===" << std::endl;
    std::cout << "Scanning: " << cfg.scan_path << std::endl;
    std::cout << "Interval: " << cfg.interval_seconds << " seconds" << std::endl;
    std::cout << "Output:   " << std::getenv("HOME") << "/.media_files" << std::endl;
    std::cout << "=============================" << std::endl;

    if (!fs::exists(cfg.scan_path)) {
        std::cerr << "Error: path " << cfg.scan_path << " does not exists" << std::endl;
        return 1;
    }

    while (true) {
        scan_and_save(cfg.scan_path);
        std::this_thread::sleep_for(std::chrono::seconds(cfg.interval_seconds));
    }

    return 0;
}

Config parse_args(int argc, char* argv[]) {
    Config cfg;
    cfg.scan_path = std::getenv("HOME");  // default
    cfg.interval_seconds = 10;             // default
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--path") == 0 && i + 1 < argc) {
            cfg.scan_path = argv[++i];
        }
        else if (strcmp(argv[i], "--interval") == 0 && i + 1 < argc) {
            cfg.interval_seconds = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--help") == 0) {
            std::cout << "Usage: " << argv[0] << " [OPTIONS]\n"
                      << "  --path <dir>      Directory to scan (default: HOME)\n"
                      << "  --interval <sec>  Scan interval in seconds (default: 10)\n"
                      << "  --help            Show this help\n";
            exit(0);
        }
    }
    
    return cfg;
}

void scan_and_save(const std::string& str_home_file_path) {
    fs::path home_dir_path = str_home_file_path;

    json result;

    result["audio"] = json::array();
    result["video"] = json::array();
    result["images"] = json::array();

    std::vector<std::string> audio_ext = {".mp3", ".wav", ".ogg", ".flac"};
    std::vector<std::string> video_ext = {".mp4", ".avi", ".mkv", ".mpg", ".mpeg"};
    std::vector<std::string> image_ext = {".jpg", ".jpeg", ".png", ".gif", ".bmp"};

    if (fs::is_directory(home_dir_path)) {

        for (const auto& entry : fs::recursive_directory_iterator(home_dir_path, fs::directory_options::skip_permission_denied)) {
            if (!entry.is_regular_file()) continue;

            std::string file_name = entry.path().filename();
            std::string file_ext = entry.path().extension();

            if (std::find(audio_ext.begin(), audio_ext.end(), file_ext) != audio_ext.end()) {
                result["audio"].push_back(file_name);
            }

            if (std::find(video_ext.begin(), video_ext.end(), file_ext) != video_ext.end()) {
                result["video"].push_back(file_name);
            }

            if (std::find(image_ext.begin(), image_ext.end(), file_ext) != image_ext.end()) {
                result["images"].push_back(file_name);
            }
        }


    }

    show_iteration_info(result);

    std::ofstream file(std::string(std::getenv("HOME")) + "/.media_files");
    file << result.dump(4);
    file.close();
}

void show_iteration_info(const json& iter_res) {
    std::cout <<"------------------------------------------" <<std::endl;
    std::cout << "Found " << iter_res["audio"].size() << " audio files." << std::endl;
    std::cout << "Found " << iter_res["video"].size() << " video files." << std::endl;
    std::cout << "Found " << iter_res["images"].size() << " image files." << std::endl;
    std::cout <<"------------------------------------------" <<std::endl;
}