#ifndef LOX_H
#define LOX_H

#include<fstream>
#include<iostream>
#include<string>

#include"interpreter.h"
#include"token.h"

namespace lox
{

class Lox {
public:
    Lox():source("") {};
    Lox(const Lox&) = delete; /* prohibit copying */
    Lox(const std::string& source): source(source) {};
    ~Lox() = default;
    void runFile();
    void runPrompt();
    void run(const std::string& buf);
    static void error(int line, const std::string& message)
    {
        report(line, "", message);
    }
    static void runtimeError(const RuntimeError& err) {
        std::cerr << err.message() <<
                  "\n[line " << err.token.line << "]" << std::endl;
        hadRuntimeError = true;
    }
    static void error(const Token& token, const std::string& msg);
    static void report(int line, const std::string& where, const std::string& message);
    static bool hadError;
    static bool hadRuntimeError;


private:
    std::string source;


};



} //namespace lox


#endif