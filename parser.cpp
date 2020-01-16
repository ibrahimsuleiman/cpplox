#include "parser.h"

namespace lox {

Parser::Parser(std::vector<std::unique_ptr<Token> > tokens)
    : current(0), tokens(std::move(tokens)) {}


StmtPtr Parser::declaration()
{

    try
    {
        if(match({VAR})) return varDeclaration();

        return statement();


    }
    catch(const ParseError& err)
    {   /*
        ** The declaration function is what is called
        ** repeatedly when we parse a list of statements.
        ** It is thus the right place to synchronize the
        ** parser when it goes into panic mode
        */
        synchronize();
        return nullptr;
    }

}

StmtPtr Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expected variable name.");

    ExprPtr initializer;

    if(match({EQUAL}))
    {   /* copy elision, aka move asgnt operator */
        initializer = expression();
    }

    consume(SEMI_COLON, "Expected ';' after variable declaration.");
    return StmtPtr(new Var(name, std::move(initializer)));
}
StmtPtr Parser::statement() {
    if(match({IF})) return ifStatement();
    if(match({PRINT})) return printStatement();
    if(match({WHILE})) return whileStatement();
    if(match({FOR})) return forStatement();
    if(match({LEFT_BRACE})) return StmtPtr(new Block(block()));
    return expressionStatement();
}

std::vector<StmtPtr> Parser::block()
{
    std::vector<StmtPtr> statements;

    while(!check(RIGHT_BRACE) && !isAtEnd())
    {
        statements.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expected '}' after block.");
    return statements;
}

StmtPtr Parser::expressionStatement() {
    ExprPtr expr = comma();
    consume(SEMI_COLON, "Expected ';' after expression.");
    return StmtPtr(new Expression(std::move(expr)));
}

StmtPtr Parser::printStatement() {
    ExprPtr value = comma();

    consume(SEMI_COLON, "Expected ';' after value.");
    return StmtPtr(new Print(std::move(value)));
}

StmtPtr Parser::ifStatement() {
    consume(LEFT_PAREN, "Expected '(' after if.");
    ExprPtr condition = comma();
    consume(RIGHT_PAREN, "Expected ')' after if condition.");
    StmtPtr thenBlock = statement();

    StmtPtr elseBlock;
    /*
    ** In the case of nested if statements,
    ** the else clause belongs to the closest 'if' to it.
    */
    if(match({ELSE})) elseBlock = statement();

    return StmtPtr(new If(std::move(condition), std::move(thenBlock), std::move(elseBlock)));

}

StmtPtr Parser::whileStatement() {
    consume(LEFT_PAREN, "Expected '(' after while");
    ExprPtr condition = comma();
    consume(RIGHT_PAREN, "Expected ')' after condition");
    /** call to statement returns the body of the while loop */
    return StmtPtr(new While(std::move(condition), statement()));
}

StmtPtr Parser::forStatement() {
    consume(LEFT_PAREN, "Expected '(' after for");
    StmtPtr initializer;
    /* first we parse the intializer*/
    if(match({SEMI_COLON})) initializer = nullptr;
    else if(match({VAR})) initializer = varDeclaration();
    else initializer = expressionStatement();
    /** then the condition */
    ExprPtr condition = nullptr;
    if (!check(SEMI_COLON)) {
        condition = comma();
    }
    consume(SEMI_COLON, "Expect ';' after loop condition.");
    /** then the increment*/
    ExprPtr increment = nullptr;

    if(!check(RIGHT_PAREN)) {
        increment = comma();
    }
    consume(RIGHT_PAREN, "Expected ')' after 'for' clause");

    /** we now parse the body */
    StmtPtr body = statement();



    if(increment != nullptr) {
        std::vector<StmtPtr> v;
        v.push_back(std::move(body));
        v.push_back(StmtPtr(new Expression(std::move(increment))));
        body.reset(new Block(std::move(v)));
    }

    if(condition == nullptr) {
        condition.reset(new Literal(true));
    }

    body.reset(new While(std::move(condition), std::move(body)));


    if(initializer != nullptr) {
        std::vector<StmtPtr> v;
        v.push_back(std::move(initializer));
        v.push_back(std::move(body)); /* a while loop*/
        body.reset(new Block(std::move(v)));
    }

    return body;

}

ExprPtr Parser::comma()
{
    ExprPtr expr = expression();
    while(match({COMMA}))
    {
        Token oper = previous();
        ExprPtr right = expression();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }

    return expr;
}

ExprPtr Parser::logicOr() {
    ExprPtr expr = logicAnd();

    while(match({OR})) {
        Token oper = previous();
        ExprPtr right = logicAnd();
        expr = ExprPtr(new Logical(std::move(expr), oper, std::move(right)));
    }

    return expr;
}
ExprPtr Parser::logicAnd() {
    ExprPtr expr = equality();

    while(match({AND})) {
        Token oper = previous();
        ExprPtr right = equality();
        expr = ExprPtr(new Logical(std::move(expr), oper, std::move(right)));
    }
    return expr;
}

ExprPtr Parser::assignment() {
    ExprPtr expr = logicOr();

    if(match({EQUAL}))
    {
        Token equals = previous();
        /* We recursively call assignment to parse the RHS, because
        ** it is right associative
        */
        ExprPtr value = assignment();

        /* is expr an instance of Variable* ?*/
        if(dynamic_cast<Variable*>(expr.get()) != nullptr)
        {
            return ExprPtr(new Assign(dynamic_cast<Variable*>(expr.get())->name, std::move(value)));
        }

        Lox::error(equals, "Invalid assignment target.");
    }

    return expr;
}
ExprPtr Parser::expression() {
    return assignment();
}

ExprPtr Parser::equality() {
    ExprPtr expr = comparison();
    while(match({BANG_EQUAL, EQUAL_EQUAL}))
    {
        Token oper = previous();
        ExprPtr right = comparison();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }
    return expr;
}

ExprPtr Parser::comparison() {
    ExprPtr expr = addition();
    while(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL}))
    {
        Token oper = previous();
        ExprPtr right = addition();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }
    return expr;
}

ExprPtr Parser::addition() {
    ExprPtr expr = multiplication();
    while(match({MINUS, PLUS}))
    {
        Token oper = previous();
        ExprPtr right = multiplication();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }
    return expr;

}

ExprPtr Parser::multiplication() {
    ExprPtr expr = unary();
    while(match({SLASH, STAR}))
    {
        Token oper = previous();
        ExprPtr right = unary();
        expr = ExprPtr(new Binary(std::move(expr), oper, std::move(right)));
    }
    return expr;

}

ExprPtr Parser::unary() {
    if(match({BANG, MINUS}))
    {
        Token oper = previous();
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
        return ExprPtr(new Literal(previous().literal));
    }

    if (match({LEFT_PAREN})) {
        ExprPtr expr = expression();
        consume(RIGHT_PAREN, "Expected ')' after expression.\n");
        return ExprPtr(new Grouping(std::move(expr)));
    }

    if(match({VAR})) return ExprPtr(new Variable(previous()));


    if(match({IDENTIFIER})) return ExprPtr(new Variable(previous()));


    /* if none of the above cases is matched, we're on a token that
    ** cant start an expression
    */
    throw ParseError::error(peek(), "Expected expression.\n");
}

Token Parser::consume(TokenType type, const std::string& message) {
    if(check(type)) return advance();
    throw ParseError::error(peek(), message);
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
        if(previous().type == SEMI_COLON) return;
        /* if the next token is any of these, we're ready to resume parsing*/
        switch(peek().type)
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

std::vector<StmtPtr> Parser::parse()
{
    std::vector<StmtPtr> declarations;
    while(!isAtEnd())
    {
        declarations.push_back(declaration());
    }

    return declarations;

}
} // namespace lox