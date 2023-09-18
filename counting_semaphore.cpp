#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

struct CountingSemaphore {
    int value;
    mutex mtx;
    condition_variable cv;
};

void P(CountingSemaphore& s) {
    unique_lock<mutex> lock(s.mtx);
    while (s.value == 0) {
        s.cv.wait(lock);
    }
    s.value--;
}

void V(CountingSemaphore& s) {
    unique_lock<mutex> lock(s.mtx);
    s.value++;
    s.cv.notify_one();
}

int main() {
    CountingSemaphore s;
    s.value = 2;

    thread t1([&s]() {
        P(s);
        cout << "Thread 1: Kritik bolgeye erisim sagladi." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Thread 1: Kritik bolgeden cikti." << endl;
        V(s); // Kritik bölgeden çıktıktan sonra semaforu serbest bırak
    });

    thread t2([&s]() {
        P(s);
        cout << "Thread 2: Kritik bolgeye erisim sagladi." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Thread 2: Kritik bolgeden cikti." << endl;
        V(s); // Kritik bölgeden çıktıktan sonra semaforu serbest bırak
    });

    thread t3([&s]() {
        P(s);
        cout << "Thread 3: Kritik bolgeye erisim sagladi." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Thread 3: Kritik bolgeden cikti." << endl;
        V(s); // Kritik bölgeden çıktıktan sonra semaforu serbest bırak
    });

    thread t4([&s]() {
        P(s);
        cout << "Thread 4: Kritik bolgeye erisim sagladi." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Thread 4: Kritik bolgeden cikti." << endl;
        V(s); // Kritik bölgeden çıktıktan sonra semaforu serbest bırak
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
