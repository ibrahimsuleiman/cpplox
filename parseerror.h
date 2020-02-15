#ifndef LOX_PARSE_ERROR_H
#define LOX_PARSE_ERROR_H

#include<exception>
#include<string>

#include"lox.h"
#include"token.h"


namespace lox{

class ParseError : public std::exception {
public:
    ParseError()
    {

    }
    const char* what() const noexcept override
    {
        return "Parse Error";
    }
    static ParseError error(const Token& token, const std::string& msg)
    {
        Lox::error(token, msg);
        return ParseError();
    }

};


}// namespace lox
#endif