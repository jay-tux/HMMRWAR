/////////////////////////////////////////////////////////////////////////////
// Name:        bam_reader.cpp
// Purpose:     Wrapper around SeqAn's BAM file reader (impl)
// Author:      jay-tux
// Created:     May 8, 2022 04:44 PM
// Copyright:   (c) 2022 jay-tux
// Licence:     MPL
/////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include "seqan3/io/sam_file/all.hpp"
#include "io/bam_reader.hpp"

void hmmrwar::io::read_bam(std::string file) {
  seqan3::sam_file_input in{file};

  for(auto &&rec : in) {
    std::cout << rec.id() << "\n";
  }
}
