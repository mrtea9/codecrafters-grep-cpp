#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <cctype>


bool match_pattern(const std::string& input_line, const std::string& pattern) {
    std::cout << "pattern : " << pattern << std::endl;
    if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
    }
    else if (pattern.compare("\\d") == 0) {
        return input_line.find_first_of("1234567890") != std::string::npos;
    }
    else if (pattern.compare("\\w") == 0) {
        return input_line.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890_") != std::string::npos;
    }
    else if (pattern[0] == '[' && pattern.back() == ']') {
        std::string matching_characters;
        matching_characters = pattern.substr(1, pattern.size() - 2);
        std::cout << matching_characters << std::endl;
        result = input_line.find_first_of(matching_characters) != std::string::npos;
        return matching_characters[0] == '^' ? !result : result;
    }
    else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here" << std::endl;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string pattern = argv[2];

    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    // Uncomment this block to pass the first stage
    
     std::string input_line;
     std::getline(std::cin, input_line);


     try {
         if (match_pattern(input_line, pattern)) {
             return 0;
         } else {
             return 1;
         }
     } catch (const std::runtime_error& e) {
         std::cerr << e.what() << std::endl;
         return 1;
     }
}
