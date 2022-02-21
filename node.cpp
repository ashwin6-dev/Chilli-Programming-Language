#include "node.h"
#include "visitor.h"
#include <string>

using namespace std;


void Node::print() {
    cout << "node" << endl;
}

void Node::accept(Visitor* visitor) {};

void Node::mutate(Visitor* visitor) {};

void Node::point(Visitor* visitor) {};

BlockNode::BlockNode(vector<Node*> l) {
    lines = l;
}

void BlockNode::print() {
    cout << "block:" << endl;
    for (Node* n : lines) {
        cout << "   ";
        n->print();
    }
}

StringNode::StringNode(string v) {
    value = v.substr(1, v.size() - 2);
}

VarNode::VarNode(string v) {
    value = v;
}

FloatNode::FloatNode(string v) {
    value = stod(v);
}

NotNode::NotNode(Node* e) {
    expr = e;
}

IndexingNode::IndexingNode(Node* o, Node* e) {
    object = o;
    indexExpr = e;
}

FuncCall::FuncCall(string v, vector<Node*> a) {
    value = v;
    args = a;
}

OpNode::OpNode(string o, Node* l, Node* r) {
    op = o;
    left = l;
    right = r;
}

VarAssignNode::VarAssignNode(string v, Node* e) {
    value = v;
    expr = e;
}

PropertyAssignNode::PropertyAssignNode(string p, Node* e) {
    property = p;
    expr = e;
}

IfNode::IfNode(Node* exp, Node* b, Node* eb) {
    expr = exp;
    block = b;
    elseBlock = eb;
}

WhileNode::WhileNode(Node* e, Node* b) {
    expr = e;
    block = b;
}

ForNode::ForNode(string i, Node* it, Node* b) {
    iter = i;
    iterable = it;
    block = b;
}

FuncDec::FuncDec(string v, vector<string> p, Node* b) {
    value = v;
    params = p;
    block = b;
}

ListNode::ListNode(vector<Node*> list) {
    value = list;
}

ReturnNode::ReturnNode(Node* e) {
    expr = e;
}

ClassMethod::ClassMethod(string v, string c, vector<string> p, Node* b) {
    value = v;
    class_name = c;
    params = p;
    block = b;
}

void StringNode::print() {
    cout << "string:" + value << endl;
}

void VarNode::print() {
    cout << "var:" + value << endl;
}

void FloatNode::print() {
    cout << "float:" << value << endl;
}

void FuncCall::print() {
    cout << "function call: " << value << endl;
    cout << "   args:" << endl;
    for (Node* arg : args) {
        cout << "       ";
        arg->print();
    }
}


void OpNode::print() {
    cout << "op: " << op << endl;
    cout << "   ";
    left->print();
    cout << "   ";
    right->print();
}

void BlockNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void StringNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void FloatNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void NotNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void IndexingNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void VarNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void FuncCall::accept(Visitor* visitor) {
    visitor->visit(this);
}

void OpNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void OpNode::mutate(Visitor* visitor) {
    visitor->mutate(this);
}

void OpNode::point(Visitor* visitor) {
    visitor->PointTo(this);
}

void VarAssignNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void PropertyAssignNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void IfNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void WhileNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ForNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void FuncDec::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ReturnNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ListNode::accept(Visitor* visitor) {
    visitor->visit(this);
}

void ClassMethod::accept(Visitor* visitor) {
    visitor->visit(this);
}

void VarNode::mutate(Visitor* visitor) {
    visitor->mutate(this);
}

void VarNode::point(Visitor* visitor) {
    visitor->PointTo(this);
}