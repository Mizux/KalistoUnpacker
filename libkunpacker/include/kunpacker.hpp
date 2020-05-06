#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace ku {
bool isKixFile(const fs::path& path);

bool isHeraFile(const fs::path& path);

void parseKixBlock(const fs::path& basedir, std::ifstream& kix, std::ifstream& kbf, bool extract = false);

struct [[gnu::packed]] kixHdr_t {
 public:
  char name[32];
  std::uint32_t numRecords;

  //! @brief Fill an output stream with a KIX Header.
  //! @param[out] os The output stream to fill.
  //! @param[in] hdr The KIX header to print.
  //! @return The output stream.
  friend std::ostream& operator<<(std::ostream& os, const kixHdr_t& hdr) {
    os << "{Name: '" << hdr.name << "' "
       << "nRecs: " << std::dec << hdr.numRecords << "}";
    return os;
  }
};
void readKixHdr(std::ifstream& kix, kixHdr_t* hdr, bool advance_cursor = false);

enum class kixNodeType : std::uint8_t { DIRECTORY = 0, FILE = 1 };
std::ostream& operator<<(std::ostream& os, kixNodeType type);

struct [[gnu::packed]] kixNode_t {
 public:
  kixNodeType type;
  std::uint32_t memAddr;
  std::uint32_t offset;
  std::uint32_t size;

  //! @brief Fill an output stream with a KIX Node.
  //! @param[out] os The output stream to fill.
  //! @param[in] hdr The KIX node to print.
  //! @return The output stream.
  friend std::ostream& operator<<(std::ostream& os, const kixNode_t& node) {
    os << "{Type: '" << node.type << "' " << std::hex << ", memAddr: 0x" << node.memAddr << ", offset: 0x"
       << node.offset << std::dec << " (" << node.size << "bytes)}";
    return os;
  }
};
void readKixNode(std::ifstream& kix, kixNode_t* node, bool advance_cursor = false);
void extractKixNode(const fs::path& basedir, const kixNode_t& node, std::ifstream& kix, std::ifstream& kbf);

struct [[gnu::packed]] kbfNode_t {
  char name[32];

  //! @brief Fill an output stream with a KBF Node.
  //! @param[out] os The output stream to fill.
  //! @param[in] hdr The KBF node to print.
  //! @return The output stream.
  friend std::ostream& operator<<(std::ostream& os, const kbfNode_t& node) {
    os << "{Name: '" << node.name << "'}";
    return os;
  }
};
void readKbfNode(std::ifstream& kbf, kbfNode_t* node, bool advance_cursor = false);
} // namespace ku
