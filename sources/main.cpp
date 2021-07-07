// Copyright 2020 qDerezzed
#include "Utils.h"

int main(int argc, char *argv[]) {
  auto returnFlag = parsArgs(argc, argv);
  if (returnFlag) {
    return returnFlag;
  }

  auto hostAndTarget = getHostAndTarget(InputArguments::url);
  boost::asio::thread_pool downloaders{InputArguments::networkThreads};
  boost::asio::thread_pool parsers{InputArguments::parserThreads};
  std::vector<std::string> images;
  std::atomic<size_t> parsersAmount{1};
  std::mutex globalMutex;
  Downloader downloader{hostAndTarget.first, "443"};
  std::ofstream outputFile{InputArguments::output};
  ThreadData threadData{downloader,  downloaders,   parsers,   images,
                        globalMutex, parsersAmount, outputFile};
  globalMutex.lock();
  boost::asio::post(
      downloaders,
      boost::bind(download, Page{hostAndTarget.second, InputArguments::depth},
                  std::ref(threadData)));
  globalMutex.lock();
  return 0;
}
