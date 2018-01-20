#include <chrono>
#include <string>
#include <iostream>

namespace common
{
  class timer
  {
    private:
      void log()
      {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - m_start;
        std::cout << m_name << " took " << elapsed_seconds.count() << "s." << std::endl;
      }

    public:
      timer(const std::string &name)
      {
        m_name = name;
        m_start = std::chrono::system_clock::now();
      }

      timer() : timer("Operation") {}

      void stop()
      {
        if (!m_stopped)
        {
          m_stopped = true;
          log();
        }
      }

      ~timer()
      {
        stop();
      }

    private:
      bool m_stopped = false;
      std::string m_name;
      std::chrono::system_clock::time_point m_start;
  };

}