/////////////////////////////////////////////////////////////////////////////
// Name:        argparser.cpp
// Purpose:     Argument parser and related structs for HMMRWAR (impl)
// Author:      jay-tux
// Created:     May 7, 2022 11:20 AM
// Copyright:   (c) 2022 jay-tux
// Licence:     MPL
/////////////////////////////////////////////////////////////////////////////

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include "argparser.hpp"

// typedefs to be used in arguments meta file

#define STRING std::string
#define LIST(T) std::vector<T>

// useful macros to declare arguments

#define FLAG(name, desc) (name, #desc)
#define ARGUMENT(name, desc, type) (name, #desc, cxxopts::value<type>())
#define DEFAULTED(name, desc, type, default)                                   \
  (name, #desc, cxxopts::value<type>()->default_value(default))
#define EXPECT_FILES(...) arg_files = {__VA_ARGS__};
#define ONE_OF(param, ...) limited_values[param] = {__VA_ARGS__};
#define MANDATORY(...) mandatory = {__VA_ARGS__};

using hmmrwar::parser;
using hmmrwar::invalid_argument_error;
using hmmrwar::argdata;

parser::parser() : source{cxxopts::Options("HMMWAR", "HMMRATAC in C++")},
  arg_files{strvc()}, mandatory{strvc()}, limited_values{strtostrvc()}
{
  source.add_options()
#include "../meta/arguments.meta" // fetch arguments etc from data file
}

parser::result parser::operator()(int argc, const char **argv) {
  cxxopts::ParseResult result;
  try {
    result = source.parse(argc, argv);

  } catch(std::exception &v) {
    throw invalid_argument_error(
      "Failed to parse arguments (" + std::string(v.what()) + ")"
    );
  }

  if(result.count("help") > 0) return help_only{};

  // verify args
  for(auto &key : mandatory) {
    if(result.count(key) < 1) {
      throw invalid_argument_error("Parameter `" + key + "` is mandatory.");
    }
  }

  for(auto &key : arg_files) {
    if(result.count(key) == 0) continue;

    std::ifstream opened(result[key].as<std::string>());
    if(!opened.is_open() || !opened.good()) {
      throw invalid_argument_error(
        "Parameter `" + key + "` expects a file. Can't open or read `" +
        result[key].as<std::string>() + "`."
      );
    }
    opened.close();
  }

  for(auto &kvp : limited_values) {
    const std::string &key = std::get<0>(kvp);
    const std::vector<std::string> &opts = std::get<1>(kvp);
    const std::string &value = result[key].as<std::string>();

    if(std::find(opts.begin(), opts.end(), value) == opts.end()) {
      std::string optstr = opts.size() == 0 ? "" : opts[0];
      for(size_t i = 1; i < opts.size(); i++) optstr += ", " + opts[i];
      throw invalid_argument_error(
        "Parameter `" + key + "` should be one of [" + optstr + "]."
      );
    }
  }

  using optstring = std::optional<std::string>;
  #define optional_string(key) \
    (result.count(key) > 0) \
    ? std::optional{result[key].as<std::string>()} \
    : std::nullopt;
  #define or_default(key, T, default) \
    (result.count(key) > 0) ? result[key].as<T>() : default

  std::string bamfile = result["bam"].as<std::string>();
  std::string indexfile = result["index"].as<std::string>();
  std::string genome = result["genome"].as<std::string>();

  using dvec = std::vector<double>;

  dvec means = or_default("means", dvec, dvec({50, 200, 400, 600}));
  dvec stddev = or_default("stddev", dvec, dvec({20,20,20,20}));
  bool frag_em = or_default("frag_em", bool, true);
  int min_map_qual = or_default("minmapq", int, 30);
  int upper_fold = or_default("upper", int, 20);
  int lower_fold = or_default("lower", int, 10);
  int zscore = or_default("zscore", int, 100);
  std::string prefix = or_default("output", std::string, "NA");
  optstring blacklist = optional_string("blacklist");
  bool report_peaks = or_default("peaks", bool, true);
  int kmean_count = or_default("kmeans", int, 3);
  optstring train_regions = optional_string("training");
  bool bedgraph = or_default("bedgraph", bool, false);
  int min_peak_len = or_default("minlen", int, 200);
  std::string score_sys = or_default("score", std::string, "max");
  bool bg_score = or_default("bgscore", bool, false);
  int trim_signals = or_default("trim", int, 0);
  int window_vit = or_default("window", int, 25000000);
  optstring model = optional_string("model");
  bool model_only = or_default("modelonly", bool, false);
  int max_train_region = or_default("maxTrain", int, 1000);

  return argdata{
    bamfile, indexfile, genome,
    {means[0], means[1], means[2], means[3]}, {stddev[0], stddev[1], stddev[2], stddev[3]},
    frag_em, report_peaks, bedgraph, bg_score, model_only,
    min_map_qual, upper_fold, lower_fold, zscore, kmean_count, min_peak_len,
    trim_signals, window_vit, max_train_region,
    prefix, score_sys,
    blacklist, train_regions, model
  };
}

void parser::write_help(std::ostream &target) const {
  target << source.help() << std::endl;
}

bool parser::is_help(result &&r) {
  return std::holds_alternative<help_only>(r);
}

argdata parser::as_data(result &&r) {
  return std::move(std::get<argdata>(r));
}
