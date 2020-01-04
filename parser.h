#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include<exception>
#include<memory>
#include<vector>

#include"expr.h"
#include"lox.h"
#include"stmt.h"
#include"token.h"



/*
** The grammar for lox is defined as follows:
** -------------------------------------------------------------
** program        --> statement* EOF;
** statement      --> ExprStmt | printStmt;
** exprStmt       --> expression ";" ;
** printStmt      --> "print" expression ";" ;
** comma          --> expression ((",") expression)*
** expression     --> equality ;
** equality       --> comparison ( ( "!=" | "==" ) comparison )* ;
** comparison     --> addition ( ( ">" | ">=" | "<" | "<=" ) addition )* ;
** addition       --> multiplication ( ( "-" | "+" ) multiplication )* ;
** multiplication --> unary ( ( "/" | "*" ) unary )* ;
** unary          -->  ( "!" | "-" ) unary
**                 | primary ;
** primary        --> NUMBER | STRING | "false" | "true" | "nil"
**                   | "(" expression ")" ;
*/

namespace lox{
    class Parser{
        public:
            Parser(const std::vector<std::shared_ptr<Token> >& tokens);
            StmtPtr statement();
            StmtPtr expressionStatement();
            StmtPtr printStatement();
            ExprPtr comma();
            ExprPtr expression();
            ExprPtr equality();
            ExprPtr comparison();
            ExprPtr addition();
            ExprPtr multiplication();
            ExprPtr unary();
            ExprPtr primary();
            Token consume(TokenType type, std::string message);
            bool match(const std::vector<TokenType>& types);
            /* check type of next token*/
            bool check(const TokenType& type){
                if(isAtEnd()) return false;
                return peek()->type == type;
            }
            bool isAtEnd(){
                return peek()->type == END_OF_FILE;
            }
            /*I use raw ptrs because the tokens are owned by the tokens vector*/
            Token* advance(){
                if(!isAtEnd()) current++;
                return previous();
            }
            Token* peek(){
                return tokens[current].get();
            }
            Token* previous(){
                return tokens[current - 1].get();
            }

            void synchronize();
            /*return unique_ptr's to be owned by caller (aka Lox::run())*/
            std::vector<StmtPtr> parse();

        private:
            unsigned int current;
            /* might change this to unique_ptr and collect the raw pointers
            ** in Lox instead. To make this possible, I might also want to change
            ** the scanner tokens vector to unique_ptr's as well. Then, I can return 
            ** and transfer ownership of the tokens to the parser.
            ** */
            std::vector<std::shared_ptr<Token> > tokens;

    };

    class ParseError : public std::exception{
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