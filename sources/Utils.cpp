// Copyright 2020 qDerezzed
#include "Utils.h"

std::string InputArguments::url;
size_t InputArguments::depth = 0;
size_t InputArguments::networkThreads = 0;
size_t InputArguments::parserThreads = 0;
std::string InputArguments::output;

int parsArgs(int argc, char **argv) {
  namespace po = boost::program_options;

  po::options_description desc("Available options");
  desc.add_options()("url", po::value<std::string>(&InputArguments::url))(
      "depth", po::value<size_t>(&InputArguments::depth))(
      "network_threads", po::value<size_t>(&InputArguments::networkThreads))(
      "parser_threads", po::value<size_t>(&InputArguments::parserThreads))(
      "output", po::value<std::string>(&InputArguments::output));

  po::variables_map variablesMap;
  po::store(po::parse_command_line(argc, argv, desc), variablesMap);
  po::notify(variablesMap);

  return 0;
}

std::pair<std::string, std::string> getHostAndTarget(const std::string &str) {
  std::string host = str;
  std::string target = "/";

  auto iterator = std::find(str.cbegin(), str.cend(), '/');
  if (iterator == str.cend()) {
    return {host, target};
  }

  iterator = std::find(iterator + 1, str.cend(), '/');
  auto iterator_2 = std::find(iterator + 1, str.cend(), '/');

  host = std::string{iterator + 1, iterator_2};
  if (iterator_2 != str.cend()) {
    target = std::string{iterator_2, str.cend()};
  }

  return {host, target};
}

void parse(const Page &page, ThreadData &data) {
  Parser parser{page.html};
  auto images = parser.getLinks("src", {GUMBO_TAG_IMG});
  auto links = parser.getLinks("href", {GUMBO_TAG_A});

  for (const std::string &image : images) {
    std::lock_guard<std::mutex> locker(data.containerMutex);
    data.images.push_back(image);
  }

  std::string result;
  for (const auto &image : images) {
    result += image + "\n";
  }
  data.outputFile << result;

  data.parsersAmount--;
  if (page.depth == 0) {
    if (data.parsersAmount == 0) {
      data.globalMutex.unlock();
    }
    return;
  }

  if (!links.empty()) {
    data.parsersAmount += links.size();
  }
  for (const std::string &link : links) {
    Page curPage{link, page.depth - 1};
    boost::asio::post(data.downloaders,
                      boost::bind(download, curPage, std::ref(data)));
  }
}

void download(const Page &page, ThreadData &data) {
  Downloader downloader{data.downloader.getHost(), data.downloader.getPort()};
  std::string html = downloader.download(page.target);
  Page result{page.target, page.depth, html};
  boost::asio::post(data.parsers, boost::bind(parse, result, std::ref(data)));
}
