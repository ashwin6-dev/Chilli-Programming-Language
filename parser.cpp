#include "parser.h"
#include <vector>
#include <algorithm>

using namespace std;

Parser::Parser(string src) : tokenizer(src) {}

NodeWrapper WrapNode(bool parseSuccess, Node* node) {
    NodeWrapper wrapper;
    wrapper.parseSuccess = parseSuccess;
    wrapper.node = node;

    return wrapper;
}

Token Parser::EatType(string t) {
    Token next = tokenizer.Peek();

    if (next.type == t) {
        tokenizer.idx += next.value.size();
        return next;
    }

    cout << "line " << next.lineno << " error: unexpected token " << next.value << ".  Expected type " << t;
    exit(-1);
}

Token Parser::EatValue(string v) {
    Token next = tokenizer.Peek();

    if (next.value == v) {
        tokenizer.idx += next.value.size();

        return next;
    }

    cout << "line " << next.lineno << " error: unexpected token " << next.value << ". Expected value " << v;
    exit(-1);
}

NodeWrapper Parser::Parse() {
    return Block();
}

NodeWrapper Parser::Block() {
    vector<Node*> lines;
    vector<string> blockEnd {"end", "else", ""};

    Node* line = Line().node;

    while (tokenizer.idx < tokenizer.src.size() && !count(blockEnd.begin(), blockEnd.end(), tokenizer.Peek().value)) {
        lines.push_back(line);
        line = Line().node;
    }

    lines.push_back(line);

    return WrapNode(true, new BlockNode(lines));
}


NodeWrapper Parser::Line() {
    NodeWrapper line = VarAssign();
    if (line.parseSuccess) return line;

    line = IfStmt();
    if (line.parseSuccess) return line;

    line = WhileLoop();
    if (line.parseSuccess) return line;

    line = ForLoop();
    if (line.parseSuccess) return line;

    line = ParseFuncDec();
    if (line.parseSuccess) return line;

    line = ReturnStmt();
    if (line.parseSuccess) return line;

    line = Expr();
    if (line.parseSuccess) return line;

    cout << "line " << tokenizer.Peek().lineno << " error: unexpected token " << tokenizer.Peek().value;
    exit(-1);

    return line;
}

NodeWrapper Parser::Expr() {
    return ChainedBool();
}

vector<string> Parser::Params() {
    vector<string> params {};

    Token next = tokenizer.Peek();

    if (next.type != "IDENT") return params;

    string param = EatType("IDENT").value;

    params.push_back(param);

    next = tokenizer.Peek();

    while (next.type == "COMMA") {
        EatType("COMMA");
        param = EatType("IDENT").value;
        params.push_back(param);
        next = tokenizer.Peek();
    } 

    return params;
}

vector<Node*> Parser::Args() {
    vector<Node*> args{};

    NodeWrapper expr = Expr();

    if (!expr.parseSuccess) return args;
    
    args.push_back(expr.node);

    Token next = tokenizer.Peek();

    while (next.type == "COMMA") {
        EatType("COMMA");
        expr = Expr();
        args.push_back(expr.node);
        next = tokenizer.Peek();
    } 

    return args;
}



NodeWrapper Parser::ChainedBool() {
    NodeWrapper left = BoolExpr();

    if (!left.parseSuccess) return left;

    Node* LeftNode = left.node;
    Token next = tokenizer.Peek();

    while (next.type == "BOOL_CHAIN") {
        EatValue(next.value);
        NodeWrapper right = BoolExpr();
        LeftNode = new OpNode(next.value, LeftNode, right.node);

        next = tokenizer.Peek();
    }

    return WrapNode(true, LeftNode);
}


NodeWrapper Parser::BoolExpr() {
    NodeWrapper left = AdditiveExpr();
    
    if (!left.parseSuccess) return left;

    Node* LeftNode = left.node;
    Token next = tokenizer.Peek();

    while (next.type == "EQEQ" || next.type == "NEQ" || next.type == "GEQ" || next.type == "LEQ" || next.type == "G" || next.type == "L") {
        EatValue(next.value);
        NodeWrapper right = AdditiveExpr();
        LeftNode = new OpNode(next.value, LeftNode, right.node);

        next = tokenizer.Peek();
    }

    return WrapNode(true, LeftNode);
}


NodeWrapper Parser::AdditiveExpr() {
    NodeWrapper left = MultiplicativeExpr();

    if (!left.parseSuccess) return left;

    Node* LeftNode = left.node;
    Token next = tokenizer.Peek();

    while (next.type == "PLUS" || next.type == "MINUS") {
        EatValue(next.value);
        NodeWrapper right = MultiplicativeExpr();

        LeftNode = new OpNode(next.value, LeftNode, right.node);

        next = tokenizer.Peek();
    }
    return WrapNode(true, LeftNode);
}

NodeWrapper Parser::MultiplicativeExpr() {
    NodeWrapper left = PowExpr();

    if (!left.parseSuccess) return left;

    Node* LeftNode = left.node;
    Token next = tokenizer.Peek();

    while (next.type == "MULT" || next.type == "DIVIDE" || next.type == "MOD") {
        EatValue(next.value);
        NodeWrapper right = PowExpr();
        LeftNode = new OpNode(next.value, LeftNode, right.node);

        next = tokenizer.Peek();
    }

    return WrapNode(true, LeftNode);
}

NodeWrapper Parser::PowExpr() {
    NodeWrapper left = Not();

    if (!left.parseSuccess) return left;

    Node* LeftNode = left.node;
    Token next = tokenizer.Peek();

    while (next.type == "POW") {
        EatValue("^");
        NodeWrapper right = Not();
        LeftNode = new OpNode("^", LeftNode, right.node);

        next = tokenizer.Peek();
    }

    return WrapNode(true, LeftNode);
}

NodeWrapper Parser::Not() {
    Token next = tokenizer.Peek();

    bool apply_not = false;

    while (next.type == "NOT") {
        EatValue("not");
        apply_not = !apply_not;

        next = tokenizer.Peek();
    }

    
    NodeWrapper expr = DotExpr();

    if (!expr.parseSuccess) return expr;

    if (!apply_not) {
        return WrapNode(true, expr.node);
    }


    return WrapNode(true, new NotNode(expr.node));
}

NodeWrapper Parser::DotExpr() {
    NodeWrapper left = IndexingExpr();

    if (!left.parseSuccess) return left;

    Node* LeftNode = left.node;
    Token next = tokenizer.Peek();

    while (next.type == "DOT") {
        EatValue(".");
        NodeWrapper right = PrimaryExpr();
        LeftNode = new OpNode(".", LeftNode, right.node);
        next = tokenizer.Peek();
    }

    return WrapNode(true, LeftNode);
}


NodeWrapper Parser::IndexingExpr() {
    NodeWrapper left = PrimaryExpr();

    if (!left.parseSuccess) return left;

    Token next = tokenizer.Peek();

    while (next.type == "LSQUARE") {
        EatValue("[");
        NodeWrapper indexExpr = Expr();
        EatValue("]");

        left = WrapNode(true, new IndexingNode(left.node, indexExpr.node));
        next = tokenizer.Peek();
    }

    return left;
}


NodeWrapper Parser::PrimaryExpr() {
    Token next = tokenizer.Peek();

    if (next.type == "STRING") {
        return WrapNode(true, new StringNode(tokenizer.NextToken().value));
    }else if (next.type == "FLOAT") {
        return WrapNode(true, new FloatNode(tokenizer.NextToken().value));
    }else if (next.type == "IDENT") {
        string value = tokenizer.NextToken().value;
        next = tokenizer.Peek();
        if (next.type != "LPAREN") {
            return WrapNode(true, new VarNode(value));
        }

        EatValue("(");
        vector<Node*> args = Args();
        EatValue(")");

        return WrapNode(true, new FuncCall(value, args));
    }else if (next.type == "LPAREN") {
        EatValue("(");
        NodeWrapper expr = Expr();
        EatValue(")");
        return expr;
    }else if (next.type == "LCURLY") {
        EatValue("{");
        vector<Node*> list = Args();
        EatValue("}");

        return WrapNode(true, new ListNode(list));
    }else if (next.type == "MINUS") {
        EatValue("-");
        NodeWrapper expr = PrimaryExpr();

        return WrapNode(true, new OpNode("*", new FloatNode("-1"), expr.node));
    }

    return WrapNode(false, new Node());
}


NodeWrapper Parser::VarAssign() {
    Token next = tokenizer.Peek();
    int save = tokenizer.idx;
    int saveLineno = tokenizer.lineno;
    if (next.type == "IDENT") {
        string value = EatType("IDENT").value;

        if (tokenizer.Peek().value == "=") {
            EatValue("=");
            NodeWrapper expr = Expr();

            return WrapNode(true, new VarAssignNode(value, expr.node));
        }
    }

    tokenizer.idx = save;
    tokenizer.lineno = saveLineno;
    return WrapNode(false, new Node());
}


NodeWrapper Parser::IfStmt() {
    Token next = tokenizer.Peek();
    int save = tokenizer.idx;
    int saveLineno = tokenizer.lineno;

    if (next.value == "if") {
        EatValue("if");

        NodeWrapper expr = Expr();
        NodeWrapper block = Block();

        next = tokenizer.Peek();

        if (next.value == "end") {
            EatValue("end");
            return WrapNode(true, new IfNode(expr.node, block.node, new Node()));
        }else if (next.value == "else") {
            EatValue("else");
            next = tokenizer.Peek();
            if (next.value == "if") {
                NodeWrapper elseIf = IfStmt();
                return WrapNode(true, new IfNode(expr.node, block.node, elseIf.node));
            }

            NodeWrapper res = WrapNode(true, new IfNode(expr.node, block.node, Block().node));
            tokenizer.NextToken();
            return res;
        }
    }

    tokenizer.idx = save;
    tokenizer.lineno = saveLineno;
    return WrapNode(false, new Node());
}

NodeWrapper Parser::WhileLoop() {
    Token next = tokenizer.Peek();
    int save = tokenizer.idx;
    int saveLineno = tokenizer.lineno;

    if (next.value == "while") {
        EatValue("while");

        NodeWrapper expr = Expr();
        NodeWrapper block = Block();

        next = tokenizer.Peek();

        if (next.value == "end") {
            EatValue("end");
            return WrapNode(true, new WhileNode(expr.node, block.node));
        }
    }

    tokenizer.idx = save;
    tokenizer.lineno = saveLineno;
    return WrapNode(false, new Node());
}

NodeWrapper Parser::ForLoop() {
    Token next = tokenizer.Peek();
    int save = tokenizer.idx;
    int saveLineno = tokenizer.lineno;

    if (next.value == "for") {
        EatValue("for");

        string iter = EatType("IDENT").value;
        NodeWrapper expr = Expr();
        NodeWrapper block = Block();

        next = tokenizer.Peek();

        if (next.value == "end") {
            EatValue("end");
            return WrapNode(true, new ForNode(iter, expr.node, block.node));
        }
    }

    tokenizer.idx = save;
    tokenizer.lineno = saveLineno;
    return WrapNode(false, new Node());
}


NodeWrapper Parser::ParseFuncDec() {
    Token next = tokenizer.Peek();

    int save = tokenizer.idx;
    int saveLineno = tokenizer.lineno;

    if (next.type == "IDENT") {
        string value = EatType("IDENT").value;

        if (tokenizer.Peek().value == "::") {
            EatValue("::");
            EatValue("(");
            vector<string> params = Params();
            EatValue(")");

            NodeWrapper block = Block();

            next = tokenizer.Peek();

            if (next.value == "end") {
                EatValue("end");
                return WrapNode(true, new FuncDec(value, params, block.node));
            }
        }
    }

    tokenizer.idx = save;
    tokenizer.lineno = saveLineno;
    return WrapNode(false, new Node());
}

NodeWrapper Parser::ReturnStmt() {
    Token next = tokenizer.Peek();

    int save = tokenizer.idx;
    int saveLineno = tokenizer.lineno;

    if (next.value == "return") {
        EatValue("return");

        NodeWrapper expr = Expr();

        return WrapNode(true, new ReturnNode(expr.node));
    }

    tokenizer.idx = save;
    tokenizer.lineno = saveLineno;
    return WrapNode(false, new Node());
}