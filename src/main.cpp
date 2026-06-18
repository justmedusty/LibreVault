#include <iostream>
#include <config/config_representation.h>
#include <unistd.h>

int main(int argc, char **argv) {
#if defined(_WIN32)
    std::cout << "You shouldn't use Windows. Just mentioning that in passing. Now let's get back to the vault activities." <<
            std::endl;
#endif

    const std::vector<std::string> arguments(argv + 1, argv + argc);
    auto config = new ConfigRepresentation();
    config->parse_command_line_args(arguments);

    return 0;
}
