#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> msg = {"Hello", "from", "Debian", "on", "Chromebook!"};

    for (const auto& word : msg) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
    return 0;
}
