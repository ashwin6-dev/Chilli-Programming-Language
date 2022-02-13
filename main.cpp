#include "parser.h"
#include "visitor.h"
#include <string>
#include<fstream>
#include<sstream>

using namespace std;


int main(int argc, char** argv) {
    string s;
    ifstream f(argv[1]);

    if (f) {
      ostringstream ss;
      ss << f.rdbuf();
      s = ss.str();
    }

    Parser p(s);
    
    Node* program = p.Parse().node;
    Visitor v;

    program->accept(&v);
}

