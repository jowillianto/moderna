import moderna.thread_plus;
import moderna.test_lib;
import moderna.file_lock;
import moderna.logging;
#include <atomic>
#include <chrono>
#include <format>
#include <mutex>
#include <thread>

int main() {
  auto pool = moderna::thread_plus::pool::Pool{10};
  auto start_sig = moderna::thread_plus::channel::Channel<void>{};
  constexpr auto random_count = 1000000;
  auto logger = moderna::logging::Logger<moderna::logging::SyncLogger<moderna::logging::DefaultPipe<
    moderna::logging::StreamEmitter,
    moderna::logging::ColorfulFormatter<moderna::logging::DefaultFormatter>>>>{
    moderna::logging::DefaultPipe{
      moderna::logging::StreamEmitter{},
      moderna::logging::ColorfulFormatter{moderna::logging::DefaultFormatter{}}
    }
  };
  auto progress = std::atomic<size_t>{0};
  std::mutex mut;
  for (size_t i = 0; i < random_count; i += 1) {
    auto _ = pool.add_task([&]() {
      while (!start_sig.recv().has_value()) {
        /* spin */
      };
      auto prog = progress.fetch_add(1, std::memory_order::relaxed);
      std::this_thread::sleep_for(
        std::chrono::microseconds{moderna::test_lib::random_integer(500, 1000)}
      );
      std::unique_lock l{mut};
      logger.warn(std::format("{} / {}", prog, random_count));
    });
  }
  start_sig.send(random_count);
  pool.join();
  return 0;
}