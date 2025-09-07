#include <breakout/utils/Logger.hpp>
#include <stdexcept>

namespace breakout::utils {

Logger::Logger(const std::string &filename): m_logfile{filename}, m_mtx{} {
  if (!m_logfile) {
    throw std::runtime_error{"Failed to open " + filename};
  }
}

auto Logger::log(std::string const &msg) -> void {
  m_logfile << msg << "\n";
}

auto Logger::operator<<(std::string const &msg) -> Logger* {
  log(msg);
  return this;
}

} // namespace breakout::utils
