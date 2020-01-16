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



class Expr {
public:
    Token oper;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    /*
    ** Because the accept() method's return type depends on the expression,
    ** we use an std::variant. My initial plan was to use templates, but runtime polymorphism and
    ** static polymorphism don't work well simultaneously
    **
    */
    virtual Object accept(ExprVisitor& visitor) = 0;
    virtual ~Expr() = default; /* must implement pure virtual dtor*/


};

/* pure virtual destructor definition*/
// Expr::~Expr() = default;

typedef std::unique_ptr<Expr> ExprPtr;
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
class Expr;

class ExprVisitor {
public:
    ExprVisitor() = default;
    virtual Object visitAssignExpr(Assign& expr) = 0;
    virtual Object visitBinaryExpr(Binary& expr) = 0;
    virtual Object visitCallExpr(Call& expr) = 0;
    virtual Object visitGetExpr(Get& expr) = 0;
    virtual Object visitGroupingExpr(Grouping& expr) = 0;
    virtual Object visitLiteralExpr(Literal& expr) = 0;
    virtual Object visitLogicalExpr(Logical& expr) = 0;
    virtual Object visitSetExpr(Set& expr) = 0;
    virtual Object visitSuperExpr(Super& expr) = 0;
    virtual Object visitThisExpr(This& expr) = 0;
    virtual Object visitUnaryExpr(Unary& expr) = 0;
    virtual Object visitVariableExpr(Variable& expr) = 0;
    virtual ~ExprVisitor() = default;
};

/*Expr classes defintion*/
/* These classes are non copyable because they derive from a base class with
** non copyable std::unique_ptr's
*/

class Assign : public Expr {
public:
    /*We expect an "l-value" here, so the left side of our AST is a Token*/
    Token name;
    ExprPtr value;
    Assign(const Token& name, ExprPtr value): name(name), value(std::move(value)) {}

    Object accept(ExprVisitor& visitor) {
        return visitor.visitAssignExpr(*this);
    }
};
class Binary : public Expr {
public:
    Binary(ExprPtr left, const Token& oper, ExprPtr right)
    {
        this->left = std::move(left);
        this->right = std::move(right);
        this->oper = oper;
    }
    Object accept(ExprVisitor& visitor) override {
        return visitor.visitBinaryExpr(*this);
    }
};

class Call : public Expr {
public:
    Token paren;
    std::vector<ExprPtr> args;
    ExprPtr callee;
    Call(const Token& paren, ExprPtr callee, std::vector<ExprPtr> args)
        : paren(paren), args(std::move(args)), callee(std::move(callee)) {}

    Object accept(ExprVisitor& visitor) override {
        return visitor.visitCallExpr(*this);
    }
};


class Get : public Expr {
public:
    ExprPtr object;
    Token name;
    Get(ExprPtr object, const Token& name)
        : object(std::move(object)), name(name) {}

    Object accept(ExprVisitor& visitor) override {
        return visitor.visitGetExpr(*this);
    }
};

class Grouping : public Expr {
public:
    ExprPtr expr;
    Grouping(ExprPtr expr):expr(std::move(expr)) {}

    Object accept(ExprVisitor& visitor) override {
        return visitor.visitGroupingExpr(*this);
    }
};

class Literal : public Expr {
public:
    Object value;
    Literal(const Object& value): value(value) {}

    Object accept(ExprVisitor& visitor) override {
        return visitor.visitLiteralExpr(*this);
    }
};

class Logical : public Expr {
public:
    Logical(ExprPtr left, const Token& oper, ExprPtr right)
    {
        this->left = std::move(left);
        this->right = std::move(right);
        this->oper = oper;
    }
    Object accept(ExprVisitor& visitor) override {
        return visitor.visitLogicalExpr(*this);
    }
};

class Set : public Expr {
public:
    ExprPtr object;
    Token name;
    ExprPtr value;

    Set(ExprPtr object, const Token& name, ExprPtr value)
        : object(std::move(object)), name(name), value(std::move(value)) {}

    Object accept(ExprVisitor& visitor) override {
        return visitor.visitSetExpr(*this);
    }

};

class Super : public Expr {
public:
    Token keyword;
    Token method;
    Super(const Token& keyword, const Token& method): keyword(keyword), method(method) {}

    Object accept(ExprVisitor& visitor) override {
        return visitor.visitSuperExpr(*this);
    }

};

class This : public Expr {
public:
    Token keyword;
    This(const Token& keyword): keyword(keyword) {}

    Object accept(ExprVisitor& visitor) override {
        return visitor.visitThisExpr(*this);
    }

};

class Unary : public Expr {
public:
    Unary(const Token& oper, ExprPtr right)
    {
        this->oper = oper;
        this->right = std::move(right);
    }
    Object accept(ExprVisitor& visitor) override {
        return visitor.visitUnaryExpr(*this);
    }

};

class Variable : public Expr {
public:

    Token name;
    Variable(const Token& name): name(name) {}

    Object accept(ExprVisitor& visitor) override {
        return visitor.visitVariableExpr(*this);
    }


};


/* class to print our ast*/
class AstNodePrinter : public ExprVisitor {
public:
    std::string parenthesize(const std::string& name, std::vector<Expr*>& exprs) {

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

    std::string print(Expr* expr) {

        auto val = expr->accept(*this);
        return std::holds_alternative<std::string>(val) ?
               std::get<std::string>(val) : std::string(" ");

    }
    /* place holder std::string("") returns for some of these. Might implement lateron*/
    virtual Object visitAssignExpr(Assign& expr) override {
        return std::string("");
    }
    virtual Object visitBinaryExpr(Binary& expr)override {
        std::vector<Expr*> v = {expr.left.get(), expr.right.get()};
        return parenthesize(expr.oper.lexeme, v);
    }
    virtual Object visitCallExpr(Call& expr)override {
        return std::string("");
    }
    virtual Object visitGetExpr(Get& expr)override {
        return std::string("");
    }
    virtual Object visitGroupingExpr(Grouping& expr)override {
        std::vector<Expr*> v = {expr.expr.get()};
        return parenthesize("group", v);
    }
    virtual Object visitLiteralExpr(Literal& expr)override {

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

    virtual Object visitLogicalExpr(Logical& expr)override {
        std::vector<Expr*> v = {expr.left.get(), expr.right.get()};
        return parenthesize(expr.oper.lexeme, v);
    }
    virtual Object visitSetExpr(Set& expr)override {
        return std::string("");
    }
    virtual Object visitSuperExpr(Super& expr)override {
        return std::string("");
    }
    virtual Object visitThisExpr(This& expr)override {
        return std::string("");
    }
    virtual Object visitUnaryExpr(Unary& expr)override {
        std::vector<Expr*> v = {expr.right.get()};
        return parenthesize(expr.oper.lexeme, v);
    }
    virtual Object visitVariableExpr(Variable& expr)override {
        return std::string("");
    }
};





} // namespace lox

#endif