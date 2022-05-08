/////////////////////////////////////////////////////////////////////////////
// Name:        argparser.hpp
// Purpose:     Argument parser and related structs for HMMRWAR
// Author:      jay-tux
// Created:     May 7, 2022 11:20 AM
// Copyright:   (c) 2022 jay-tux
// Licence:     MPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _HMMRWAR_ARGPARSER_H
#define _HMMRWAR_ARGPARSER_H

#include <variant>
#include <vector>
#include <string>
#include <map>
#include <exception>
#include <iostream>
#include <fstream>

#include "cxxopts.hpp"

namespace hmmrwar {
struct invalid_argument_error : std::exception {
  explicit invalid_argument_error(std::string msg) : msg{msg} {}
  inline const char *what() const noexcept override { return msg.c_str(); }
  const std::string msg;
};

struct help_only {};

struct argdata {
  std::string bamfile, indexfile, genome;
  double means[4], stddevs[4];
  bool frag_em, report_peaks, bedgraph, bg_score, model_only;
  int min_map_qual, upper_fold, lower_fold, zscore, kmean_count, min_peak_len;
  int trim_signals, window_vit, max_train_region;
  std::string prefix, score_sys;
  std::optional<std::string> blacklist, train_regions, model;
};

struct parser {
public:
  using strvc = std::vector<std::string>;
  using strtosz = std::map<std::string, size_t>;
  using strtostrvc = std::map<std::string, std::vector<std::string>>;
  using result = std::variant<help_only, argdata>;

  parser();
  result operator()(int argc, const char **argv);
  void write_help(std::ostream &target) const;

  static bool is_help(result &&r);
  static argdata as_data(result &&r);

private:
  cxxopts::Options source;
  strvc arg_files;
  strvc mandatory;
  strtostrvc limited_values;
};
} // namespace hmmrwar

#endif
