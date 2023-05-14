#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class DiningPhilosophers {
private:
    int n;
    std::vector<std::mutex> forks;
    std::vector<std::thread> philosophers;

public:
    DiningPhilosophers(int n) : n(n), forks(n), philosophers(n) {}

    void dine() {
        for (int i = 0; i < n; ++i) {
            philosophers[i] = std::thread([this, i]() {
                while (true) {
                    think(i);
                    eat(i);
                }
            });
        }

        for (auto& philosopher : philosophers) {
            philosopher.join();
        }
    }

private:
    void think(int i) {
        std::cout << "Philosopher " << i << " is thinking.\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void eat(int i) {
        int left = i;
        int right = (i + 1) % n;

        // The std::lock function locks the supplied Lockable objects, avoiding deadlocks
        std::lock(forks[left], forks[right]);

        // The std::lock_guard object acquires the lock in the constructor and releases it in the destructor
        std::lock_guard<std::mutex> lockLeft(forks[left], std::adopt_lock);
        std::lock_guard<std::mutex> lockRight(forks[right], std::adopt_lock);

        std::cout << "Philosopher " << i << " is eating.\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Philosopher " << i << " has finished eating.\n";
    }
};

int main() {
    DiningPhilosophers diningPhilosophers(5);
    diningPhilosophers.dine();

    return 0;
}
