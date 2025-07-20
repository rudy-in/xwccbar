#pragma once
#include "cxxopts.hpp"
#include <string>
#include <iostream>
#include "version.hpp" // get the version
#include "program_name.hpp"

namespace cli {

    struct Args {
        std::string config_path = "config.toml";
        bool verbose = false;
    };

    inline Args parse(int argc, char** argv) {
        cxxopts::Options options(PROGRAM_NAME, "A GTK app with config and CLI support");

        options.add_options()
            ("c,config", "Path to config file",
                cxxopts::value<std::string>()->default_value("config.toml"))
            ("V,verbose", "Enable verbose output",
                cxxopts::value<bool>()->default_value("false"))
            ("v,version", "Print Version",
                cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
            ("h,help", "Print help");

        try {
            auto result = options.parse(argc, argv);

            if (result.count("help")) {
                std::cout << options.help() << "\n";
                exit(0);
            }

            if (result["version"].as<bool>()) {
                std::cout << PROGRAM_NAME << " version " << PROGRAM_VERSION << "\n";
                exit(0);
            }


            Args args;
            args.config_path = result["config"].as<std::string>();
            args.verbose = result["verbose"].as<bool>();
            return args;

        } catch (const std::exception& e) {
            std::cerr << "CLI parse error: " << e.what() << "\n";
            exit(1);
        }
    }

}
