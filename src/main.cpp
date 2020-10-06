#include <iostream>
#include <memory>
#include <thread>
#include "Simulation.h"
#include "ui/ui.h"

/**
 *
 * @param simulation
 */
void uiThreadFunc(std::shared_ptr<Simulation> simulation) {
    std::cout << "Starting UI Thread" << std::endl;
    char **p = nullptr;
    UI(0, p, simulation);
}

/**
 *
 * @param simulation
 */
void physicsThreadFunc(std::shared_ptr<Simulation> simulation) {

    // wait two seconds before starting physics simulation
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Starting Physics Thread" << std::endl;

    unsigned int threadMillisecDeltaT = 100; // 10 milliseconds
    double threadSecDeltaT = threadMillisecDeltaT / 1000.0; // in seconds

    simulation->deltaT = threadSecDeltaT;

    while (simulation->t >= 0) {

        std::this_thread::sleep_for(std::chrono::milliseconds(threadMillisecDeltaT));

        //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        simulation->simulateStep();

        // usually takes 4000[ns]
        //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    }

    //std::cout << "Took " << simulation.t << " sec" << std::endl;

}


int main() {

    auto simulation = std::make_shared<Simulation>();

    std::thread uiThread = std::thread(uiThreadFunc, simulation);

    //std::thread physicsThread(physicsThreadFunc, simulation);

    uiThread.join();

    //physicsThread.join();

    return 0;

}
