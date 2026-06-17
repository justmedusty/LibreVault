#include <iostream>
#include <config/config_representation.h>

int main(int argc, char **argv) {
    const std::vector<std::string> arguments(argv + 1, argv + argc);
    auto config = new ConfigRepresentation();
    config->parse_command_line_args(arguments);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
