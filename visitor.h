#include "node.h"
#include <iostream>
#include <map>
#include <functional>
#include <vector>

using namespace std;

enum Type {
    Float,
    String,
    Bool,
    List
};

class StackItem;

class Value {
    public:
        double float_value;
        string string_value;
        bool bool_value;
        vector<StackItem> list_value;

        Value() {}

        Value(double f) {
            float_value = f;

            if (f > 0) {
                bool_value = true;
            }else {
                bool_value = false;
            }
        };

        Value(string s) {
            string_value = s;

            if (s.size() > 0) {
                bool_value = true;
            }else {
                bool_value = false;
            }
        };


        Value(bool b) {
            bool_value = b;
        };

        Value(vector<StackItem> l) {
            list_value = l;

            if (l.size() > 0) {
                bool_value = false;
            }else {
                bool_value = true;
            }
        }
};


class StackItem {
    public:
        Type type;
        Value value;
        StackItem() {};
        StackItem(Type t, Value v) {
            type = t;
            value = v;
        };
        StackItem operator%(StackItem item);
        StackItem operator+(StackItem item);
        StackItem operator-(StackItem item);
        StackItem operator*(StackItem item);
        StackItem operator/(StackItem item);
        StackItem operator==(StackItem item);
        StackItem operator!=(StackItem item);
        StackItem operator<=(StackItem item);
        StackItem operator>=(StackItem item);
        StackItem operator>(StackItem item);
        StackItem operator<(StackItem item);
};


class Visitor {
    public:
        map<string, function<void(FuncCall*, Visitor*)>> FMap;
        map<string, function<void(FuncCall*, Visitor*, StackItem)>> OBJFMap;
        map<string, StackItem> VMap;
        int block_no;
        vector<int> call_no;
        bool continue_block;
        bool obj = false;
        bool mut_obj = false;
        StackItem currObj;
        Visitor();
        vector<StackItem> ExprStack;
        StackItem Pop();
        void Push(Type type, Value value);
        void visit(BlockNode* node);
        void visit(StringNode* node);
        void visit(FloatNode* node);
        void visit(NotNode* node);
        void visit(IndexingNode* node);
        void visit(VarNode* node);
        void visit(OpNode* node);
        void visit(FuncCall* node);
        void visit(FuncCall* node, StackItem obj);
        void visit(VarAssignNode* node);
        void visit(IfNode* node);
        void visit(WhileNode* node);
        void visit(ForNode* node);
        void visit(FuncDec* node);
        void visit(ReturnNode* node);
        void visit(ListNode* node);
        void Mod();
        void Add();
        void Sub();
        void Mul();
        void Div();
        void Pow();
        void Eqeq();
        void Leq();
        void Geq();
        void L();
        void G();
        void And();
        void Or();
        void Dot(OpNode* node);
};