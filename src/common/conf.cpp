#include"conf.hpp"
#include "CLI/CLI.hpp"

#include <spdlog/spdlog.h>
#include <toml++/impl/parser.hpp>

ConfigurationLoader::ConfigurationLoader() {
    set_standard_os_conf();
}

ConfigurationLoader::OperationSystem ConfigurationLoader::get_current_OS() {
    #ifdef __linux__
    return ConfigurationLoader::OperationSystem::Linux;
    #else
    return ConfigurationLoader::OperationSystem::Unknown;
    #endif
}

void ConfigurationLoader::set_conf(std::filesystem::path& path) {
    toml::table tbl = toml::parse_file(path.string());
}

int ConfigurationLoader::set_conf(CLI::App& app) {

    app.add_option(
        "--internet_protocol,--ip",
        CurrentConfiguration.internet_protocol,
        "IP address used by the server"
    );

    app.add_option(
        "--port,-p",
        CurrentConfiguration.port,
        "Port used by the server"
    );

    app.add_option(
        "--pub_cert",
        CurrentConfiguration.pub_cert,
        "Public certificate"
    );

    app.add_option(
        "--priv_cert",
        CurrentConfiguration.priv_cert,
        "Private certificate"
    );

    app.add_option(
        "--target_url,-t",
        CurrentConfiguration.target_url,
        "Target URL"
    );

    app.add_option(
        "--target_port",
        CurrentConfiguration.target_port,
        "Target port"
    );

    app.add_option_function<std::string>(
        "--conf",
        [this](const std::string& raw_path) {
            std::filesystem::path path{raw_path};
            this->set_conf(path);
        },
        "Configuration file"
    );
    


    return 0;
}
const ConfigurationLoader::Configuration& ConfigurationLoader::get_conf() {
    return ConfigurationLoader::CurrentConfiguration;
}

void ConfigurationLoader::set_standard_os_conf() {
    ConfigurationLoader::OperationSystem CurrentOperationSystem = get_current_OS();

    if(CurrentOperationSystem == OperationSystem::Unknown) {
        spdlog::error("This operating system is not supported for retrieving system and local configuration. Please use a manually created configuration file");
        return;
    };

    if(CurrentOperationSystem == OperationSystem::Linux) {
        std::string home_conf = std::getenv("XDG_CONFIG_HOME");
        home_conf += "/MITMProxyLite";

        std::string sys_conf = "/etc/MITMProxyLite";

        //load_file_conf(sys_conf);
        //load_file_conf(home_conf);
    }
}
