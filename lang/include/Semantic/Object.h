#include <string>
#include <vector>

#include "Parser/AstNode.h"

/* **************************

Short-hand Macros

************************** */

#define OBJ_CONSTRUCTOR(OBJ_NAME) \
  OBJ_NAME(std::string value) : Object(#OBJ_NAME, value) {}

/* **************************

Objects/Values

************************** */
namespace ObjTypes {

    /* CORE INFRA */
struct ObjectBase {
    std::string typeName;
    std::string value;

    ObjectBase(std::string t, std::string v) : typeName(t), value(v) {}
    std::string inspect() { return "Object name: " + value; }
    virtual ~ObjectBase() = default;
};

template <typename __RepresentationReturnType>
struct ObjectInter : ObjectBase {
    ObjectInter(std::string t, std::string v) : ObjectBase(t, v) {}
    virtual __RepresentationReturnType represent() = 0;
};

    /* CORE OBJECTS */
struct Integer : ObjectInter<int> {
    Integer(std::string v) : ObjectInter("Integer", v) {}
    int represent() override { return std::stoi(value); }
};

struct Boolean : ObjectInter<bool> {
    Boolean(std::string v) : ObjectInter("Boolean", v) {}
    bool represent() override { return value == "true"; }
};

struct None : ObjectInter<void> {
  None(std::string value) : ObjectInter("None", value) {};
  void represent() {};
};

}  // namespace ObjTypes

/* **************************

Evaluation Object

************************** */

class Evaluator {
 public:
  Ast::Node* head;
  Parser parser;

  Evaluator(Parser&& p) : parser(std::move(p)), head(parser.getAst()) {
    parser.build();
  }

  Ast::Node* getHead() { return head; }

  void eval(Ast::Node* root) {};

  ObjTypes::Integer evalInteger(Ast::Terminal* root) {};
};