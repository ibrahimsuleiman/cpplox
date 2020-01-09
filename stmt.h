#ifndef LOX_STMT_H
#define LOX_STMT_H


#include<memory>
#include<vector>

#include"expr.h"
#include"token.h"

namespace lox{

    class StmtVisitor;

    class Stmt{
        public:
            virtual void accept(StmtVisitor& visitor) = 0;
            virtual ~Stmt() = default;

    };



    typedef std::unique_ptr<Stmt> StmtPtr;

    class Block;
    class Class;
    class Expression;
    class Function;
    class If;
    class Print;
    class Return;
    class Var;
    class While;
    
    typedef std::unique_ptr<Function> FunPtr;

    class StmtVisitor{
        public:
            virtual void visitBlockStmt(Block& stmt) = 0;
            virtual void visitClassStmt(Class& stmt) = 0;
            virtual void visitExpressionStmt(Expression& stmt) = 0;
            virtual void visitFunctionStmt(Function& stmt) = 0;
            virtual void visitIfStmt(If& stmt) = 0;
            virtual void visitPrintStmt(Print& stmt) = 0;
            virtual void visitReturnStmt(Return& stmt) = 0;
            virtual void visitVarStmt(Var& stmt) = 0;
            virtual void visitWhileStmt(While& stmt) = 0;
            virtual ~StmtVisitor() = default;
            
    };

    class Block : public Stmt{
        public:
            std::vector<StmtPtr> statements;
            Block(std::vector<StmtPtr> statements):statements(std::move(statements)){}
            void accept(StmtVisitor& visitor)override{
                visitor.visitBlockStmt(*this);
            }
    };
    class Class : public Stmt{
        public:
            Token name;
            std::unique_ptr<Variable> superclass;
            std::vector<FunPtr> methods;
            Class(const Token& name, std::unique_ptr<Variable>& superclass, std::vector<FunPtr>& methods):
            name(name), superclass(std::move(superclass)), methods(std::move(methods)){}

            void accept(StmtVisitor& visitor)override{
                visitor.visitClassStmt(*this);
            }

    };
    class Expression : public Stmt{
        public:
            ExprPtr expression;
            Expression(ExprPtr expression): expression(std::move(expression)){}
            void accept(StmtVisitor& visitor)override{
                visitor.visitExpressionStmt(*this);
            }
    };
    class Function : public Stmt{
        public:
            Token name;
            std::vector<Token> params;
            std::vector<StmtPtr> body;
            Function(const Token& name, const std::vector<Token>& params, std::vector<StmtPtr>& body)
            :name(name), params(params), body(std::move(body)){}

            void accept(StmtVisitor& visitor)override{
                visitor.visitFunctionStmt(*this);
            }
    };
    class If : public Stmt{
        public:
            ExprPtr condition;
            StmtPtr thenBranch;
            StmtPtr elseBranch;
            If(ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch)
            :condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(thenBranch)){}

            void accept(StmtVisitor& visitor)override{
                visitor.visitIfStmt(*this);
            }
    };
    class Print : public Stmt{
        public:
            ExprPtr expression;
            Print(ExprPtr expression):expression(std::move(expression)){}
            
            void accept(StmtVisitor& visitor)override{
                visitor.visitPrintStmt(*this);
            }
    };
    class Return : public Stmt{
        public:
            Token keyword;
            ExprPtr value;
            Return(const Token& keyword, ExprPtr value): keyword(keyword), value(std::move(value)){}

            void accept(StmtVisitor& visitor)override{
                visitor.visitReturnStmt(*this);
            }
    };
    class Var : public Stmt{
        public:
            Token name;
            ExprPtr initializer;
            Var(const Token& name, ExprPtr initializer): name(name), initializer(std::move(initializer)){}
            
            void accept(StmtVisitor& visitor)override{
                visitor.visitVarStmt(*this);
            }
        
    };
    class While : public Stmt{
        public:
            ExprPtr condition;
            StmtPtr body;
            While(ExprPtr condition, StmtPtr body): condition(std::move(condition)), body(std::move(body)){}

            void accept(StmtVisitor& visitor)override{
                visitor.visitWhileStmt(*this);
            }
    };
    

}// namespace lox
#endif