#ifndef BREAKOUT_UTILS_LOGGER_HPP
#define BREAKOUT_UTILS_LOGGER_HPP

#include <fstream>
#include <mutex>
#include <string>
#include <sstream>
#include <filesystem>

namespace breakout::utils {

class Logger {
public:
  Logger() = delete;

  explicit Logger(const std::string &filename) : m_logfile{filename} {
    if (!m_logfile) {
      throw std::runtime_error{"Failed to open " + filename};
    }
  }

  ~Logger() {
    std::lock_guard l{m_mtx};
    m_logfile.flush();
  }

  class LogEntry {
  public:
    LogEntry(Logger& parent) : m_parent(parent) {}

    template <typename T>
    LogEntry& operator<<(T const& msg) {
      m_stream << msg;
      return *this;
    }

    ~LogEntry() {
      std::lock_guard l{m_parent.m_mtx};
      m_parent.m_logfile << m_stream.str() << '\n';
      m_parent.m_logfile.flush();
    }

  private:
    Logger& m_parent;
    std::ostringstream m_stream;
  };

  auto entry() -> LogEntry { return LogEntry{*this}; }

private:
  std::ofstream m_logfile;
  mutable std::mutex m_mtx;
};

static inline Logger global_logger{"breakout.log"};

#define LOG ::breakout::utils::global_logger.entry() << std::filesystem::path(__FILE__).filename().string() << ":" << __func__ << ":" << __LINE__ << ": "

} // namespace breakout::utils

#endif // BREAKOUT_UTILS_LOGGER_HPP

