#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

struct Semaphore {
    queue<int> q;
    int value;
    mutex mtx;
    condition_variable cv;
};

void P(Semaphore &s) {
    unique_lock<mutex> lock(s.mtx);
    if (s.value == 1) {
        s.value = 0;
        return;
    }
    else {
        s.q.push(1);
        s.cv.wait(lock);
    }
}

void V(Semaphore &s) {
    unique_lock<mutex> lock(s.mtx);
    if (s.q.empty()) {
        s.value = 1;
    }
    else {
        s.q.front();
        s.q.pop();
        s.cv.notify_one();
    }
}

int main() {
    Semaphore s;
    s.value = 1;

    thread t1([&s]() {
        P(s);
        cout << "Thread 1: Kritik bolgeye erisim sagladi." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Thread 1: Kritik bolgeden cikti." << endl;
        V(s); // Kritik bölgeden çýktýktan sonra semaforu serbest býrak
    });

    this_thread::sleep_for(chrono::seconds(5)); // Bir süre bekle

    thread t2([&s]() {
        P(s); // Semaforu al
        cout << "Thread 2: Kritik bolgeye erisim sagladi." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Thread 2: Kritik bolgeden cikti." << endl;
        V(s); // Kritik bölgeden çýktýktan sonra semaforu serbest býrak
    });

    t1.join();
    t2.join();

    return 0;
}
