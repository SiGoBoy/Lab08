#include "formatter.h"
#include <sstream>

namespace formatter {
    std::string format(const std::string& text) {
        std::ostringstream oss;
        oss << "Formatted: " << text;
        return oss.str();
    }
}
