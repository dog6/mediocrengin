#pragma once
#include <chrono>
#include "AVGNG/Core/Debug.hpp"

namespace ng {

class ScopedTimer {
public:
    explicit ScopedTimer(const char* name)
        : name(name), start(std::chrono::steady_clock::now()) {}

    ~ScopedTimer() {
        const auto end = std::chrono::steady_clock::now();
        const double ms =
            std::chrono::duration<double, std::milli>(end - start).count();

        ng::Core::Debug::Log(ng::Core::LogLevel::LOG, "%s: %.3f ms", name, ms);
    }

private:
    const char* name;
    std::chrono::steady_clock::time_point start;
};

} // namespace ng

// Helper macros
#define NG_CONCAT_INNER(a, b) a##b
#define NG_CONCAT(a, b) NG_CONCAT_INNER(a, b)

// Times the current function. Put it on the first line.
#define NG_TIME_FUNCTION() ng::ScopedTimer NG_CONCAT(ngTimer_, __LINE__)(__FUNCTION__)

// Times a block with a name that you choose.
#define NG_TIME_SCOPE(name) ng::ScopedTimer NG_CONCAT(ngTimer_, __LINE__)(name)