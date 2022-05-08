/////////////////////////////////////////////////////////////////////////////
// Name:        genome_reader.hpp
// Purpose:     File reader for genome files
// Author:      jay-tux
// Created:     May 8, 2022 03:22 PM
// Copyright:   (c) 2022 jay-tux
// Licence:     MPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _HMMRWAR_DECOMPOSE_GENOME_READER
#define _HMMRWAR_DECOMPOSE_GENOME_READER

#include <optional>
#include <string>
#include <exception>
#include <vector>
#include <fstream>

namespace hmmrwar::io {
struct genome_error : std::exception {
  explicit genome_error(std::string what) : str{what} {}
  inline const char *what() const noexcept override { return str.c_str(); }
  std::string str;
};

struct genome_record {
  std::string chromosome;
  size_t start, end;
};

struct genome_iterator {
public:
  explicit genome_iterator(std::ifstream &stream);
  operator const genome_record &();
  inline genome_record operator*() { return operator const genome_record &(); }
  operator bool() const;

  void operator++();
  inline void operator++(int) { operator++(); }

private:
  void _next();
  std::ifstream &data;
  bool set;
  genome_record next;
};

struct genome_reader {
public:
  struct genome_end{};

  explicit genome_reader(std::string infile);
  genome_iterator begin();
  inline genome_end end() { return {}; }

private:
  std::ifstream datastream;
};

} // namespace hmmrwar::io

bool operator==(
  const hmmrwar::io::genome_iterator &it,
  const hmmrwar::io::genome_reader::genome_end &);

inline bool operator!=(
  const hmmrwar::io::genome_iterator &it,
  const hmmrwar::io::genome_reader::genome_end &end)
{
    return !(it == end);
}

#endif
