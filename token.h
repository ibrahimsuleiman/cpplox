#ifndef TOKEN_H
#define TOKEN_H

#include<string>
#include<variant>

namespace lox
{
    
enum TokenType
{
    /*single-character tokens*/
    LEFT_PAREN, RIGHT_PAREN,LEFT_BRACE, RIGHT_BRACE,
    DOT, MINUS, PLUS, SLASH,SEMI_COLON, BANG, STAR,
    COMMA,

    /* one or 2 character tokens*/
    EQUAL, BANG_EQUAL,EQUAL_EQUAL,
    GREATER,GREATER_EQUAL, LESS,LESS_EQUAL,

    /* literals */
    IDENTIFIER, STRING, NUMBER,

    /*key words*/
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    END_OF_FILE
};

typedef std::variant<double, std::string> Object;

class Token {
   // typedef std::variant<double, std::string> Object;
public:
    Token() = default;
    Token(TokenType type, std::string lexeme,Object literal, int line);
    Token(const Token&) = default;
    ~Token()  = default;
    std::string toString();

private:
    TokenType type;
    std::string lexeme;
    Object literal;
    int line;


};


}// namespace lox

#endif