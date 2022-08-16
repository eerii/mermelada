//* fresa, a tiny engine by jose pazos perez

#include "engine.h"

#include "log.h"
#include "reflection.h"
#include "type_name.h"
using namespace fresa;

struct Test {
    int a, b, c;
    float d, e;
    //std::vector<int> v;
    //int* p = nullptr;
    //int aaa[4];
};

int main (int argv, char** args) {
    log::info("fields: {}", field_count_v<Test>);
    Test t{1, 2, 3, 4.5f};
    log::info("t0: {}", get<0>(t));

    for_([&](auto &i) {
        if constexpr (std::is_pointer_v<std::remove_reference_t<decltype(i)>>)
            log::info("pointer");
        else
            log::info("{}", i);
    }, t);

    Test t2{1, 2, 3, 4.5f};
    auto h = std::hash<Test>{}(t2);
    log::info("hash: {}", h);

    std::unordered_map<Test, int> s;

    log::info("{}", Test{} == Test{});

    log::info("{}", field_names<Something>());
    
    fresa::run();
    return 0;
}