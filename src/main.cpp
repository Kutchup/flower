#include "engine.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "==================================" << std::endl;
    std::cout << "   Flower - A Peaceful Adventure  " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;
    std::cout << "A game about beauty and peace." << std::endl;
    std::cout << "Remind the world that it is beautiful." << std::endl;
    std::cout << std::endl;
    
    Engine engine;
    
    if (!engine.initialize()) {
        std::cerr << "Failed to initialize engine" << std::endl;
        return 1;
    }
    
    engine.run();
    engine.shutdown();
    
    std::cout << std::endl;
    std::cout << "Thank you for playing!" << std::endl;
    std::cout << "Statistics:" << std::endl;
    std::cout << "  Flowers planted: " << engine.getPlayer().getFlowersPlanted() << std::endl;
    std::cout << "  Flowers watered: " << engine.getPlayer().getFlowersWatered() << std::endl;
    std::cout << "  Photographs taken: " << engine.getPlayer().getPhotographsTaken() << std::endl;
    
    return 0;
}
