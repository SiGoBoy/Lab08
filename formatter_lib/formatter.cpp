#include "formatter.h"
#include <sstream>

namespace formatter {
    std::string format(const std::string& message) {
        std::ostringstream oss;
        oss << "Formatted: " << message;
        return oss.str();
    }
}
