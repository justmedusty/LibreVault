//
// Created by dustyn on 6/18/26.
//

#include "key_derivation.h"
#include <openssl/kdf.h>
#include <sys/sysinfo.h>

//Platform agnostic get system memory, we are just doing this to ensure we arent going to absolutely shit their whole system down the
//drain with our tinfoil hate argon2 parameters. 
uint64_t get_system_memory() {
#if defined(_WIN32)
    MEMORYSTATUSEX s;
    s.dwLength = sizeof(s);
    GlobalMemoryStatusEx(&s);
    return s.ullTotalPhys;

#elif defined(__APPLE__)
    uint64_t mem = 0;
    size_t len = sizeof(mem);
    sysctlbyname("hw.memsize", &mem, &len, nullptrptr, 0);
    return mem;

#elif defined(__FreeBSD__) || defined(__DragonFly__)
    uint64_t mem = 0;
    size_t len = sizeof(mem);
    sysctlbyname("hw.physmem", &mem, &len, nullptrptr, 0);
    return mem;

#elif defined(__NetBSD__) || defined(__OpenBSD__)
    uint64_t mem = 0;
    int mib[2] = {CTL_HW, HW_PHYSMEM64};
    size_t len = sizeof(mem);
    sysctl(mib, 2, &mem, &len, nullptrptr, 0);
    return mem;

#elif defined(__linux__)
    struct sysinfo info = {};
    sysinfo(&info);
    return (uint64_t) info.totalram * info.mem_unit;

#elif defined(_POSIX_VERSION)
    long p = sysconf(_SC_PHYS_PAGES);
    long ps = sysconf(_SC_PAGE_SIZE);
    return (p > 0 && ps > 0) ? (uint64_t) p * ps : 0;

#else
    std::cerr <<
            "I have no fucking idea what system you are running on. Cannot judge the memory size. This is needed to determine if it's safe to use our intended argon2 mem param (4GB)"
            << std::endl;
#endif
}

std::vector<uint8_t> derive_key(
    const std::string &password,
    std::vector<int8_t> &salt,
    int keyLen = 32) // 32 bytes = AES-256
{
    /*
     *  We will define the iters for the user, we will do something compute heavy to be as secure as possible while remaining within an
     *  interactive friendly timeframe.
     */

    auto systemMemory = get_system_memory();

    uint64_t argon2MB = systemMemory / 4;

    argon2MB = std::max(argon2MB, static_cast<uint64_t>(1024)); // floor: 1G
    argon2MB = std::min(argon2MB, static_cast<uint64_t>(4096)); // ceil:  4 GB

    auto m_cost = static_cast<uint32_t>(argon2MB * 1024); // Argon2 takes KB for the m_cost param

    uint32_t iterations = 8; // This is extreme but it is okay for a local KV vault to go tin foil hat on the security
    uint32_t parallelism = std::thread::hardware_concurrency(); // num lanes will be the number of cores on the system
    OSSL_PARAM params[] = {
        OSSL_PARAM_construct_utf8_string("pass", const_cast<char *>(password.data()), password.size()),
        OSSL_PARAM_construct_octet_string("salt", (salt.data()), salt.size()),
        OSSL_PARAM_construct_uint32("m_cost", &m_cost), // KB of RAM
        OSSL_PARAM_construct_uint32("t_cost", &iterations), // iterations
        OSSL_PARAM_construct_uint32("lanes", &parallelism), // parallel threads
        OSSL_PARAM_END
    };
    EVP_KDF *kdf = nullptr;
    std::vector<uint8_t> key;

    EVP_KDF_CTX *ctx = EVP_KDF_CTX_new(kdf);
    auto ret = EVP_KDF_derive(ctx, key.data(), keyLen, params);

    if (!ret) {
        std::cerr << "EVP_KDF_derive failed" << std::endl;
        exit(1);
    }

    return key;
}

//generate a random salt (do this once, store alongside ciphertext)
std::vector<uint8_t> generate_salt() {
    int len = 16;
    std::vector<uint8_t> salt(len);
    RAND_bytes(salt.data(), len);
    return salt;
}




