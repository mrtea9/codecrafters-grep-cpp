#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <cctype>
#include <sstream>
#include <vector>
#include <algorithm>


int matchDigit(char* regexp, char* text);
int matchLetter(char* regexp, char* text);
int matchPlus(char c, char* regexp, char* text);
int matchOptional(char c, char* regexp, char* text);
int matchOr(char* regexp, char* text);
int matchBackreference(char* regexp, char* text);
int matchGroup(char* regexp, char* text);

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); 
    }
    return str;
}

static int matchHere(char* regexp, char* text) {
  
    std::cout << std::endl;
    std::cout << "[Text]: " << text << std::endl;
    std::cout << "[RegExp]: " << regexp << std::endl;

    if (regexp[0] == '\0') return 1;

    if (regexp[0] == ' ') return matchHere(regexp + 1, text);
    if (regexp[0] == '$' && regexp[1] == '\0') return *text == '\0';
    if (regexp[1] == '?') return matchOptional(regexp[0], regexp + 2, text);
    if (regexp[1] == '+') return matchPlus(regexp[0], regexp + 2, text);
    if (regexp[0] == '\\' && regexp[1] == 'd') return matchDigit(regexp + 2, text);
    if (regexp[0] == '\\' && regexp[1] == 'w') return matchLetter(regexp + 2, text);
    if (regexp[0] == '(' && std::string(regexp).find('\\') != std::string::npos) {
        return matchBackreference(regexp, text);
    }
    if (regexp[0] == '(') {
            if ( std::string(regexp).find('|') != std::string::npos) return matchOr(regexp + 1, text);
        }
    if (regexp[0] == '[') return matchGroup(regexp, text);
    if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text)) return matchHere(regexp + 1, text + 1);

    if (*text != '\0') return matchHere(regexp, text + 1);

    return 0;
}

int matchDigit(char* regexp, char* text) {
    do {
        std::cout << "[Digit] Text: " << text << std::endl;
        std::cout << "[Digit] RegExp: " << regexp << std::endl;

        if (*regexp == '+' && isdigit(*text)) {
            *regexp++;

            do {
                *text++;

                std::cout << "[Digit +] Text: " << text << std::endl;
                std::cout << "[Digit +] RegExp: " << regexp << std::endl;

            } while (isdigit(*text));

            return matchHere(regexp, text);
        }

        if (isdigit(*text)) return matchHere(regexp, text + 1);
    } while (*text++ != '\0');
    return 0;
}

int matchLetter(char* regexp, char* text) {
    do {
        std::cout << "[Letter] Text: " << text << std::endl;
        std::cout << "[Letter] RegExp: " << regexp << std::endl;

        if (*regexp == '+' && isalpha(*text)) {
            *regexp++;

            do {
                *text++;

                std::cout << "[Letter +] Text: " << text << std::endl;
                std::cout << "[Letter +] RegExp: " << regexp << std::endl;

            } while (isalpha(*text));

            return matchHere(regexp, text);
        }

        if (isalpha(*text)) return matchHere(regexp, text + 1);
    } while (*text++ != '\0');
    return 0;
}

int matchPlus(char c, char* regexp, char* text) {
    do {
        std::cout << "[Plus] Text: " << text << std::endl;
        std::cout << "[Plus] RegExp: " << regexp << std::endl;

        if (c == *text) return matchHere(regexp, text + 1);
    } while (*text++ != '\0');

    return 0;
}

int matchPlus(std::string chars, char* regexp, char* text) {

    int length = chars.length();
    char* chars_array = new char[length + 1];
    strcpy(chars_array, chars.c_str());

    do {
        std::cout << "[Plus2] Text: " << text << std::endl;
        std::cout << "[Plus2] RegExp: " << regexp << std::endl;
        std::cout << "[Plus2] Chars: " << chars_array << std::endl;

        if (*chars_array == *text) *chars_array++;

        if (*chars_array == '\0') return matchHere(regexp + 1, text + 1);

    } while (*text++ != '\0');

    return 0;
}

int matchOptional(char c, char* regexp, char* text) {

    if (c == *text) *text++;

    std::cout << "[Optional] Text: " << text << std::endl;
    std::cout << "[Optional] RegExp: " << regexp << std::endl;

    if (c == *text) return 0;

    return matchHere(regexp, text);
}

int matchOr(char* regexp, char* text) {
    std::size_t length = strlen(regexp);
    if (length) {
        regexp[length] = '\0';
    }

    std::cout << "[Or] Text: " << text << std::endl;
    std::cout << "[Or] RegExp: " << regexp << std::endl;

    std::string captured = regexp;
    std::string rest_regexp;
    size_t begin_group = captured.find(')');
    size_t end_group = captured.length();

    rest_regexp = captured.substr(begin_group + 1, end_group - 1);
    std::cout << "[Or] Rest: " << rest_regexp << std::endl;

    std::stringstream ss(regexp);
    std::string token;
    std::vector<std::string> tokens;
    char delimeter = '|';

    while (getline(ss, token, delimeter)) {
        tokens.push_back(token);
    }

    for (std::string token : tokens) {
        if (token.find(')') != std::string::npos) token.pop_back();

        token = token + rest_regexp;

        char* copy_text = text;
        int length = token.length();
        char* token_array = new char[length + 1];
        strcpy(token_array, token.c_str());



        std::cout << "[Or Variant] RegExp: " << token_array << std::endl;

        if (matchHere(token_array, copy_text) == 1) {
            return 1;
        }
    }

    return 0;
}

char* returnCaptured(char* regexp, char* text) {
    std::string test = "";
    std::cout << "[returnCaptured] Text: " << text << std::endl;
    std::cout << "[returnCaptured] RegExp: " << regexp << std::endl;
    std::cout << "[test]: " << test << std::endl;
    std::cout << "[test] *text: " << *text << std::endl;
    std::cout << std::endl;


    if (regexp[0] == '\\' && regexp[1] == 'd') {
        if (regexp[0] == '\\' && regexp[1] == 'd') {
            regexp += 2;
        }
        else {
            *regexp++;
        }

        do {
            std::cout << "[returnCaptured Digit] Text: " << text << std::endl;
            std::cout << "[returnCaptured Digit] RegExp: " << regexp << std::endl;
            std::cout << "[returnCaptured Digit test]: " << test << std::endl;
            std::cout << std::endl;

            if (regexp[0] == '\0') break;

            if (*regexp == '+' && isdigit(*text)) {
                *regexp++;

                do {
                    test += *text++;

                    std::cout << "[returnCaptured Digit +] Text: " << text << std::endl;
                    std::cout << "[returnCaptured Digit +] RegExp: " << regexp << std::endl;
                    std::cout << "[returnCaptured Digit + test]: " << test << std::endl;
                    std::cout << std::endl;


                } while (isdigit(*text));

            }

            if (isdigit(*text)) {
                if (regexp[0] == '\\' && regexp[1] == 'd') {
                    regexp += 2;
                }
                else {
                    *regexp++;
                }
                test += *text;
            }
        } while (*text++ != '\0' && isdigit(*text));
    }

    if (regexp[0] == '\\' && regexp[1] == 'w') {
        if (regexp[0] == '\\' && regexp[1] == 'w') {
            regexp += 2;
        }
        else {
            *regexp++;
        }


        do {
            std::cout << "[returnCaptured Letter] Text: " << text << std::endl;
            std::cout << "[returnCaptured Letter] RegExp: " << regexp << std::endl;
            std::cout << "[returnCaptured Letter test]: " << test << std::endl;
            std::cout << "[returnCaptured Letter] *text: " << *text << std::endl;
            std::cout << std::endl;


            if (regexp[0] == '\0') break;

            if (*regexp == '+' && isalpha(*text)) {
                *regexp++;

                do {
                    test += *text++;

                    std::cout << "[returnCaptured Letter +] Text: " << text << std::endl;
                    std::cout << "[returnCaptured Letter +] RegExp: " << regexp << std::endl;
                    std::cout << "[returnCaptured Letter + test]: " << test << std::endl;
                    std::cout << std::endl;


                } while (isalpha(*text));

            }

            if (isalpha(*text)) {
                if (regexp[0] == '\\' && regexp[1] == 'w') {
                    regexp += 2;
                }
                else {
                    *regexp++;
                }
                test += *text;
            }
        } while (*text++ != '\0' && isalpha(*text));
        test += *text;
    }

    std::cout << "[test]: " << test << std::endl;

    int length = test.length();
    char* result = new char[length + 1];
    strcpy(result, test.c_str());

    return result;
}

int capturedGroup(char* regexp, char* text) {
    std::string captured;
    std::string string_regexp = regexp;
    std::string string_text = text;
    std::string reference;
    std::string reference_count;
    int reference_int;
    bool nested = false;
    char* result;
    bool finded = true;
    size_t begin_group;
    size_t end_group;
    size_t begin_group2;
    size_t end_group2;
    size_t length2;
    int count = 1;

    while (finded) {
        std::cout << "[captured Group reference1]: " << reference << std::endl;
        std::cout << "[captured Group reference_count1]: " << reference_count << std::endl;
        std::cout << "[captured Group finded1]: " << finded << std::endl;
        size_t pos = 0;
        pos = string_regexp.find("\\");
        while (pos != std::string::npos) {
            if (string_regexp[pos] == '\\' && string_regexp[pos + 1] != 'w' && string_regexp[pos + 1] != 'd') {
                reference = string_regexp.substr(pos, 2);
                break;
            }
            pos++;
            if (pos == string_regexp.length()) break;
        }
        reference_count = "\\" + std::to_string(count);
        finded = string_regexp.find(reference) != std::string::npos;
        reference_int = std::atoi(reference[1]);
        std::cout << "[captured Group reference_int]: " << reference_int << std::endl;
        std::cout << "[captured Group reference[1]]: " << reference[1] << std::endl;
        std::cout << "[captured Group reference2]: " << reference << std::endl;
        std::cout << "[captured Group reference_count2]: " << reference_count << std::endl;
        std::cout << "[captured Group finded2]: " << finded << std::endl;

        if (finded) {
            if (string_regexp.find('|') != std::string::npos) {
                begin_group = string_regexp.find_first_of('(');
                end_group = string_regexp.find_first_of(')');
                length2 = end_group - begin_group;

                captured = string_regexp.substr(begin_group, length2 + 1);
                std::cout << "[captured]: " << captured << std::endl;
                std::cout << "[string_regexp]: " << string_regexp << std::endl;

                string_regexp = ReplaceAll(string_regexp, reference, captured);

                begin_group = string_regexp.find_first_of('(');
                string_regexp.replace(begin_group, 1, "{");
                end_group = string_regexp.find_first_of(')');
                string_regexp.replace(end_group, 1, "}");
                std::cout << "[string_regexp2]: " << string_regexp << std::endl;
            } else {
                std::string test;
                size_t pos_brace = 0;
                int openBrace = 0;
                int closedBrace = 0;
                size_t start_pos = 0;
                size_t end_pos = 0;
                pos_brace = string_regexp.find("(");
                while (pos_brace != std::string::npos) {
        
                    if (string_regexp[pos_brace] == '(') {
                        openBrace++;
                        start_pos = pos_brace;
                        std::cout << std::endl;

                        std::cout << "[openBrace]: " << openBrace << std::endl;
                        std::cout << "[start_pos]: " << start_pos << std::endl;
                        std::cout << "[end_pos]: " << end_pos << std::endl;
                        std::cout << "[string_regexp brace]: " << string_regexp << std::endl;
                    }


                    /*if (string_regexp[pos_brace] == ')') {
                        closedBrace++;
                        end_pos = pos_brace;

                        std::cout << std::endl;
                        std::cout << "[closedBrace]: " << closedBrace << std::endl;
                        std::cout << "[start_pos]: " << start_pos << std::endl;
                        std::cout << "[end_pos]: " << end_pos << std::endl;
                        std::cout << "[string_regexp brace]: " << string_regexp << std::endl;

                    }*/

                    if (string_regexp[pos_brace] == '(') {
                        start_pos = pos_brace;

                        std::cout << std::endl;
                        std::cout << "[start_pos]: " << start_pos << std::endl;
                        std::cout << "[end_pos]: " << end_pos << std::endl;
                        std::cout << "[string_regexp brace]: " << string_regexp << std::endl;
                    }

                    if (string_regexp[pos_brace] == ')') {
                        closedBrace++;
                        //start_pos = string_regexp.find_first_of('(');
                        end_pos = pos_brace;

                        std::cout << std::endl;
                        std::cout << "[start_pos]: " << start_pos << std::endl;
                        std::cout << "[end_pos]: " << end_pos << std::endl;
                        std::cout << "[string_regexp brace]: " << string_regexp << std::endl;

                        test = string_regexp.substr(start_pos + 1, end_pos - start_pos - 1);
                        string_regexp.replace(start_pos, 1, "");
                        string_regexp.replace(end_pos - 1, 1, "");

                        std::cout << "[CAPTURED1]: " << test << std::endl;
                        std::cout << "[string_regexp brace]: " << string_regexp << std::endl;
                        closedBrace--;
                        openBrace--;
                        break;
                    }

                    if (string_regexp[pos_brace] == '\\' && string_regexp[pos_brace + 1] != 'w' && string_regexp[pos_brace + 1] != 'd') {
                        test = string_regexp.substr(start_pos + 1, end_pos - start_pos - 1);
                        string_regexp.replace(start_pos, 1, "");
                        string_regexp.replace(end_pos - 1, 1, "");

                        std::cout << "[start_pos]: " << start_pos << std::endl;
                        std::cout << "[end_pos]: " << end_pos << std::endl;
                        std::cout << "[CAPTURED2]: " << test << std::endl;
                        std::cout << "[string_regexp brace]: " << string_regexp << std::endl;
                        closedBrace--;
                        openBrace--;

                        break;
                    }

                    pos_brace++;
                    if (pos_brace == string_regexp.length()) break;
                }

                begin_group = string_regexp.find_first_of('(');
                end_group = string_regexp.find_first_of(')');
                length2 = end_group - begin_group;

                if (test != "") {
                    captured = test;
                }
                else {
                    captured = string_regexp.substr(begin_group + 1, length2 - 1);
                }
                std::cout << "[captured]: " << captured << std::endl;

                int len = captured.length();
                char* captured_array = new char[len + 1];
                strcpy(captured_array, captured.c_str());

                int len2 = string_text.length();
                char* text_array = new char[len2 + 1];
                strcpy(text_array, string_text.c_str());

                if (matchHere(captured_array, text_array) == 1) {
                    result = returnCaptured(captured_array, text_array);
                }

                std::cout << "[result]: " << result << std::endl;
                std::cout << "[string_text]: " << string_text << std::endl;
                std::cout << "[string_regexp]: " << string_regexp << std::endl;

                if (string_text.find_first_of(std::string(result)) != std::string::npos) {
                    end_group2 = string_text.find_first_of(std::string(result));
                    std::cout << "[end_group2]: " << end_group2 << std::endl;
                    string_text.replace(end_group2, std::string(result).length(), "");
                    std::cout << "[string_text2]: " << string_text << std::endl;
                }
                else {
                    result = captured_array;
                }

                std::cout << "[result2]: " << result << std::endl;

                //begin_group = string_regexp.find_first_of('(');
                //string_regexp.replace(begin_group, 1, "");
                //end_group = string_regexp.find_first_of(')');
                //string_regexp.replace(end_group, 1, "");

                std::cout << "[string_regexp2]: " << string_regexp << std::endl;
                std::cout << "[reference]: " << reference << std::endl;

                string_regexp = ReplaceAll(string_regexp, reference, result);

                std::cout << "[string_regexp3]: " << string_regexp << std::endl;

                std::cout << "[captured Backreference Group] RegExp: " << string_regexp << std::endl;
            }
        }

        count++;
    }

    string_regexp = ReplaceAll(string_regexp, "{", "(");
    string_regexp = ReplaceAll(string_regexp, "}", ")");

    int length = string_regexp.length();
    char* result_regexp = new char[length + 1];
    strcpy(result_regexp, string_regexp.c_str());

    std::cout << "[Backreference Group] Text: " << text << std::endl;
    std::cout << "[Backreference Group] RegExp: " << result_regexp << std::endl;

    return matchHere(result_regexp, text);
}


int matchBackreference(char* regexp, char* text) {
    char* original_regexp = regexp;

    do {
        std::cout << "[Backreference] Text: " << text << std::endl;
        std::cout << "[Backreference] RegExp: " << regexp << std::endl;

        if (*regexp == '\\') return capturedGroup(original_regexp, text);
    } while (*regexp++ != '\0');

    return 0;
}

int matchGroup(char* regexp, char* text) {
    std::string captured = regexp;
    std::string string_regexp = regexp;
    std::string text_string = text;
    std::string chars_to_match;
    bool negate;
    bool isMatch;

    std::cout << "[Group] Text: " << text << std::endl;
    std::cout << "[Group] RegExp: " << regexp << std::endl;

    size_t begin_group = captured.find_first_of('[');
    size_t end_group = captured.find_first_of(']');
    size_t plus_pos = string_regexp.find_first_of('+');

    chars_to_match = captured.substr(begin_group + 1, end_group - 1);
    int length = chars_to_match.length();

    std::cout << "[Group] Chars to Match: " << chars_to_match << std::endl;
    negate = chars_to_match[0] == '^';

    if (end_group + 1 == plus_pos && !negate) return matchPlus(chars_to_match, regexp + length + 2, text);

    isMatch = text_string.find_first_of(chars_to_match) != std::string::npos;
    isMatch = negate ? !isMatch : isMatch;
    
    std::cout << "[Group] isMatch: " << isMatch << std::endl;

    if (isMatch && negate) return matchHere(regexp + length + 3, text + length);

    if (isMatch) return matchHere(regexp + length + 2, text + length);

    return 0;
}

static int match(char* regexp, char* text) {
    if (regexp[0] == '^' && (regexp[1] == *text || regexp[1] == '(' || regexp[1] == '[')) return matchHere(regexp + 1, text);
    if (matchHere(regexp, text)) return 1;

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
