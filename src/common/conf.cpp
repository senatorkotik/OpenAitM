#include"conf.hpp"
#include "CLI/CLI.hpp"

#include <spdlog/spdlog.h>
#include <toml++/impl/parse_error.hpp>
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

void ConfigurationLoader::set_conf(std::filesystem::path path) {
    try {
        const toml::table tbl = toml::parse_file(path.string());


    if (const auto* server = tbl["Server"].as_table()) {
        if (auto v = (*server)["ip"].value<std::string>())
            CurrentConfiguration.internet_protocol = *v;

        if (auto v = (*server)["port"].value<std::int64_t>())
            CurrentConfiguration.port = static_cast<int>(*v);

        if (auto v = (*server)["pub_cert"].value<std::string>())
            CurrentConfiguration.pub_cert = *v;

        if (auto v = (*server)["priv_cert"].value<std::string>())
            CurrentConfiguration.priv_cert = *v;
    }

    if (const auto* client = tbl["Client"].as_table()) {
        if (auto v = (*client)["url"].value<std::string>())
            CurrentConfiguration.target_url = *v;

        if (auto v = (*client)["port"].value<std::int64_t>())
            CurrentConfiguration.target_port = static_cast<int>(*v);
    }
    }
    catch (toml::parse_error& error) {
        spdlog::error(path.string() + ": "+ error.what());
    }
}

void ConfigurationLoader::set_conf(CLI::App& app) {

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
        home_conf += "/openaitm/config.toml";

        std::string sys_conf = "/etc/openaitm/config.toml";

        set_conf(sys_conf);
        set_conf(home_conf);
    }
}
