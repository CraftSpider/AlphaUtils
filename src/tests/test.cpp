
#include "tests/test.h"
#include "console.h"

namespace testing {

std::vector<void(*)()> ToRun::tests = std::vector<void (*)()>();

int Results::successes = 0;
int Results::failures = 0;
int Results::errors = 0;
int Results::skipped = 0;

int Results::total() {
    return successes + failures + errors + skipped;
}

int Results::success_percent() {
    return 100 * successes / total();
}

int Results::failure_percent() {
    return 100 * failures / total();
}

int Results::error_percent() {
    return 100 * errors / total();
}

int Results::skipped_percent() {
    return 100 * skipped / total();
}

assertion_failure::assertion_failure(const std::string &msg) : runtime_error(msg) {};

skip_test::skip_test() : runtime_error("Skipped test") {};
skip_test::skip_test(const std::string &msg) : runtime_error(msg) {};

constexpr char fill_char = '=';
constexpr char blank_char = '-';

void run_tests(std::string name) {
    for (auto test : ToRun::tests) {
        test();
    }
    
    using namespace term;
    // Write out basic results
    std::cout << "\n" << name << " testing complete. Results:\n";
    std::cout << "Tests Found: " << Results::total() << "\n";
    std::cout << "Successes: " << fore::GREEN << Results::successes << fore::DEFAULT;
    std::cout << " Failures: " << fore::RED << Results::failures << fore::DEFAULT;
    std::cout << " Errors: " << fore::YELLOW << Results::errors << fore::DEFAULT;
    std::cout << " Skipped: " << fore::LIGHT_BLUE << Results::skipped << fore::DEFAULT << "\n";
    
    // Write out percentage bar
    std::cout << "[" << fore::GREEN;
    for (int i = 0; i < Results::success_percent() / 2; ++i) {
        std::cout << fill_char;
    }
    std::cout << fore::RED;
    for (int i = 0; i < Results::failure_percent() / 2; ++i) {
        std::cout << fill_char;
    }
    std::cout << fore::YELLOW;
    for (int i = 0; i < Results::error_percent() / 2; ++i) {
        std::cout << blank_char;
    }
    std::cout << fore::LIGHT_BLUE;
    for (int i = 0; i < Results::skipped_percent() / 2; ++i) {
        std::cout << blank_char;
    }
    std::cout << fore::DEFAULT << "]\n";
}

}

