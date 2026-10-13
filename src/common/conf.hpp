#pragma once

#include "CLI/CLI.hpp"
#include<log.hpp>

#include<CLI/CLI.hpp>
#include<toml++/toml.hpp>

class ConfigurationLoader {
    struct Configuration {
        std::string internet_protocol = "127.0.0.1";
        int16_t port = 80;

        std::string pub_cert = "";
        std::string priv_cert = "";


        std::string target_url = "google.com";
        int16_t target_port = 80;
        
    } CurrentConfiguration;

    enum class OperationSystem {
        Unknown,
        Linux
    };    

        OperationSystem get_current_OS();

        void set_standard_os_conf();

    public:
        ConfigurationLoader();

        const Configuration& get_conf();

        void set_conf(std::filesystem::path& path);
        int set_conf(CLI::App& app);
};