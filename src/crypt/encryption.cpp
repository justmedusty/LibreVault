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
