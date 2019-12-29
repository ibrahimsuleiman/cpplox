#ifndef SCANNER_H
#define SCANNER_H

#include<string>
#include<vector>
#include<map>
#include<variant>
#include<memory>

#include"token.h"

class Scanner {
    typedef std::variant<double, std::string> Object;
public:
    Scanner(const std::string& source);
    Scanner(const Scanner&) = default;
    ~Scanner() = default;

    std::vector<std::shared_ptr<Token> > scanTokens();

    void addToken(TokenType type) {
        addToken(type, "");
    }
    void addToken(TokenType type, Object literal);
    void matchString();
    void matchNumber();
    void matchIdentifier();
    void scan();

    /* Scanner only looks ahead at most 2 characters, so we disallow arbitrary peeking*/
    char peek() {
        if(isAtEnd()) return '\0';
        return source[current];
    }
    char peekNext() {
        if(current + 1 >= source.length()) return '\0';
        return source[current + 1];
    }
    char advance() {
        current++;
        return source[current - 1];
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }
    bool match(const char expected);
    bool isAtEnd() {
        return current >= source.length();
    }


private:
    std::string source;
    std::vector<std::shared_ptr<Token> > tokens;
    int start;
    int current;
    int line;
    std::map<std::string, TokenType> keywords =
    {
        {"and",      AND},
        {"class",  CLASS},
        {"else",    ELSE},
        {"false",  FALSE},
        {"for",      FOR},
        {"fun",      FUN},
        {"if",        IF},
        {"nil",      NIL},
        {"or",         OR},
        {"print",  PRINT},
        {"return", RETURN},
        {"super",   SUPER},
        {"this",     THIS},
        {"true",     TRUE},
        {"var",       VAR},
        {"while",   WHILE}
    };


};

#endif