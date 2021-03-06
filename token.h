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
    AND, BREAK, CLASS, CONTINUE, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    END_OF_FILE
};

/* the pointer to void in this variant
** must never point anywhere. It can only assume a value of
** nullptr
 */
typedef std::variant<double, std::string, bool, void*> Object;

class Token {
    // typedef std::variant<double, std::string> Object;
public:
    Token() = default;
    Token(const TokenType& type, const std::string& lexeme,const Object& literal, int line);
    Token(const Token&) = default;
    ~Token()  = default;
    std::string toString();

public:
    TokenType type;
    std::string lexeme;
    Object literal;
    unsigned int line;


};


}// namespace lox

#endif