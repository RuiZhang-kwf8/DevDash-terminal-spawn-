#pragma once
#include <chrono>
#include <string>

class Benchmark {
public:
    void start();
    void stop();
    double elapsed_ms() const;
    void report(const std::string& label) const;

private:
    std::chrono::high_resolution_clock::time_point start_time_;
    std::chrono::high_resolution_clock::time_point end_time_;
};
