#include <iostream>
#include <chrono>
#include "mingw.thread.h"

using namespace std;
__thread int globalValue;

void helloWorld()
{
  cout << "Hello from thread!\n";
  auto start = chrono::high_resolution_clock::now();
    mingw_stdthread::this_thread::sleep_for(chrono::seconds(5));
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> elapsed = end-start;
    cout << "Waited " << elapsed.count() << " ms\n";
}

void goodbyeWorld()
{
  cout << "Goodbye from thread!\n";
}

void increaseValue(int& value, int times, int thread_number)
{
 globalValue = thread_number;
 for(int i = 0; i < times; i++)
   value++;
 cout << "Global value: " << globalValue << " Value: " << value << " increased for " << times << " times on thread #" << thread_number << "\n";
}

int main()
{
  int a = 0;
  int b = 0;
  globalValue = 3;
  const auto processor_count = hardware_concurrency();
  cout << "cores: " << processor_count << "\n";
  cout << "Global value in main: " << globalValue << "\n";
  //cout << "a = " << a << " b = " << b << "\n";
  system("pause");
  //thread th(helloWorld);
  //thread tg(goodbyeWorld);
  //th.detach();
  //tg.detach();
  thread t0(increaseValue, ref(a), 100, 0);
  thread t1(increaseValue, ref(b), 11000, 1);
  t0.detach();
  t1.detach();
  system("pause");
  cout << "a = " << a << " b = " << b << "\n";
  cout << "Global value in main: " << globalValue << "\n";
  system("pause");

  //g++ bare.cpp -std=c++0x -o bare.exe -D _WIN32_WINNT=0x0A00

  return 0;
}