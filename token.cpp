#include"token.h"


namespace lox
{

Token::Token(const TokenType& type, const std::string& lexeme,const Object& literal, int line):
    type(type),
    lexeme(lexeme),
    literal(literal),
    line(line)
{

}

std::string Token::toString()
{
    if(type == NUMBER)
    {
        double d = std::get<double>(literal);
        return std::string(std::to_string(type) + " " + lexeme + " " + std::to_string(d));
    }
    else if(type == STRING)
    {
        return std::string(std::to_string(type) + " " + lexeme + " " + std::get<std::string>(literal));
    }
    else
        return std::string(std::to_string(type) + " " + lexeme);


}
} //namespace lox
