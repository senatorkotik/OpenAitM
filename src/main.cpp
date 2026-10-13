#include "CLI/CLI.hpp"
#include <stl.hpp>
#include <conf.hpp>
#include <log.hpp>

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
    ConfigurationLoader Conf;
    CLI::App app{"Server configuration"};

    Conf.set_conf(app);

    cout << Conf.get_conf().internet_protocol << endl;

    CLI11_PARSE(app, argc, argv);
    
    return 0;
}