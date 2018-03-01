#include <iostream>
#include <vector>
#include <chrono>

#include "ThreadPool.h"

int main()
{
    
    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cerr << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cerr << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cerr << result.get() << ' ';
    std::cerr << std::endl;
    
    return 0;
}
