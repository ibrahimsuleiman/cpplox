#ifndef LOX_EXPR_H
#define LOX_EXPR_H

#include<memory>
#include<vector>

#include"token.h"

namespace lox
{   
    
 class Expression{
        public:
            Token oper;
            std::unique_ptr<Expression> left;
            std::unique_ptr<Expression> right;
            virtual ~Expression() = 0; /* must implement pure virtual dtor*/
            
    };

    class Binary : public Expression{
        public:
            Binary(std::unique_ptr<Expression> left, Token oper, std::unique_ptr<Expression> right)
            {
                this->left = std::move(left);
                this->right = std::move(right);
                this->oper = oper;
            }
            


    };
    class Grouping : public Expression{
        public:
            std::unique_ptr<Expression> expr;
            Grouping(std::unique_ptr<Expression> expr)
            {
                expr = std::move(expr);
   
            }
    };
    class Unary : public Expression{
        public:
            Unary(const Token& oper, std::unique_ptr<Expression> right)
            {
                this->oper = oper;
                this->right = std::move(right);
    
            }


    };
    class Literal : public Expression{
        public:
            Object value;
            Literal(const Object& value)
            {
                this->value = value;
            }


    };
    class Assign : public Expression{
        public:
            Token name;
            std::unique_ptr<Expression> value;
            Assign(const Token& name, std::unique_ptr<Expression> value)
            {
                this->name = name;
                this->value = std::move(value);

            }
    };
    class Call : public Expression{
        public:
            Token paren;
            std::vector<std::unique_ptr<Expression> > args;
            std::unique_ptr<Expression> callee;
            Call(const Token& paren, std::unique_ptr<Expression> callee, std::vector<std::unique_ptr<Expression> > args)
            {
                this->paren = paren;
                this->callee = std::move(callee);
                this->args = args;

            }
    };

   
} // namespace lox

#endif