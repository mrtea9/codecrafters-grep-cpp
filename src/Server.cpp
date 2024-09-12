#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <cctype>


int matchstar(int c, char* regexp, char* text) {
    do {
        if (matchhere(regexp, text)) return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}

int mactchhere(char* regexp, char* text) {
    if (regexp[0] == '\0') return 1;
    if (regexp[1] == '*') return matchstar(regexp[0], regexp + 2, text);
    if (regexp[0] == '$' && regexp[1] == '\0') return *text == '\0';
    if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text)) return matchhere(regexp + 1, text + 1);
    return 0;
}

int match(char* regexp,char* text) {
    if (regexp[0] == '^') return matchhere(regexp + 1, text);
    do {
        if (matchhere(regexp, text)) return 1;
    } while (*text++ != '\0');
    return 0;
    std::cout << regexp << std::endl;
    std::cout << text << std::endl;
    mactchhere(regexp + 1, text);
    return 1;
}


bool match_pattern(const std::string& input_line, const std::string& pattern) {
    std::cout << "pattern : " << pattern << std::endl;

    const int length = pattern.length();
    char* char_array = new char[length + 1];

    strcpy(char_array, pattern.c_str());

    const int length2 = input_line.length();
    char* char_array2 = new char[length2 + 1];

    strcpy(char_array2, input_line.c_str());

    match(char_array, char_array2);
    if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
    }
    else if (pattern.compare("\\d") == 0) {
        std::cout << pattern << std::endl;
        return input_line.find_first_of("1234567890") != std::string::npos;
    }
    else if (pattern.compare("\\w") == 0) {
        return input_line.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890_") != std::string::npos;
    }
    else if (pattern[0] == '[' && pattern.back() == ']') {
        std::string chars_to_match;
        bool negate;
        bool isMatch;

        chars_to_match = pattern.substr(1, pattern.size() - 2);
        std::cout << chars_to_match << std::endl;
        negate = chars_to_match[0] == '^';

        isMatch = input_line.find_first_of(chars_to_match) != std::string::npos;
        return negate ? !isMatch : isMatch;
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
    char* pattern = argv[2];

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
