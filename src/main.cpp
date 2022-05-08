/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     HMMRWAR entry point
// Author:      jay-tux
// Created:     May 7, 2022 11:20 AM
// Copyright:   (c) 2022 jay-tux
// Licence:     MPL
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <utility>

#include "argparser.hpp"
#include "io/genome_reader.hpp"

int main(int argc, const char **argv) {
  using hmmrwar::parser;
  using hmmrwar::argdata;
  using hmmrwar::io::genome_reader;

  // step 1: parse cli arguments
  parser p;
  parser::result pre_args;
  // try {
    pre_args = p(argc, argv);
  // }
  // catch(std::exception &e) {
  //   p.write_help(std::cerr);
  //   std::cerr << std::endl << e.what() << std::endl;
  //   return -1;
  // }

  if(p.is_help(std::move(pre_args))) {
    p.write_help(std::cout);
    return 0;
  }

  argdata args(p.as_data(std::move(pre_args)));

  // step 2: signal decomposition (optionally via og hmmratac)
  // step 3: model training using Baum-Welch (priority1)
  // step 4: genome annotation
}
