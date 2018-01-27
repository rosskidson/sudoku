#include <chrono>

class MyTimer
{
public:
  MyTimer()
  {
    start_ = std::chrono::high_resolution_clock::now();
  }

  ~MyTimer()
  {
    end_ = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end_ - start_ ).count();
    std::cout << duration / 1000.0 << "ms" << std::endl;
  }

private:
  std::chrono::high_resolution_clock::time_point start_;
  std::chrono::high_resolution_clock::time_point end_;

};

MyTimer timer;
