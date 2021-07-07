// Copyright 2020 qDerezzed
#pragma once

#include <boost/asio/thread_pool.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <utility>

#include "Downloader.h"
#include "Parser.h"

struct InputArguments {
  static std::string url;
  static size_t depth;
  static size_t networkThreads;
  static size_t parserThreads;
  static std::string output;
};

struct ThreadData {
  Downloader &downloader;
  boost::asio::thread_pool &downloaders;
  boost::asio::thread_pool &parsers;
  std::vector<std::string> &images;
  std::mutex &globalMutex;
  std::atomic<size_t> &parsersAmount;
  std::ofstream &outputFile;

  std::mutex containerMutex{};
};

struct Page {
  std::string target;
  size_t depth;
  std::string html{};
};

int parsArgs(int argc, char **argv);

std::pair<std::string, std::string> getHostAndTarget(const std::string &str);

void parse(const Page &page, ThreadData &data);

void download(const Page &page, ThreadData &data);
