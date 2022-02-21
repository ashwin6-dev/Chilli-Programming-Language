#include "visitor.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;


StackItem FunctionAsStackItem(function<void(FuncCall*, Visitor*, StackItem)> f) {
    StackItem fn;
    Value v(f);

    fn.type = Function;
    fn.value = v;

    return fn;
}

StackItem FunctionAsStackItem(function<void(FuncCall*, Visitor*)> f) {
    StackItem fn;
    Value v(f);

    fn.type = Function;
    fn.value = v;

    return fn;
}

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
    }else if (st.type == Function) {
        os << "Function";
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
    
    for (Node* arg : node->args) {
        arg->accept(visitor);
        cout << visitor->Pop() << " ";
    }
    cout << endl;
}

void Print(FuncCall* node, Visitor* visitor) {
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

Visitor::Visitor() {
    block_no = 0;
    continue_block = true;
    VMap["true"] = StackItem(Bool, Value(true));
    VMap["false"] = StackItem(Bool, Value(false));
    call_no.push_back(0);
    VMap["println"] = FunctionAsStackItem(Println);
    VMap["print"] = FunctionAsStackItem(Print);
    VMap["range"] = FunctionAsStackItem(Range);
    VMap["str"] = FunctionAsStackItem(Str);
    VMap["number"] = FunctionAsStackItem(Number);
    VMap["readln"] = FunctionAsStackItem(Readln);
}

void Visitor::Push(Type type, Value value) {
    next_item.type = type;
    next_item.value = value;
    ExprStack.push_back(next_item);
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
    if (obj) {
        if (!currObj.value.object_props.count(node->value)) {
            cout << "error: no property called " << node->value;
            exit(-1);
        }
        ExprStack.push_back(currObj.value.object_props[node->value]);
    }
    else {
        ExprStack.push_back(VMap[node->value]);
    }
}

void Visitor::mutate(VarNode* node) {
    if (obj) {
        update_me->value.object_props[node->value] = Pop();
    }else {
        VMap[node->value] = Pop();
    }
}

void Visitor::PointTo(VarNode* node) {
    if (obj) {
        update_me = &update_me->value.object_props[node->value];
    }else {
        update_me = &VMap[node->value];
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
            Value v(ch);
            Push(String, v);
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

void Visitor::visit(FuncCall *node)
{
    if (obj)
    {
        call_no.push_back(block_no);

        StackItem fn = currObj.value.object_props[node->value];
        if (fn.type == Function)
        {
            fn.value.method_value(node, this, currObj);
        }

        obj = false;
        call_no.pop_back();
        continue_block = true;
    }
    else if (!VMap.count(node->value))
    {
        cout << "error: the function " << node->value << " has not been defined";
        exit(-1);
    }
    else
    {
        call_no.push_back(block_no);
        StackItem fn = VMap[node->value];
        if (fn.type == Function)
        {
            if (fn.value.func_args_no == 2)
            {
                fn.value.func_value(node, this);
            }
            else
            {
                fn.value.method_value(node, this, StackItem());
                mut_obj = false;
            }
        }
        call_no.pop_back();
        continue_block = true;
    }
}

void Visitor::visit(VarAssignNode* node) {
    node->expr->accept(this);
    VMap[node->value] = Pop();
}

void Visitor::visit(PropertyAssignNode* node) {
    node->expr->accept(this);
    StackItem value = Pop();
    StackItem* object;
    vector<string> props;
    string cur_prop = "";

    for (char ch : node->property) {
        if (ch != '.') { 
            cur_prop += ch;
        }else {
            props.push_back(cur_prop);
            cur_prop = "";
        }
    }

    if (cur_prop != "") {
        props.push_back(cur_prop);
    }

    for (int i = 0; i < props.size() - 1; i++) {
        string prop = props[i];
        if (i == 0) {
            object = &VMap[prop];
        }else {
            object = &object->value.object_props[prop];
        }
    }
    cur_prop = props.back();
    object->value.object_props[cur_prop] = value;
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
    function<void(FuncCall* call_node, Visitor* visitor)> func = [=](FuncCall* call_node, Visitor* visitor) {
        auto copy = visitor->VMap;
        
        for (int i = 0; i < call_node->args.size(); i++) {
            call_node->args[i]->accept(visitor);
            visitor->VMap[node->params[i]] = visitor->Pop();
        }

        node->block->accept(visitor); 
        visitor->VMap = copy;
    };

    Value v(func);
    StackItem fn(Function, v);
    
    VMap[node->value] = fn;
}

void Visitor::visit(ClassMethod* node) {
    function<void(FuncCall*, Visitor*, StackItem)> func = [=](FuncCall* call_node, Visitor* visitor, StackItem self) {
        visitor->obj = false;
        auto copy = visitor->VMap;

        if (node->value == "init") {
            self = visitor->classes[node->class_name];
        }
        
        for (int i = 0; i < call_node->args.size(); i++) {
            call_node->args[i]->accept(visitor);
            visitor->VMap[node->params[i]] = visitor->Pop();
        }

        visitor->VMap["this"] = self;
        node->block->accept(visitor); 
        visitor->ExprStack.push_back(visitor->VMap["this"]);  
        visitor->mut_obj = true;
        visitor->VMap = copy;
    };

    StackItem fn = FunctionAsStackItem(func);

    if (node->value == "init") {
        VMap[node->class_name] = fn;
    }else if (!classes.count(node->class_name)) {
        Value v(node->class_name, "obj");
        v.object_props[node->value] = fn;
        StackItem obj(Object, v);

        classes[node->class_name] = obj;
    }else {
        classes[node->class_name].value.object_props[node->value] = fn;
    }  
}

void ListPush(FuncCall* node, Visitor* visitor, StackItem self) {
    visitor->obj = false;
    node->args[0]->accept(visitor);
    StackItem item = visitor->Pop();

    self.value.list_value.push_back(item);
    visitor->ExprStack.push_back(self);
    visitor->mut_obj = true;
}

void ListSize(FuncCall* node, Visitor* visitor, StackItem self) {
    double size = self.value.list_value.size();
    Value v(size);
    visitor->Push(Float, v);
}

void Visitor::visit(ListNode* node) {
    vector<StackItem> value;

    for (Node* el : node->value) {
        el->accept(this);
        value.push_back(Pop());
    }

    Value v(value);
    v.object_props["push"] = FunctionAsStackItem(ListPush);
    v.object_props["size"] = FunctionAsStackItem(ListSize);

    Push(List, v);
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
            Push(Float, v);
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
    

    ExprStack.push_back(StackItem(Bool, Value(left.value.bool_value && right.value.bool_value)));
}

void Visitor::Or() {
    StackItem right = Pop();
    StackItem left = Pop();
    

    ExprStack.push_back(StackItem(Bool, Value(left.value.bool_value || right.value.bool_value)));
}

void Visitor::Dot(OpNode* node) {
    node->left->accept(this);
    currObj = Pop();
    obj = true;
    node->right->accept(this);
    obj = false;

    if (mut_obj) {
        node->left->mutate(this);
        mut_obj = false;
    }
}

void Visitor::PointTo(OpNode* node) {
    if (node->op == ".") {
        node->left->point(this);
        obj = true;
        node->right->point(this);
        obj = false;
    }
}
void Visitor::mutate(OpNode* node) {
    if (node->op == ".") {
        node->left->point(this);
        obj = true;
        node->right->mutate(this);
        obj = false;
    }
}