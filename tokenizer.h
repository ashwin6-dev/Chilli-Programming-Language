#include <iostream>
#include <vector>

using namespace std;

class Token {
    public:
        string value;
        string type;
        int lineno;

        Token(string v, string t, int n);
};

class Tokenizer {
    public:
        string src;
        vector<vector<string>> scheme{
            {"^(\\r\\n|\\r|\\n)", "NEW_LINE"},
            {"^\\s+", ""},
            {"^and", "BOOL_CHAIN"},
            {"^or", "BOOL_CHAIN"},
            {"^not", "NOT"},
            {"^[a-zA-Z_][a-zA-Z0-9_]*",  "IDENT"},
            {"^\\d+(\\.\\d+)*",  "FLOAT"},
            {"^\\.", "DOT"},
            {"^\\+=", "PEQ"},
            {"^\\-=", "SEQ"},
            {"^\\*=", "MEQ"},
            {"^\\/=", "DEQ"},
            {"^%", "MOD"},
            {"^\\+", "PLUS"},
            {"^\\-", "MINUS"},
            {"^\\*", "MULT"},
            {"^/", "DIVIDE"},
            {"^\\^", "POW"},
            {"^\\(", "LPAREN"},
            {"^\\)", "RPAREN"},
            {"^,", "COMMA"},
            {"^::", "COLCOL"},
            {"^:", "COL"},
            {"^!=", "NEQ"},
            {"^>=", "GEQ"},
            {"^<=", "LEQ"},
            {"^>", "G"},
            {"^<", "L"},
            {"^==","EQEQ"},
            {"^=", "EQ"},
            {"^\\{", "LCURLY"},
            {"^\\}", "RCURLY"},
            {"^\\[", "LSQUARE"},
            {"^\\]", "RSQUARE"},
            {"^\".*?\"", "STRING"}
        };
        int idx;
        int lineno;

        Tokenizer(string s);
        Token NextToken();
        Token Peek();
};