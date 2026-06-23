//
// Created by dustyn on 6/14/26.
//

#include "encryption.h"

#include <iostream>
#include <ostream>
#include <string>
#if defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
#include <sys/mman.h>
#endif

#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
namespace Encryption {
    /*
     *  This SHOULDNt be an issue generally speaking for an application like this that just runs does something interactive then exits, but
     *  we should be cautious anyway and ensure that it cannot be paged to disk with sensitive key material or plaintext password.
     */
    void lock_memory() {
#if defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
        if (mlockall(MCL_CURRENT | MCL_FUTURE) <= 0) {
            // lock all current and future pages into main memory
            std::cerr <<
                    "mlockall failed, exiting early for security purposes. Check memory availability/consumption and try again."
                    << std::endl;
            exit(1);
        }


#elif defined(_WIN32)
        VirtualLock(ptr, size);
        return 1;
#else
        std::cout <<
                "Memory lock cannot be attained, this is not a massive risk since this program is short lived, but paging to disk can compromise program security"
                << std::endl;
        return 0;
#endif
    }

//Check if we are actually in a terminal environment
    bool stdin_terminal() {
#ifdef WIN32
        return _isatty(_fileno(stdin));
#else
        return isatty(fileno(stdin));
#endif
    }

    void set_stdin_echo(bool enable) {
        if (!stdin_terminal()) {
            return;
        }
#ifdef WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        if (!GetConsoleMode(hStdin, &mode)) {
            fputs("GetConsoleMode failed\n", stderr);
            return;
        }
        if (!enable) {
            mode &= ~ENABLE_ECHO_INPUT;
        } else {
            mode |= ENABLE_ECHO_INPUT;
        }
        if (!SetConsoleMode(hStdin, mode)) {
            fputs("SetConsoleMode failed\n", stderr);
        }
#else
        struct termios tty;
        if (tcgetattr(STDIN_FILENO, &tty) != 0) {
            fputs("tcgetattr failed\n", stderr);
            return;
        }
        if (!enable) {
            tty.c_lflag &= static_cast<decltype(tty.c_lflag)>(~ECHO);
        } else {
            tty.c_lflag |= ECHO;
        }
        if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) != 0) {
            fputs("tcsetattr failed\n", stderr);
        }
#endif
    }

    void EncryptionContext::receive_passphrase() {
        set_stdin_echo(false);
        std::string current_DEFCON;

        switch (this->current_defcon) {
            case Defcon::DEFCON1:
                current_DEFCON = std::string{"DEFCON1"};
                break;
            case Defcon::DEFCON2:
                current_DEFCON = std::string{"DEFCON2"};
                break;
            case Defcon::DEFCON3:
                current_DEFCON = std::string{"DEFCON3"};
                break;
            case Defcon::DEFCON4:
                current_DEFCON = std::string{"DEFCON4"};
                break;
            case Defcon::DEFCON5:
                current_DEFCON = std::string{"DEFCON5"};
                break;
        }
        std::cout << "Please enter your password, the DEFCON level password you must provide is " << current_DEFCON <<
                ":" << std::endl;
        std::cin >> passphrase;

        set_stdin_echo(true);
    }
};
