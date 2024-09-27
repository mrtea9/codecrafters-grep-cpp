#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <cctype>
#include <sstream>
#include <vector>
#include <algorithm>

int matchHere(char* regexp, char* text);

char* toChar(std::string s) {
    int length = s.length();
    char* result = new char[length + 1];
    strcpy(result, s.c_str());

    return result;
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

int matchBackreference(char* regexp, char* orig_regexp, char* text, int count) {
    int len = 0;
    int openBrackets = 1;
    int closedBrackets = 0;
    size_t start_pos = 0;
    std::string string_regexp = regexp;
    std::string string_orig = orig_regexp;
    std::string captured;
    std::string reference_full = "\\" + std::to_string(count);
    std::cout << "[matchBackreference orig_regexp]: " << string_orig << std::endl;
    start_pos = string_orig.find(string_regexp);
    
    do {
        std::cout << std::endl;
        std::cout << "[matchBackreference RegExp]: " << regexp << std::endl;
        std::cout << "[matchBackreference Reference]: " << reference_full << std::endl;
        std::cout << "[matchBackreference orig_regexp]: " << string_orig << std::endl;

        len++;

        if (regexp[0] == '(') openBrackets++;
        if (regexp[0] == ')') closedBrackets++;

        if (openBrackets == closedBrackets) break;
        
    } while (*regexp++ != '\0');

    captured = string_regexp.substr(0, len - 1);

    std::cout << "[matchBackreference string_orig]: " << string_orig << std::endl;
    std::cout << "[matchBackreference string_regexp]: " << string_regexp << std::endl;
    std::cout << "[matchBackreference len]: " << len - 1 << std::endl;
    std::cout << "[matchBackreference captured]: " << captured << std::endl;
    std::cout << "[matchBackreference Reference_full]: " << reference_full << std::endl;

    string_orig = ReplaceAll(string_orig, reference_full, captured);
    string_orig.replace(start_pos - 1, 1, "");
    string_orig.replace(start_pos - 1 + len - 1, 1, "");

    std::cout << "[matchBackreference start_pos]: " << start_pos << std::endl;
    std::cout << "[matchBackreference string_orig]: " << string_orig << std::endl;
    std::cout << "[matchBackreference result]: " << toChar(string_orig) << std::endl;

    return matchHere(toChar(string_orig), text);
}

int matchParentheses(char* regexp, char* orig_regexp, char* text) {
    int count = 0;
    char* parentheses_regexp = regexp;

    do {
        std::cout << std::endl;
        std::cout << "[matchParentheses orig_regexp]: " << orig_regexp << std::endl;
        std::cout << "[matchParentheses RegExp]: " << regexp << std::endl;

       
        if (regexp[0] == '\\') {
            count++;
            return matchBackreference(parentheses_regexp, orig_regexp, text, count);
        }

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
    if (regexp[0] == '(') return matchParentheses(regexp + 1, regexp, text);

    if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text)) return matchHere(regexp + 1, text + 1);

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
