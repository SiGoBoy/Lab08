#include <iostream>
#include "formatter_ex.h"

int main() {
    std::string text;
    std::cout << "Enter text: ";
    std::getline(std::cin, text);

    std::string formatted = formatter_ex::format_ex(text);
    std::cout << "Formatted text: " << formatted << std::endl;

    return 0;
}
