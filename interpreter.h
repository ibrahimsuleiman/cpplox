#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H

#include"expr.h"

namespace lox{
    class Interpreter : public ExprVisitor{
        public:
            Interpreter() = default;
            ~Interpreter() = default;
            virtual Object visitAssignExpr(Assign& expr)override;
            virtual Object visitBinaryExpr(Binary& expr)override;
            virtual Object visitCallExpr(Call& expr)override;
            virtual Object visitGetExpr(Get& expr)override;
            virtual Object visitGroupingExpr(Grouping& expr)override;
            virtual Object visitLiteralExpr(Literal& expr)override;
            virtual Object visitLogicalExpr(Logical& expr)override;
            virtual Object visitSetExpr(Set& expr)override;
            virtual Object visitSuperExpr(Super& expr)override;
            virtual Object visitThisExpr(This& expr)override;
            virtual Object visitUnaryExpr(Unary& expr)override;
            virtual Object visitVariableExpr(Variable& expr)override;

            Object evaluate(ExprPtr& expr);
            bool isTruthy(const Object& obj);
            bool isEqual(const Object& a, const Object& b);
            void checkNumberOperand(const Token& oper, const Object& operand);
            void checkNumberOperands(const Token& oper, const Object& left, const Object& right);
            /* The expression is already owned by an RAII object in our AST. Thus, we pass by reference*/
            void interpret(ExprPtr& expr);
            std::string stringify(Object& expr);

    };

    class RuntimeError : public std::exception{
        public:
            RuntimeError(const Token& token, const char *msg):token(token), msg(msg){}
            const char* what() const noexcept override
            {
                return "Runtime error";
            } 
            const char* message()const{
                return msg;
            }
            friend class Lox;
        private:
            Token token;
            const char* msg;
    };
} // namespace lox
#endif