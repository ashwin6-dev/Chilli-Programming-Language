#include "tokenizer.h"
#include <regex>
#include <string>

using namespace std;

Token::Token(string v, string t, int n) {
    value = v;
    type = t;
    lineno = n;
}


Tokenizer::Tokenizer(string s) {
    src = s;
    idx = 0;
    lineno = 1;
}

Token Tokenizer::NextToken() {
    Token token = Peek();
    idx += token.value.size();

    return token;
}

Token Tokenizer::Peek() {
    for (vector<string> s : scheme) {
        smatch sm;
        regex r(s[0]);
        string sub_src = src.substr(idx);

        if (regex_search(sub_src, sm, r)) {
            string v = sm[0];
            string t = s[1];

            if (t == "NEW_LINE") {
                idx += v.size();
                lineno += 1;
                return Peek();
            }else if (t == "") { 
                idx += v.size();
                return Peek();
            }

            Token token(v, t, lineno);
            
            return token;
        }
    }

    if (idx < src.size()) {
        cout << "line " << lineno << " error: invalid token " << src[idx];
        exit(-1);
    }

    return Token("", "NONE", -1);
}