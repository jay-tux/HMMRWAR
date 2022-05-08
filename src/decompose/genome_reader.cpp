/////////////////////////////////////////////////////////////////////////////
// Name:        genome_reader.cpp
// Purpose:     File reader for genome files (impl)
// Author:      jay-tux
// Created:     May 8, 2022 03:22 PM
// Copyright:   (c) 2022 jay-tux
// Licence:     MPL
/////////////////////////////////////////////////////////////////////////////

#include <string>
#include <sstream>

#include "decompose/genome_reader.hpp"

using hmmrwar::decompose::genome_record;
using hmmrwar::decompose::genome_iterator;
using hmmrwar::decompose::genome_reader;

genome_iterator::genome_iterator(std::ifstream &stream) :
  data{stream}, set{false}, next{} {}

genome_iterator::operator const genome_record &() {
  if(!set) _next();
  set = false;
  return next;
}

void genome_iterator::operator++() {
  _next();
}

genome_iterator::operator bool() const { return data.good() && !data.eof(); }

void genome_iterator::_next() {
  if(!set) {
    std::string read;
    std::getline(data, read);
    std::stringstream strm;
    strm.str(read);
    strm >> next.chromosome >> next.end;
    next.start = 0;
    set = true;
  }
}

genome_reader::genome_reader(std::string infile) : datastream{infile} {
  if(!datastream.good())
    throw genome_error("Can't open `" + infile + "` for reading.");
}

genome_iterator genome_reader::begin() {
  return genome_iterator(datastream);
}

bool operator==(const genome_iterator &it, const genome_reader::genome_end &) {
  return !it;
}
