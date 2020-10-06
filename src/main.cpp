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

int main() {

    auto simulation = std::make_shared<Simulation>();

    std::thread uiThread = std::thread(uiThreadFunc, simulation);

    uiThread.join();

    return 0;

}
