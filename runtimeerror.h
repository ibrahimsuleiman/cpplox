#ifndef LOX_RUNTIME_ERROR_H
#define LOX_RUNTIME_ERROR_H

#include<exception>
#include<string>

#include"token.h"

namespace lox{

class RuntimeError : public std::exception {
public:
    RuntimeError(const Token& token, const std::string& msg):token(token), msg(msg) {}
    const char* what() const noexcept override
    {
        return "Runtime error";
    }
    std::string message()const {
        return msg;
    }
    friend class Lox;
    friend class Environment;
private:
    Token token;
    std::string msg;
};

} // namespace lox

#endif