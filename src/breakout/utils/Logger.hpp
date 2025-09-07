#ifndef BREAKOUT_UTILS_LOGGER_HPP
#define BREAKOUT_UTILS_LOGGER_HPP

#include <fstream>
#include <mutex>

namespace breakout::utils {

class Logger {
public:
  explicit Logger(const std::string &filename);
  Logger() = delete;
  ~Logger() = default;

  auto log(std::string const &msg) -> void;

  auto operator<<(std::string const &msg) -> Logger*;

private:
  std::ofstream m_logfile;
  mutable std::mutex m_mtx;
};

} // namespace breakout::utils

#endif // BREAKOUT_UTILS_LOGGER_HPP
