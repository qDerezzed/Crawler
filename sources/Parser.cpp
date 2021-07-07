// Copyright 2020 qDerezzed
#include "Parser.h"

Parser::Parser(std::string html)
    : html_(std::move(html)), root(gumbo_parse(html_.c_str())->root) {}

std::set<std::string> Parser::getLinks(
    const std::string &attr, const std::set<decltype(GUMBO_TAG_A)> &tags) {

  std::set<std::string> set;
  getLinks(root, attr, tags, set);
  return set;
}

void Parser::getLinks(const GumboNode *node, const std::string &attr,
                      const std::set<decltype(GUMBO_TAG_A)> &tags,
                      std::set<std::string> &container) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }

  GumboAttribute *href;
  if (href = gumbo_get_attribute(&node->v.element.attributes, attr.c_str());
      boost::is_any_of(tags)(node->v.element.tag) && href) {
    std::string link = href->value;
    if (!link.empty()) {
      container.emplace(std::move(link));
    }
  }

  const GumboVector *children = &node->v.element.children;
  for (size_t i = 0; i < children->length; ++i) {
    getLinks(static_cast<GumboNode *>(children->data[i]), attr, tags,
             container);
  }
}
