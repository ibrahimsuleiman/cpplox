#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include<exception>
#include<memory>
#include<vector>

#include"expr.h"
#include"lox.h"
#include"parseerror.h"
#include"stmt.h"
#include"token.h"



/*
** The grammar for lox is defined as follows:
** -------------------------------------------------------------
** program        --> declaration* EOF;
** declaration    --> varDecl | statement;
** varDecl        --> "var" IDENTIFIER ("=" expression)? ";" ;
** statement      --> exprStmt | printStmt | ifStmt | whileStmt | forStmt | block;
** ifStmt         --> "if" "(" expression ")" ("else" statement)?;
** whileStmt      --> "while" "(" expression ")" statment ;
** forStmt        --> "for" "(" varDecl | exprStmt | ";" expression? ";" expression? ")" statement
** block          --> "{" declaration* "}";
** exprStmt       --> comma ";" ;
** printStmt      --> "print" comma ";" ;
** comma          --> expression ((",") expression)*
** expression     --> assignment;
** assignment     -->  IDENTIFIER "=" assignment | logic_or ;
** logic_or       -->  logic_and ("or" logic_and)*;
** logic_and      -->  equality ("and" equality)*;
** equality       --> comparison ( ( "!=" | "==" ) comparison )* ;
** comparison     --> addition ( ( ">" | ">=" | "<" | "<=" ) addition )* ;
** addition       --> multiplication ( ( "-" | "+" ) multiplication )* ;
** multiplication --> unary ( ( "/" | "*" ) unary )* ;
** unary          -->  ( "!" | "-" ) unary
**                 | primary ;
** primary        --> NUMBER | STRING | "false" | "true" | "nil"
**                   | "(" expression ")" | IDENTIFIER | "break" | "continue";
*/

namespace lox {


class Parser {
public:
    Parser(const std::vector<Token> &tokens);

    StmtPtr declaration();
    StmtPtr varDeclaration();
    FunPtr function(const std::string &kind);
    StmtPtr statement();
    std::vector<StmtPtr> block();
    StmtPtr expressionStatement();
    StmtPtr printStatement();
    StmtPtr ifStatement();
    StmtPtr whileStatement();
    StmtPtr forStatement();
    ExprPtr comma();
    ExprPtr expression();
    ExprPtr assignment();
    ExprPtr logicOr();
    ExprPtr logicAnd();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr addition();
    ExprPtr multiplication();
    ExprPtr unary();
    ExprPtr primary();
    ExprPtr finishCall(ExprPtr callee);
    Token consume(TokenType type, const std::string& message);
    bool match(const std::vector<TokenType>& types);

    bool check(const TokenType& type) {
        if(isAtEnd()) return false;
        return peek().type == type;
    }
    bool isAtEnd() {
        return peek().type == END_OF_FILE;
    }

    Token& advance() {
        if(!isAtEnd()) current++;
        return previous();
    }
    Token& peek() {
        return tokens[current];
    }
    Token& previous() {
        return tokens[current - 1];
    }

    void synchronize();
    /*return unique_ptr's to be owned by caller (aka Lox::run())*/
    std::vector<StmtPtr> parse();

private:
    unsigned int current;
    std::vector<Token> tokens;

};

} // namespace lox

#endif