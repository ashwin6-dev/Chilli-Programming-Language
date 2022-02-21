#include "tokenizer.h"
#include "node.h"

using namespace std;

struct NodeWrapper {
    bool parseSuccess;
    Node* node;
};

class Parser {
    public:
        Tokenizer tokenizer;
        Parser(string src);
        NodeWrapper Parse();
        NodeWrapper Block();
        NodeWrapper Line();
        NodeWrapper Expr();
        NodeWrapper ChainedBool();
        NodeWrapper BoolExpr();
        NodeWrapper AdditiveExpr();
        NodeWrapper MultiplicativeExpr();
        NodeWrapper PowExpr();
        NodeWrapper DotExpr();
        NodeWrapper Not();
        NodeWrapper IndexingExpr();
        NodeWrapper PrimaryExpr();
        NodeWrapper VarAssign();
        NodeWrapper PropertyAssign();
        NodeWrapper IfStmt();
        NodeWrapper WhileLoop();
        NodeWrapper ForLoop();
        NodeWrapper ParseFuncDec();
        NodeWrapper ParseClassMethod();
        NodeWrapper ReturnStmt();
        vector<Node*> Args();
        vector<string> Params();
        Token EatType(string t);
        Token EatValue(string v);
};