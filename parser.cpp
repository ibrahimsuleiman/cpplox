#include "parser.h"

namespace lox{

Parser::Parser(const std::vector<std::shared_ptr<Token> >& tokens)
: current(0), tokens(tokens) {}

ExprPtr Parser::comma()
{
    ExprPtr expr = expression();
    while(match({COMMA}))
    {
        Token oper = *previous();
        ExprPtr right = expression();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }
    consume(SEMI_COLON, "Error. Expected ;");
    return expr;
}

ExprPtr Parser::expression(){
    return equality();
}

ExprPtr Parser::equality(){
    ExprPtr expr = comparison();
    while(match({BANG_EQUAL, EQUAL_EQUAL}))
    {
        Token oper = *previous();
        ExprPtr right = comparison();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }
    return expr;
}

ExprPtr Parser::comparison(){
    ExprPtr expr = addition();
    while(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL}))
    {
        Token oper = *previous();
        ExprPtr right = addition();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }
    return expr;  
}

ExprPtr Parser::addition(){
    ExprPtr expr = multiplication();
    while(match({MINUS, PLUS}))
    {
        Token oper = *previous();
        ExprPtr right = multiplication();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }
    return expr;  

}

ExprPtr Parser::multiplication(){
    ExprPtr expr = unary();
    while(match({SLASH, STAR}))
    {
        Token oper = *previous();
        ExprPtr right = unary();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }
    return expr;  

}

ExprPtr Parser::unary(){
    if(match({BANG, MINUS}))
    {
        Token oper = *previous();
        ExprPtr right = unary();
        return ExprPtr(new Unary(oper, std::move(right)));
    }
    return primary();
}

ExprPtr Parser::primary()
{
    if (match({FALSE})) return ExprPtr(new Literal(false));      
    if (match({TRUE})) return ExprPtr(new Literal(true));        
    if (match({NIL})) return ExprPtr(new Literal(nullptr));

    if (match({NUMBER, STRING})) {                           
      return ExprPtr(new Literal(previous()->literal));         
    }                                                      

    if (match({LEFT_PAREN})) {                               
      ExprPtr expr = expression();                            
      consume(RIGHT_PAREN, "Expected ')' after expression.\n");
      return ExprPtr(new Grouping(std::move(expr)));                      
    }  
    /* if none of the above cases is matched, we're on a token that
    ** cant start an expression
    */  
        throw ParseError::error(*peek(), "Expected expression.\n");                            
}

Token Parser::consume(TokenType type, std::string message){
    if(check(type)) return *advance();
    throw ParseError::error(*peek(), message);
}

bool Parser::match(const std::vector<TokenType>& types)
{
    for(TokenType t: types)
    {
        if(check(t))
        {
            advance();
            return true;
        }
    }
    return false;
}

void Parser::synchronize()
{
    advance();
    while(!isAtEnd())
    {   
        /* if the most recently consumed token is a semi colon, we can synchronize from there*/
        if(previous()->type == SEMI_COLON) return;
    /* if the next token is any of these, we're ready to resume parsing*/
        switch(peek()->type)
        {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
            default:
                break;

        }
        /*keep discarding tokens until ready to resume*/
        advance();
    }
}

ExprPtr Parser::parse()
{
    try{
        return comma();
    }catch(const ParseError& error)
    {
        return nullptr;
    }   
}
} // namespace lox