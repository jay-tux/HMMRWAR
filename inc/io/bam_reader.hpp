/////////////////////////////////////////////////////////////////////////////
// Name:        bam_reader.hpp
// Purpose:     Wrapper around SeqAn's BAM file reader
// Author:      jay-tux
// Created:     May 8, 2022 04:44 PM
// Copyright:   (c) 2022 jay-tux
// Licence:     MPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _HMMRWAR_IO_BAM_READER
#define _HMMRWAR_IO_BAM_READER

#include <string>

namespace hmmrwar::io {
// struct bam_iterator {};
void read_bam(std::string file);
} // hmmrwar::io

#endif
