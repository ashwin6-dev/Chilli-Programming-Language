#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Visitor;


class Node {
    public:
        virtual void print();
        virtual void accept(Visitor* visitor);
        virtual void mutate(Visitor* visitor);
        virtual void point(Visitor* visitor);
};


class BlockNode : public Node {
    public:
        vector<Node*> lines;
        BlockNode(vector<Node*> l);
        void print();
        void accept(Visitor* visitor);
};

class StringNode : public Node {
    public:
        string value;
        StringNode(string v);
        void print();
        void accept(Visitor* visitor);
};

class FloatNode : public Node {
    public:
        double value;
        FloatNode(string v);
        void print();
        void accept(Visitor* visitor);
};

class VarNode : public Node {
    public:
        string value;
        VarNode(string v);
        void print();
        void accept(Visitor* visitor);
        void mutate(Visitor* visitor);
        void point(Visitor* visitor);
};

class NotNode : public Node {
    public:
        Node* expr;
        NotNode(Node* e);
        void accept(Visitor* visitor);
};

class IndexingNode : public Node {
    public:
        Node* object;
        Node* indexExpr;
        IndexingNode(Node* o, Node* e);
        void accept(Visitor* visitor);
};


class FuncCall : public Node {
    public:
        string value;
        vector<Node*> args;
        FuncCall(string v, vector<Node*> a);
        void print();
        void accept(Visitor* visitor);
};


class OpNode : public Node {
    public:
        string op;
        Node* left;
        Node* right;
        OpNode(string o, Node* l, Node* r);
        void print();
        void accept(Visitor* visitor);
        void mutate(Visitor* visitor);
        void point(Visitor* visitor);
};

class VarAssignNode : public Node {
    public:
        string value;
        Node* expr;
        VarAssignNode(string v, Node* e);
        void accept(Visitor* visitor);
};


class PropertyAssignNode : public Node {
    public:
        string property;
        Node* expr;

        PropertyAssignNode(string p, Node* e);
        void accept(Visitor* visitor);
};

class ListNode : public Node {
    public:
        vector<Node*> value;
        ListNode(vector<Node*> list);
        void accept(Visitor* visitor);
};

class IfNode : public Node {
    public:
        Node* expr;
        Node* block;
        Node* elseBlock;
        IfNode(Node* exp, Node* b, Node* eb);
        void accept(Visitor* visitor);
};


class WhileNode : public Node {
    public:
        Node* expr;
        Node* block;
        WhileNode(Node* e, Node* b);
        void accept(Visitor* visitor);
};

class ForNode : public Node {
    public:
        string iter;
        Node* iterable;
        Node* block;
        ForNode(string i, Node* it, Node* b);
        void accept(Visitor* visitor);
};


class FuncDec : public Node {
    public:
        string value;
        vector<string> params;
        Node* block;
        FuncDec(string v, vector<string> p, Node* b);
        void accept(Visitor* visitor);
};


class ReturnNode : public Node {
    public:
        Node* expr;
        ReturnNode(Node* e);
        void accept(Visitor* visitor);
};

class ClassMethod : public Node {
    public:
        string class_name;
        string value;
        vector<string> params;
        Node* block;
        ClassMethod(string v, string c, vector<string> p, Node* b);
        void accept(Visitor* visitor);
};