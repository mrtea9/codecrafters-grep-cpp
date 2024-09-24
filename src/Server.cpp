#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <cctype>
#include <sstream>
#include <vector>
#include <algorithm>

int matchHere(char* regexp, char* text);

int matchBackreference(char reference, char* regexp, char* text) {

    std::cout << std::endl;
    std::cout << "[matchBackreference Text]: " << text << std::endl;
    std::cout << "[matchBackreference RegExp]: " << regexp << std::endl;
    std::cout << "[matchBackreference Reference]: " << regexp << std::endl;

    return 0;
}

int matchParentheses(char* regexp, char* text) {
    int in_paren = 0;
    char* orig_regexp = regexp;

    do {
        std::cout << std::endl;
        std::cout << "[matchParentheses Text]: " << text << std::endl;
        std::cout << "[matchParentheses RegExp]: " << regexp << std::endl;

        if (regexp[0] == '(') return matchParentheses(regexp + 1, text);

        if (regexp[0] == '\\') return matchBackreference(regexp[1], orig_regexp, text);

    } while (*regexp != '\0' && (*regexp++ != '\\'));

    return 0;
}

int matchStar(int c, char* regexp, char* text) {
    do {
        if (matchHere(regexp, text)) return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));

    return 0;
}

int matchHere(char* regexp, char* text) {

    if (regexp[0] == '\0') return 1;
    if (regexp[0] == '$' && regexp[1] == '\0') return *text == '\0';
    if (regexp[1] == '*') return matchStar(regexp[0], regexp + 2, text);
    if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text)) return matchHere(regexp + 1, text + 1);
    if (regexp[0] == '(') return matchParentheses(regexp + 1, text);

    return 0;
}

static int match(char* regexp, char* text) {
    if (regexp[0] == '^') return matchHere(regexp + 1, text);
    do {
        std::cout << std::endl;
        std::cout << "[Text]: " << text << std::endl;
        std::cout << "[RegExp]: " << regexp << std::endl;

        if (matchHere(regexp, text)) return 1;
    } while (*text++ != '\0');
    return 0;
}


static bool match_pattern(const std::string& input_line, const std::string& pattern) {
    std::cout << "pattern : " << pattern << std::endl;


    int length = pattern.length();
    char* regexp = new char[length + 1];

    strcpy(regexp, pattern.c_str());

    length = input_line.length();
    char* text = new char[length + 1];

    strcpy(text, input_line.c_str());

    if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
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
    else if (match(regexp, text) == 1) {
        return true;
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
