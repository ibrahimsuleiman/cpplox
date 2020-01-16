#ifndef LOX_ENVIRONMENT_H
#define LOX_ENVIRONMENT_H

#include<map>
#include<memory>

#include"token.h"
#include"interpreter.h"

namespace lox {
class Environment {
public:
    /* bind a new name to a value */
    Environment() = default;
    Environment(Environment* enclosing): enclosing(enclosing) {}
    void define(const std::string& name, const Object& value) {
        /*
        ** By not checking if the name already exists, we permit
        ** variable redefinition. E.g. this is allowed:
        ** var a = "me";
        ** print a;
        ** var a = "you";
        ** We permit this only for global variables so that REPL mode
        ** is consistent with script execution
         */

        values[name] = value;
    }

    Object get(const Token& name) {
        if(!(values.find(name.lexeme) == values.end())) return values[name.lexeme];

        /*try enclosing scope if variable is not found*/
        if(enclosing != nullptr) {
            return enclosing->get(name);
        }

        throw RuntimeError(name, "Undefined Identifier '" + name.lexeme + "' .");
    }

    void assign(const Token& name, const Object& value) {
        if(!(values.find(name.lexeme) == values.end())) {
            values[name.lexeme] = value;
            return;
        }
        /*try enclosing scope if variable is not found*/
        if(enclosing != nullptr) {
            enclosing->define(name.lexeme, value);
            return;
        }

        throw RuntimeError(name, "Undefined Identifier '" + name.lexeme + "' .");
    }
private:
    std::map<std::string, Object> values;
    Environment *enclosing;

};
} // namespace lox
#endif