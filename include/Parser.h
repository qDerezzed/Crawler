// Copyright 2020 qDerezzed
#pragma once

#include <gumbo.h>

#include <boost/algorithm/string/classification.hpp>
#include <set>
#include <string>

class Parser {
 public:
  explicit Parser(std::string html);

  std::set<std::string> getLinks(const std::string &attr,
                                 const std::set<decltype(GUMBO_TAG_A)> &tags);

 private:
  static void getLinks(const GumboNode *node, const std::string &attr,
                       const std::set<decltype(GUMBO_TAG_A)> &tags,
                       std::set<std::string> &container);

  std::string html_;
  GumboNode *root;
};
