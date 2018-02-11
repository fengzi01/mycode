#include <mutex>

int main(int argc, char *argv[])
{
    std::mutex mutex_; 
    std::lock_guard<std::mutex> lock(mutex_);
    return 0;
}
