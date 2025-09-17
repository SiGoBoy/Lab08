#include "formatter_ex.h"
#include "formatter.h"
#include <sstream>

namespace formatter_ex {
    std::string format_ex(const std::string& text) {
        return formatter::format(text) + " (Extended)";
    }
}
