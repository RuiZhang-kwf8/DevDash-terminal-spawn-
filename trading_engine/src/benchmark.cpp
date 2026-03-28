#include "benchmark.h"
#include <iostream>

void Benchmark::start() {
    start_time_ = std::chrono::high_resolution_clock::now();
}

void Benchmark::stop() {
    end_time_ = std::chrono::high_resolution_clock::now();
}

double Benchmark::elapsed_ms() const {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time_ - start_time_);
    return duration.count() / 1000.0;
}

void Benchmark::report(const std::string& label) const {
    std::cout << label << ": " << elapsed_ms() << " ms" << std::endl;
}
