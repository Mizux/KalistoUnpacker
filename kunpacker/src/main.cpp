#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "kunpacker.hpp"

int main(int argc, char** argv) {
  std::cout << "Unpacker for Kallisto Index File (KIX) & Kallisto Binary File "
               "(KBF)\n";
  if (argc == 1) {
    std::cout << "Usage:\n";
    std::cout << "* Read KIX file: " << argv[0] << "[file.kix]\n";
    std::cout << "* Extract KBF file: " << argv[0] << "[file.kix] [file.kbf]\n";
    std::exit(EXIT_SUCCESS);
  }

  // Read first file
  fs::path kix_path = "/dev/null";
  std::ifstream ifs_kix;
  if (argc >= 2) {
    kix_path = argv[1];
    ifs_kix.open(kix_path, std::ios::in | std::ios::binary);
    if (!ifs_kix.is_open()) {
      std::cerr << "ERROR: cannot open KIX file '" << kix_path << "'\n";
      std::exit(EXIT_FAILURE);
    }

    // Display file size
    try {
      auto kix_size = fs::file_size(kix_path);
      std::cout << "File size = " << kix_size << "bytes\n";
    } catch (fs::filesystem_error& e) {
      std::cerr << e.what() << '\n';
      std::exit(EXIT_FAILURE);
    }
  }

  // Read second file
  fs::path kbf_path = "/dev/null";
  std::ifstream ifs_kbf;
  bool extract = false;
  if (argc >= 3) {
    kbf_path = argv[2];
    ifs_kbf.open(kbf_path, std::ios::in | std::ios::binary);
    if (!ifs_kbf.is_open()) {
      std::cerr << "ERROR: cannot open KBF file '" << kbf_path << "'\n";
      std::exit(EXIT_FAILURE);
    }

    // Display file size
    try {
      auto kbf_size = fs::file_size(kbf_path);
      std::cout << "File size = " << kbf_size << "bytes\n";
      if (kbf_size == 0) exit(EXIT_FAILURE);
    } catch (fs::filesystem_error& e) {
      std::cerr << e.what() << '\n';
      std::exit(EXIT_FAILURE);
    }
    extract = true;
  }

  if (ku::isKixFile(kix_path)) {
    std::cout << kix_path << " => KIX File Detected!\n";
    // Parse KIX [KBF] file(s)
    try {
      std::filesystem::path base_path = fs::current_path();
      std::cout << "Current dir = " << base_path << "\n";
      ku::parseKixBlock(base_path, ifs_kix, ifs_kbf, extract);
      std::exit(EXIT_SUCCESS);
    } catch (fs::filesystem_error& e) {
      std::cerr << e.what() << '\n';
      std::exit(EXIT_FAILURE);
    }
  } else if (ku::isHeraFile(kix_path)) {
    std::cout << kix_path << " => Hera File Detected !\n";
    std::exit(EXIT_SUCCESS);
  } else {
    std::cout << kix_path << " => Unknown/Raw File Detected !\n";
    std::exit(EXIT_FAILURE);
  }

  // never reach
  return EXIT_FAILURE;
}
