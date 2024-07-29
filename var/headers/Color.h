#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


enum class Color
{
    WHITE,
    BLACK
};
#if FMT_VERSION >= 90000
template <> struct fmt::formatter<Color> : ostream_formatter {};
#endif


// since cannot add friend method to enum class, need to specify signature here so that compiled at the right time
std::ostream& operator<<(std::ostream& os, const Color& color);
