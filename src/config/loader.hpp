#pragma once

#include "config.hpp"
#include <toml++/toml.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace config {

inline WindowConfig load_config(const std::string& path) {
    WindowConfig cfg;

    try {
        toml::table tbl = toml::parse_file(path);

        if (auto window = tbl["window"].as_table()) {
            if (auto position = window->get("position")) {
                cfg.position = position->value_or(cfg.position);
            }
        }

    } catch (const toml::parse_error& err) {
        std::cerr << "TOML Parse error: " << err.description()
                  << "\nAt: " << err.source().begin << std::endl;
    } catch (...) {
        std::cerr << "Unknown error loading config." << std::endl;
    }

    return cfg;
}

}