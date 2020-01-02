#ifndef LOX_EXPR_H
#define LOX_EXPR_H

#include<iostream>
#include<memory>
#include<vector>
#include<variant>

#include"token.h"

/*
 ** This file is organized into 4 parts. We begin with a definition of 
 ** the abstract interface for all AST nodes representing an expression, followed by a foward declaraction of 
 ** the concrete expression classes. We then define an abstract visitor class to extend functionality of the
 ** expression classes while keeping them small. Following that are the definitions of the 
 ** expression classes. Lastly, we provide an AST printer, a concrete implementation of the ExprVisitor class 
 */

namespace lox
{   


 class ExprVisitor;
 typedef std::variant<double, std::string, Object> Result;

 class Expression{
        public:
            Token oper;
            std::unique_ptr<Expression> left;
            std::unique_ptr<Expression> right;
            /*
            ** Because the accept() method's return type depends on the expression, 
            ** we use an std::variant. My initial plan was to use templates, but runtime polymorphism and 
            ** static polymorphism don't work well simultaneously
            **
            */
            typedef std::variant<double, std::string, Object> Result;

            virtual Result accept(ExprVisitor& visitor) = 0;
            virtual ~Expression() = default; /* must implement pure virtual dtor*/
            
            
    };

    /* pure virtual destructor definition*/
   // Expression::~Expression() = default;

    typedef std::unique_ptr<Expression> ExprPtr;  
    /*foward declarations*/
    class Assign;
    class Binary;
    class Call;
    class Get;
    class Grouping;
    class Literal;
    class Logical;
    class Set;
    class Super;
    class This;
    class Unary;
    class Variable;
    class Expression;

 class ExprVisitor{
        public:
            ExprVisitor() = default;
            virtual Result visitAssignExpr(Assign& expr) = 0;
            virtual Result visitBinaryExpr(Binary& expr) = 0;
            virtual Result visitCallExpr(Call& expr) = 0;
            virtual Result visitGetExpr(Get& expr) = 0;
            virtual Result visitGroupingExpr(Grouping& expr) = 0;
            virtual Result visitLiteralExpr(Literal& expr) = 0;
            virtual Result visitLogicalExpr(Logical& expr) = 0;
            virtual Result visitSetExpr(Set& expr) = 0;
            virtual Result visitSuperExpr(Super& expr) = 0;
            virtual Result visitThisExpr(This& expr) = 0;
            virtual Result visitUnaryExpr(Unary& expr) = 0;
            virtual Result visitVariableExpr(Variable& expr) = 0;
            virtual ~ExprVisitor() = default;
    };

    /*Expression classes defintion*/
    class Binary : public Expression {
        public:
            Binary(ExprPtr left, const Token& oper, ExprPtr right)
            {
                this->left = std::move(left);
                this->right = std::move(right);
                this->oper = oper;
            }
            Result accept(ExprVisitor& visitor) override{
                return visitor.visitBinaryExpr(*this);
            }
    };

    class Call : public Expression {
        public:
            Token paren;
            std::vector<ExprPtr> args;
            ExprPtr callee;
            Call(const Token& paren, ExprPtr callee, std::vector<ExprPtr> args)
            : paren(paren), args(std::move(args)), callee(std::move(callee)) {}

            Result accept(ExprVisitor& visitor) override{
                return visitor.visitCallExpr(*this);
            }
    };


    class Get : public Expression {
        public:
            ExprPtr object;
            Token name;
            Get(ExprPtr object, const Token& name)
            : object(std::move(object)), name(name) {}

            Result accept(ExprVisitor& visitor) override{
                return visitor.visitGetExpr(*this);
            }
    };

    class Grouping : public Expression {
        public:
            ExprPtr expr;
            Grouping(ExprPtr expr):expr(std::move(expr)){}

            Result accept(ExprVisitor& visitor) override{
                return visitor.visitGroupingExpr(*this);
            }    
    };

    class Literal : public Expression {
        public:
            Object value;
            Literal(const Object& value): value(value){}

            Result accept(ExprVisitor& visitor) override{
                return visitor.visitLiteralExpr(*this);
            }
    };

    class Logical : public Expression {
        public:
            Logical(ExprPtr left, const Token& oper, ExprPtr right)
            {
                this->left = std::move(left);
                this->right = std::move(right);
                this->oper = oper;
            }
            Result accept(ExprVisitor& visitor) override{
                return visitor.visitLogicalExpr(*this);
            }
    };

    class Set : public Expression {
        public:
            ExprPtr object;
            Token name;
            ExprPtr value;

            Set(ExprPtr object, const Token& name, ExprPtr value)
            : object(std::move(object)), name(name), value(std::move(value)) {}

            Result accept(ExprVisitor& visitor) override{
                return visitor.visitSetExpr(*this);
            }

    };

    class Super : public Expression {
        public:
            Token keyword;
            Token method;
            Super(const Token& keyword, const Token& method): keyword(keyword), method(method) {}

            Result accept(ExprVisitor& visitor) override{
                return visitor.visitSuperExpr(*this);
            }

    };

    class This : public Expression {
        public:
            Token keyword;
            This(const Token& keyword): keyword(keyword){}

            Result accept(ExprVisitor& visitor) override{
                return visitor.visitThisExpr(*this);
            }

    };

    class Unary : public Expression {
        public:
            Unary(const Token& oper, ExprPtr right)
            {
                this->oper = oper;
                this->right = std::move(right);
            }
            Result accept(ExprVisitor& visitor) override{
                return visitor.visitUnaryExpr(*this);
            }

    };

    class Variable : public Expression {
        public:
            Token name;
            Variable(const Token& name): name(name){}

            Result accept(ExprVisitor& visitor) override{
                return visitor.visitVariableExpr(*this);
            }


    };


   /* class to print our ast*/
  class AstNodePrinter : public ExprVisitor{
        public:
            std::string parenthesize(const std::string& name, std::vector<Expression*>& exprs){

                std::string AstString("(" + name);
                 
                for(auto expr : exprs)
                {
                    AstString += " ";
                    auto val = expr->accept(*this);

                    if(std::holds_alternative<std::string>(val))
                        AstString += std::get<std::string>(val);
                    else if(std::holds_alternative<double>(val))
                        AstString += std::to_string(std::get<double>(val));
                    else
                        AstString += " ";

                }

                AstString += ")";

                
                return AstString;         
            }

            std::string print(Expression* expr){

                auto val = expr->accept(*this);
                return std::holds_alternative<std::string>(val) ?
                std::get<std::string>(val) : std::string(" ");
    
            }
            /* place holder std::string("") returns for some of these. Might implement lateron*/
            virtual Result visitAssignExpr(Assign& expr) override{
                return std::string("");                
            }
            virtual Result visitBinaryExpr(Binary& expr)override{
                std::vector<Expression*> v = {expr.left.get(), expr.right.get()};
                return parenthesize(expr.oper.lexeme, v);
            }
            virtual Result visitCallExpr(Call& expr)override{
                return std::string("");
            }
            virtual Result visitGetExpr(Get& expr)override{
                return std::string("");
            }
            virtual Result visitGroupingExpr(Grouping& expr)override{
                std::vector<Expression*> v = {expr.expr.get()};
                return parenthesize("group", v);
            }
            virtual Result visitLiteralExpr(Literal& expr)override{

                if(std::holds_alternative<std::string>(expr.value))
                {
                    if(std::get<std::string>(expr.value) == "__null__" )
                        return std::string("nil");
                    return std::get<std::string>(expr.value);
                }
                    
                else if(std::holds_alternative<double>(expr.value))
                    return std::to_string(std::get<double>(expr.value));
                else 
                    return std::string(" ");
                
            }
            
            virtual Result visitLogicalExpr(Logical& expr)override{
                return std::string("");
            }
            virtual Result visitSetExpr(Set& expr)override{
                return std::string("");
            }
            virtual Result visitSuperExpr(Super& expr)override{
                return std::string("");
            }
            virtual Result visitThisExpr(This& expr)override{
                return std::string("");
            }
            virtual Result visitUnaryExpr(Unary& expr)override{
                std::vector<Expression*> v = {expr.right.get()};
                return parenthesize(expr.oper.lexeme, v);
            }
            virtual Result visitVariableExpr(Variable& expr)override{
                return std::string("");
            }
    };

  
    


} // namespace lox

#endif