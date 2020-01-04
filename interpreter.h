#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H

#include"expr.h"
#include"stmt.h"

namespace lox{
    class Interpreter : public ExprVisitor, StmtVisitor{
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

            virtual void visitBlockStmt(Block& stmt)override;
            virtual void visitClassStmt(Class& stmt)override;
            virtual void visitExpressionStmt(Expression& stmt)override;
            virtual void visitFunctionStmt(Function& stmt)override;
            virtual void visitIfStmt(If& stmt)override;
            virtual void visitPrintStmt(Print& stmt)override;
            virtual void visitReturnStmt(Return& stmt)override;
            virtual void visitVarStmt(Var& stmt)override;
            virtual void visitWhileStmt(While& stmt)override;

            void execute(StmtPtr& expr);
            Object evaluate(ExprPtr& expr);
            bool isTruthy(const Object& obj);
            bool isEqual(const Object& a, const Object& b);
            void checkNumberOperand(const Token& oper, const Object& operand);
            void checkNumberOperands(const Token& oper, const Object& left, const Object& right);
            void interpret(std::vector<StmtPtr>& expr);
            std::string stringify(const Object& expr);

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