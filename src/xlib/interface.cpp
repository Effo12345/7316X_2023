#include "interface.hpp"

namespace xlib {
    void Interface::update() {
        std::cout << "Valid inputs: ";
        for(std::string s : inputs) {
            std::cout << s << ", ";
        }
        std::cout << "\n";
        std::cout << "Selection: ";
        std::string input;
        std::cin >> input;

        if(input == "update_gain") {
            std::cout << "Current gain: " << fw.getGain() << "\n";
            std::cout << "Enter new gain: ";
            double gain;
            std::cin >> gain;
            fw.setGain(gain);
            std::cout << "Gain set as " << fw.getGain() << "\n";
        } else if(input == "clear_graph") {
            std::cout << "Clearing graph...";
            grapher.clearGraph();
        } else {
            std::cout << "Invalid input\n";
        }
    }
    Interface interface;
}

