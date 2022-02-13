#include "visitor.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;

ostream& operator<<(ostream& os, const StackItem& st)
{
    if (st.type == Float) {
        os << st.value.float_value;
    }else if (st.type == String) {
        os << st.value.string_value;
    }else if (st.type == Bool) {
        if (st.value.bool_value) {
            os << "true";
        }else {
            os << "false";
        }
    }else if (st.type == List) {
        Visitor v;

        cout << "{ ";
    
        for (StackItem el : st.value.list_value) {
            cout << el;
            cout << " ";
        }

        cout << "}";
    }
    
    return os;
}

StackItem StackItem::operator%(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(fmod(value.float_value, item.value.float_value));
            return StackItem(Float, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator+(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value + item.value.float_value);
            return StackItem(Float, v);
        }else if (type == String) {
            Value v(value.string_value + item.value.string_value);
            return StackItem(String, v);
        }
    }

    if (type == List) {
        value.list_value.push_back(item);
        Value v(value.list_value);

        return StackItem(List, v);
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator-(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value - item.value.float_value);
            return StackItem(Float, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator*(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value * item.value.float_value);
            return StackItem(Float, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator/(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value / item.value.float_value);
            return StackItem(Float, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator==(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value == item.value.float_value);
            return StackItem(Bool, v);
        }else if (type == String) {
            Value v(value.string_value == item.value.string_value);
            return StackItem(Bool, v);
        }
        else if (type == Bool) {
            Value v(value.bool_value == item.value.bool_value);
            return StackItem(Bool, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator!=(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value != item.value.float_value);
            return StackItem(Bool, v);
        }else if (type == String) {
            Value v(value.string_value != item.value.string_value);
            return StackItem(Bool, v);
        }
        else if (type == Bool) {
            Value v(value.bool_value != item.value.bool_value);
            return StackItem(Bool, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator<=(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value <= item.value.float_value);
            return StackItem(Bool, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator>=(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value >= item.value.float_value);
            return StackItem(Bool, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator<(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value < item.value.float_value);
            return StackItem(Bool, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

StackItem StackItem::operator>(StackItem item) {
    if (type == item.type) {
        if (type == Float) {
            Value v(value.float_value > item.value.float_value);
            return StackItem(Bool, v);
        }
    }

    return StackItem(Float, Value(0.0));
}

void Println(FuncCall* node, Visitor* visitor) {
    cout.precision(15);
    
    for (Node* arg : node->args) {
        arg->accept(visitor);
        cout << visitor->Pop() << " ";
    }
    cout << endl;
}

void Print(FuncCall* node, Visitor* visitor) {
    cout.precision(15);

    for (Node* arg : node->args) {
        arg->accept(visitor);
        cout << visitor->Pop() << " ";
    }
}

void Readln(FuncCall* node, Visitor* visitor) {
    node->args[0]->accept(visitor);
    cout << visitor->Pop();
    string input;

    getline (cin, input);

    StackItem item;
    Value v(input);
    item.type = String;
    item.value = v;
    visitor->ExprStack.push_back(item);
}

void Str(FuncCall* node, Visitor* visitor) {
    node->args[0]->accept(visitor);
    StackItem item = visitor->Pop();

    if (item.type == Float) {
        string s = to_string(item.value.float_value);
        Value v(s);

        StackItem newItem;
        newItem.type = String;
        newItem.value = v;

        visitor->ExprStack.push_back(newItem);
    }else if (item.type == String) {
        visitor->ExprStack.push_back(item);
    }else if (item.type == Float) {
        string s = to_string(item.value.float_value);
        Value v(s);

        StackItem newItem;
        newItem.type = String;
        newItem.value = v;

        visitor->ExprStack.push_back(newItem);
    }
}

void Number(FuncCall* node, Visitor* visitor) {
    node->args[0]->accept(visitor);
    StackItem item = visitor->Pop();

    if (item.type == String) {
        double d = atof(item.value.string_value.c_str());
        Value v(d);

        StackItem newItem;
        newItem.type = Float;
        newItem.value = v;

        visitor->ExprStack.push_back(newItem);
    }else if (item.type == Float) {
        visitor->ExprStack.push_back(item);
    }if (item.type == Bool) {
        double d = item.value.bool_value;
        Value v(d);

        StackItem newItem;
        newItem.type = Float;
        newItem.value = v;

        visitor->ExprStack.push_back(newItem);
    }
}

void Size(FuncCall* node, Visitor* visitor, StackItem obj) {
    visitor->obj = false;
    if (obj.type == List) {
        StackItem size;

        Value v(double(obj.value.list_value.size()));
        size.type = Float;
        size.value = v;

        
        visitor->ExprStack.push_back(size);
    }else if (obj.type == String) {
        StackItem size;
        Value v(double(obj.value.string_value.size()));
        size.type = Float;
        size.value = v;
        visitor->ExprStack.push_back(size);
    }
}

void Slice(FuncCall* node, Visitor* visitor, StackItem obj) {
    visitor->obj = false;
    node->args[0]->accept(visitor);
    StackItem start = visitor->Pop();
    node->args[1]->accept(visitor);
    StackItem end = visitor->Pop();

    if (obj.type == List) {
        if (start.type == end.type && start.type == Float) {
            vector<StackItem> list;
            for (int i = start.value.float_value; i <= end.value.float_value; i++) {
                list.push_back(obj.value.list_value[i]);
            }

            StackItem item;
            Value v(list);

            item.type = List;
            item.value = v;

            visitor->ExprStack.push_back(item);
        }
    }else if (obj.type == String) {
        if (start.type == end.type && start.type == Float) {
            string s;
            for (int i = start.value.float_value; i <= end.value.float_value; i++) {
                s += obj.value.string_value[i];
            }

            StackItem item;
            Value v(s);

            item.type = String;
            item.value = v;

            visitor->ExprStack.push_back(item);
        }
    }
}


void Range(FuncCall* node, Visitor* visitor) {
    double start = 0;
    double end = 0;
    double step = 0;

    node->args[0]->accept(visitor);
    start = visitor->Pop().value.float_value;

    node->args[1]->accept(visitor);
    end = visitor->Pop().value.float_value;

    node->args[2]->accept(visitor);
    step = visitor->Pop().value.float_value;

    vector<StackItem> result;

    if (step > 0) {
        for (double i = start; i <= end; i += step) {
            StackItem item;
            Value v(i);
            item.type = Float;
            item.value = v;
            result.push_back(item);
        }
    }else if (step < 0) {
        for (double i = start; i >= end; i -= step) {
            StackItem item;
            Value v(i);
            item.type = Float;
            item.value = v;
            result.push_back(item);
        }
    }

    StackItem range;
    Value v(result);
    range.type = List;
    range.value = v;

    visitor->ExprStack.push_back(range);
}


void _Push(FuncCall* node, Visitor* visitor, StackItem obj) {
    visitor->obj = false;
    if (obj.type == List) {
        vector<StackItem>* list = &obj.value.list_value;
        node->args[0]->accept(visitor);
        list->push_back(visitor->Pop());

        visitor->ExprStack.push_back(obj);
        visitor->mut_obj = true;
    }
}

Visitor::Visitor() {
    block_no = 0;
    continue_block = true;
    VMap["true"] = StackItem(Bool, Value(true));
    VMap["false"] = StackItem(Bool, Value(false));
    call_no.push_back(0);
    FMap["println"] = Println;
    FMap["print"] = Print;
    FMap["range"] = Range;
    FMap["str"] = Str;
    FMap["number"] = Number;
    FMap["readln"] = Readln;
    OBJFMap["size"] = Size;
    OBJFMap["push"] = _Push;
    OBJFMap["slice"] = Slice;
}

void Visitor::Push(Type type, Value value) {
    StackItem item(type, value);
    ExprStack.push_back(item);
}

StackItem Visitor::Pop() {
    StackItem back = ExprStack.back();
    ExprStack.pop_back();

    return back;
}

void Visitor::visit(BlockNode* node) {
    block_no += 1;
    for (Node* n : node->lines) {
        n->accept(this);
        if (!continue_block) {
            if (block_no > call_no.back()) {
                block_no -= 1;
                break;
            }
            continue_block = true;
            call_no.pop_back();
        }  
    }
}

void Visitor::visit(ReturnNode* node) {
    node->expr->accept(this);
    
    continue_block = false;
}

void Visitor::visit(StringNode* node) {
    Value value(node->value);
    Push(String, value);
}

void Visitor::visit(FloatNode* node) {
    Value value(node->value);
    Push(Float, value);
}

void Visitor::visit(VarNode* node) {
    if (mut_obj) {
        VMap[node->value] = Pop();
        mut_obj = false;
    }else {
        ExprStack.push_back(VMap[node->value]);
    }
}

void Visitor::visit(IndexingNode* node) {
    node->object->accept(this);
    StackItem object = Pop();
    node->indexExpr->accept(this);
    StackItem indexExpr = Pop();

    if (indexExpr.type == Float) {
        double v = indexExpr.value.float_value;
        if (object.type == String) {
            if (v < 0) {
                v += object.value.string_value.size();
            }
            string ch = string(1, object.value.string_value[v]);
            StackItem item;
            Value v(ch);
            item.value = v;
            item.type = String;

            ExprStack.push_back(item);
        }else if (object.type == List) {
            if (v < 0) {
                v += object.value.list_value.size();
            }
            StackItem item = object.value.list_value[v];
            ExprStack.push_back(item);
        }
     }
}

void Visitor::visit(OpNode* node) {

    if (node->op == ".") {
        Dot(node);
    }
    else {
        node->left->accept(this);
        node->right->accept(this);

        if (node->op == "+") {
            Add();
        }else if (node->op == "-") {
            Sub();
        }else if (node->op == "%") {
            Mod();
        }else if (node->op == "*") {
            Mul();
        }else if (node->op == "/") {
            Div();
        }else if (node->op == "^") {
            Pow();
        }else if (node->op == "==") {
            Eqeq();
        }else if (node->op == "<=") {
            Leq();
        }else if (node->op == ">=") {
            Geq();
        }else if (node->op == ">") {
            G();
        }else if (node->op == "<") {
            L();
        }else if (node->op == "and") {
            And();
        }else if (node->op == "or") {
            Or();
        }
    }
}


void Visitor::visit(FuncCall* node) {
    if (obj) {
        call_no.push_back(block_no);
        OBJFMap[node->value](node, this, currObj);
        obj = false;
        call_no.pop_back();
        continue_block = true;
    }else {

        if (!FMap.count(node->value)) {
            cout << "error: the function " << node->value << " has not been defined";
            exit(-1);
        }

        call_no.push_back(block_no);
        FMap[node->value](node, this);
        call_no.pop_back();
        continue_block = true;
    }
}

void Visitor::visit(VarAssignNode* node) {
    node->expr->accept(this);
    VMap[node->value] = Pop();
}

void Visitor::visit(IfNode* node) {
    node->expr->accept(this);
    StackItem condition = Pop();

    if (condition.value.bool_value) {
        node->block->accept(this);
    }else {
        node->elseBlock->accept(this);
    }
}

void Visitor::visit(WhileNode* node) {
    node->expr->accept(this);
    StackItem condition = Pop();

    while (condition.value.bool_value) {
        node->block->accept(this);
        node->expr->accept(this);
        condition = Pop();
    }
}

void Visitor::visit(ForNode* node) {
    node->iterable->accept(this);
    StackItem iterable = Pop();

    if (iterable.type == String) {
        for (char i : iterable.value.string_value) {
            VMap[node->iter] = StackItem(String, string(1, i));
            node->block->accept(this);
        }
    }if (iterable.type == List) {
        for (StackItem el : iterable.value.list_value) {
            VMap[node->iter] = el;
            node->block->accept(this);
        }
    }
}

void Visitor::visit(FuncDec* node) {
    FMap[node->value] = [=](FuncCall* call_node, Visitor* visitor) {
        auto copy = visitor->VMap;
        
        for (int i = 0; i < call_node->args.size(); i++) {
            call_node->args[i]->accept(visitor);
            visitor->VMap[node->params[i]] = visitor->Pop();
        }

        node->block->accept(visitor); 
        visitor->VMap = copy;
    };
}

void Visitor::visit(ListNode* node) {
    StackItem list;
    vector<StackItem> value;

    for (Node* el : node->value) {
        el->accept(this);
        value.push_back(Pop());
    }

    Value v(value);
    list.type = List;
    list.value = v;

    ExprStack.push_back(list);
} 


void Visitor::visit(NotNode* node) {
    node->expr->accept(this);

    StackItem* item = &ExprStack.back();

    item->value.bool_value = !item->value.bool_value;
}


void Visitor::Mod() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left % right);
}

void Visitor::Add() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left + right);
}

void Visitor::Sub() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left - right);
}

void Visitor::Mul() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left * right);
}

void Visitor::Pow() {
    StackItem right = Pop();
    StackItem left = Pop();

    if (left.type == right.type) {
        if (left.type == Float) {
            double res = pow(left.value.float_value, right.value.float_value);
            Value v(res);
            StackItem item;
            item.type = Float;
            item.value = v;
            ExprStack.push_back(item);
        }
    }
}

void Visitor::Div() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left / right);
}

void Visitor::Eqeq() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left == right);
}

void Visitor::Leq() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left <= right);
}

void Visitor::Geq() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left >= right);
}

void Visitor::L() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left < right);
}

void Visitor::G() {
    StackItem right = Pop();
    StackItem left = Pop();

    ExprStack.push_back(left > right);
}

void Visitor::And() {
    StackItem right = Pop();
    StackItem left = Pop();
    

    ExprStack.push_back( StackItem(Bool, Value(left.value.bool_value && right.value.bool_value)));
}

void Visitor::Or() {
    StackItem right = Pop();
    StackItem left = Pop();
    

    ExprStack.push_back(StackItem(Bool, Value(left.value.bool_value || right.value.bool_value)));
}

void Visitor::Dot(OpNode* node) {
    node->left->accept(this);
    StackItem left = Pop();
    obj = true;
    currObj = left;
    node->right->accept(this);

    if (mut_obj) {
        node->left->accept(this);
    }
}