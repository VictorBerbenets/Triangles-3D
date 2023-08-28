#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "unit_tests.hpp"
#include "end2end_tests.hpp"

int main() {
#ifdef UNIT_TESTS
    RUN_UNIT_TESTS();
#else
    RUN_END2END_TESTS();
#endif
}
