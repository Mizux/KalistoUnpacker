#include "kunpacker.hpp"

#include <fstream>
#include <iostream>

#define DRYRUN false

namespace ku {
bool isKixFile(const fs::path& path) {
  std::ifstream file;
  file.open(path, std::ios::binary);
  if (!file.is_open())
    std::exit(EXIT_FAILURE);

  kixHdr_t hdr;
  readKixHdr(file, &hdr, /*advance_cursor=*/true);

  kixNode_t node;
  readKixNode(file, &node, /*advance_cursor=*/true);
  if (node.type != kixNodeType::DIRECTORY && node.type != kixNodeType::FILE)
    return false;

  return true;
}

bool isHeraFile(const fs::path& path) {
  std::ifstream file;
  file.open(path, std::ios::binary);
  if (!file.is_open())
    std::exit(EXIT_FAILURE);

  kixHdr_t hdr;
  readKixHdr(file, &hdr, /*advance_cursor=*/true);

  // for (int i = 0;; i++) {
  //  if (i + 10 >= 32) return false;
  //  if (!std::strncmp((char*)(data + i), " \r\n%%%% \r\n", 10)) return true;
  //}
  return false;
}

void parseKixBlock(const fs::path& basedir, std::ifstream& kix, std::ifstream& kbf, bool extract) {
  if (!extract) {
    std::cout << "--- KIX BLOCK 0x" << std::hex << kix.tellg() << std::dec << " ---\n";
    std::cout << "Path: " << basedir << std::endl;
  }
  kixHdr_t hdr;
  readKixHdr(kix, &hdr, /*advance_cursor=*/true);
  fs::path current_dir = basedir / fs::path(std::string(hdr.name));
  if (!extract) {
    std::cout << "-------- HEADER --------" << std::endl;
    std::cout << hdr << std::endl;
    std::cout << "------------------------" << std::endl;
  } else {
    std::cout << "Create dir: " << current_dir << "\n";
    if (!DRYRUN) {
      fs::create_directories(current_dir);
      fs::permissions(current_dir, fs::perms::all, fs::perm_options::replace);
    }
  }
  for (std::uint32_t i = 0; i < hdr.numRecords; i++) {
    kixNode_t node;
    readKixNode(kix, &node, /*advance_cursor=*/true);
    if (!extract) {
      std::cout << "#" << i << ": " << node;
    }

    if (node.type == kixNodeType::DIRECTORY) {
      // Save the input position indicator
      auto pos = kix.tellg();
      kix.seekg(node.offset, std::ios::beg);
      parseKixBlock(current_dir, kix, kbf, extract);
      // Reset the input position indicator
      kix.seekg(pos);
    } else if (node.type == kixNodeType::FILE) {
      if (extract) {
        extractKixNode(current_dir, node, kix, kbf);
      } else {
        std::uint8_t name_length;
        kix.read(reinterpret_cast<char*>(&name_length), sizeof(std::uint8_t));
        std::vector<char> name(name_length + 1, '\0');
        kix.read(name.data(), name_length);
        std::cout << ", name: `" << std::string(name.data(), name.size()) << "`" << std::endl;
      }
    } else {
      std::cerr << "Unknow type node !\n";
      std::exit(EXIT_FAILURE);
    }
  }
  if (!extract) {
    std::cout << "---- " << basedir << " ----" << std::endl;
  }
}

void readKixHdr(std::ifstream& kix, kixHdr_t* hdr, bool advance_cursor) {
  // Save the input position indicator
  auto pos = kix.tellg();

  kix.read(reinterpret_cast<char*>(hdr), sizeof(kixHdr_t));

  // Reset the input position indicator
  if (!advance_cursor) {
    kix.seekg(pos);
  }
}

std::ostream& operator<<(std::ostream& os, kixNodeType type) {
  switch (type) {
    case kixNodeType::DIRECTORY:
      return os << "Directory";
    case kixNodeType::FILE:
      return os << "File";
  };
  // Should be never reached.
  return os << "Type unknow !";
}

void readKixNode(std::ifstream& kix, kixNode_t* node, bool advance_cursor) {
  // Save the input position indicator
  auto pos = kix.tellg();

  kix.read(reinterpret_cast<char*>(node), sizeof(kixNode_t));

  // Reset the input position indicator
  if (!advance_cursor) {
    kix.seekg(pos);
  }
}

void extractKixNode(const fs::path& basedir, const kixNode_t& node, std::ifstream& kix, std::ifstream& kbf) {
  std::uint8_t name_length;
  kix.read(reinterpret_cast<char*>(&name_length), sizeof(std::uint8_t));
  std::vector<char> name(name_length + 1, '\0');
  kix.read(name.data(), name_length);

  // Get the file entry referenced by this index entry
  kbf.seekg(node.offset);
  if (kbf.fail() || kbf.eof()) {
    std::cerr << std::hex << "ERROR: cannot reach the offset (0x" << node.offset << ") !\n", std::exit(-2);
  }
  kbfNode_t kbf_node;
  readKbfNode(kbf, &kbf_node, /*advance_cursor=*/true);

  std::vector<std::uint8_t> data(node.size);
  kbf.read(reinterpret_cast<char*>(data.data()), node.size);

  std::string kbf_name(kbf_node.name);
  // std::cout << "[DBG] filename: '" << kbf_name << "'\n";

  fs::path filename = basedir / fs::path(kbf_name);
  std::cout << "Extracting: " << filename << "...";
  if (!DRYRUN) {
    std::ofstream ofs_out;
    ofs_out.open(filename, std::ios::binary);
    if (!ofs_out.is_open()) {
      std::cerr << "ERROR: cannot open filename '" << filename << '\n';
      std::exit(EXIT_FAILURE);
    }
    ofs_out.write(reinterpret_cast<char*>(data.data()),
                  node.size); // binary output
    ofs_out.close();
  }
  std::cout << "DONE\n";
}

void readKbfNode(std::ifstream& kbf, kbfNode_t* node, bool advance_cursor) {
  // Save the input position indicator
  auto pos = kbf.tellg();

  kbf.read(reinterpret_cast<char*>(node), sizeof(kbfNode_t));

  // Reset the input position indicator
  if (!advance_cursor) {
    kbf.seekg(pos);
  }
}
} // namespace ku
