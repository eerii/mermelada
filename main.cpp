//* fresa, a tiny engine by jose pazos perez

#include "fresa_types.h"
#include "fresa_test.h"

using namespace fresa;

inline TestSuite suite("Some tests", []{
    "Test one"_test = []{ return expect(1 == 1); };
    "Test two"_test = []{ return expect(1 == 2); };
});

int main (int argv, char** args) {
    test_runner.run();

    return 0;
}