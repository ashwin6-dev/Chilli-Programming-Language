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
    List,
    Function,
    Object
};

class Visitor;
class StackItem;

class Value {
    public:
        double float_value;
        string string_value;
        bool bool_value;
        vector<StackItem> list_value;
        string object_name;
        map<string, StackItem> object_props;
        function<void(FuncCall*, Visitor*, StackItem)> method_value;
        function<void(FuncCall*, Visitor*)> func_value;
        int func_args_no;

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

        Value(string o_name, string s) {
            if (s == "obj") {
                object_name = o_name;
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

        Value(function<void(FuncCall*, Visitor*, StackItem)> f) {
            method_value = f;
            func_args_no = 3;
        }

        Value(function<void(FuncCall*, Visitor*)> f) {
            func_value = f;
            func_args_no = 2;
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
        map<string, StackItem> classes;
        map<string, StackItem> VMap;
        int block_no;
        vector<int> call_no;
        bool continue_block;
        bool obj = false;
        bool mut_obj = false;
        StackItem currObj;
        StackItem next_item;
        StackItem* update_me;
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
        void mutate(VarNode* node);
        void PointTo(VarNode* node);
        void visit(OpNode* node);
        void visit(FuncCall* node);
        void visit(FuncCall* node, StackItem obj);
        void visit(VarAssignNode* node);
        void visit(PropertyAssignNode* node);
        void visit(IfNode* node);
        void visit(WhileNode* node);
        void visit(ForNode* node);
        void visit(FuncDec* node);
        void visit(ReturnNode* node);
        void visit(ListNode* node);
        void visit(ClassMethod* node);
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
        void mutate(OpNode* node);
        void PointTo(OpNode* node);
};