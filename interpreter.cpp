#include"interpreter.h"
#include"lox.h"

namespace lox{

Object Interpreter::evaluate(ExprPtr& expr){
    return expr->accept(*this);
}

bool  Interpreter::isTruthy(const Object& obj){
    /*everything else but false and nil is truthy in Lox*/
    if(std::holds_alternative<void*>(obj))
        return false;
    if(std::holds_alternative<bool>(obj))
        return std::get<bool>(obj);
    return true;
}

bool Interpreter::isEqual(const Object& a, const Object& b){
    /* nil is only equal to nil*/
    if(std::holds_alternative<void*>(a)
      && std::holds_alternative<void*>(b))
      return true;
    if(std::holds_alternative<void*>(a)) return false;

    return a == b;
}

void Interpreter::checkNumberOperand(const Token& oper, const Object& operand){
    if(std::holds_alternative<double>(operand)) return;
    throw RuntimeError(oper, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(const Token& oper, const Object& left, const Object& right){
    if(std::holds_alternative<double>(left)
    && std::holds_alternative<double>(right)) return;

    throw RuntimeError(oper, "Operand must be a number.");
}

Object Interpreter::visitAssignExpr(Assign& expr){
    return nullptr;
}
Object Interpreter::visitBinaryExpr(Binary& expr){
    Object left = evaluate(expr.left);
    Object right = evaluate(expr.right);

    switch(expr.oper.type){
        case PLUS:
            if(std::holds_alternative<double>(right)
            && std::holds_alternative<double>(left)){

                return std::get<double>(left) + std::get<double>(right);
            }

            if(std::holds_alternative<std::string>(right)
            && std::holds_alternative<std::string>(left)){
                return std::get<std::string>(left) + std::get<std::string>(right);
            }
            throw RuntimeError(expr.oper,"Operands must be two numbers or two strings.");

        case MINUS:
            checkNumberOperands(expr.oper, left, right);
            return std::get<double>(left) - std::get<double>(right);
        case SLASH:
            checkNumberOperands(expr.oper, left, right);
            return std::get<double>(left) / std::get<double>(right);
        case STAR:
            checkNumberOperands(expr.oper, left, right);
            return std::get<double>(left) * std::get<double>(right);
        case GREATER:
            checkNumberOperands(expr.oper, left, right);
            return std::get<double>(left) > std::get<double>(right);
        case GREATER_EQUAL:
            checkNumberOperands(expr.oper, left, right);
            return std::get<double>(left) >= std::get<double>(right);
        case LESS:
            checkNumberOperands(expr.oper, left, right);
            return std::get<double>(left) < std::get<double>(right);
        case LESS_EQUAL:
            checkNumberOperands(expr.oper, left, right);
            return std::get<double>(left) <= std::get<double>(right);
        case COMMA:
            return right;

        default:
            break;
    }

    return nullptr;
}
/*** place holders nullptr**/
Object Interpreter::visitCallExpr(Call& expr){
    return nullptr;
}
Object Interpreter::visitGetExpr(Get& expr){
    return nullptr;
}
Object Interpreter::visitGroupingExpr(Grouping& expr){
    return evaluate(expr.expr);
}
Object Interpreter::visitLiteralExpr(Literal& expr){
    return expr.value;
}
Object Interpreter::visitLogicalExpr(Logical& expr){
    return nullptr;
}
Object Interpreter::visitSetExpr(Set& expr){
    return nullptr;
}
Object Interpreter::visitSuperExpr(Super& expr){
    return nullptr;
}
Object Interpreter::visitThisExpr(This& expr){
    return nullptr;
}
Object Interpreter::visitUnaryExpr(Unary& expr){
    Object right = evaluate(expr.right);
    switch (expr.oper.type)
    {
    case MINUS:
        checkNumberOperand(expr.oper, right);
        return -std::get<double>(right);
        break;
    case BANG:
        return !isTruthy(right);
    default:
        break;
    }

    return nullptr;
}
Object Interpreter::visitVariableExpr(Variable& expr){
    return nullptr;
}

std::string Interpreter::stringify(const Object& obj)
{
    if(std::holds_alternative<void*>(obj)) return std::string("nil");

    if(std::holds_alternative<double>(obj)){
        double v = std::get<double>(obj);
        if(v == static_cast<int>(v))
            return std::string(std::to_string(static_cast<int>(v)));
        else return std::string(std::to_string(v));

        
    }
    if(std::holds_alternative<bool>(obj))
        return std::get<bool>(obj) ? std::string("true") : std::string("false");
    
    return std::get<std::string>(obj);
}

void Interpreter::visitBlockStmt(Block& stmt){

}
void Interpreter::visitClassStmt(Class& stmt){

}
void Interpreter::visitExpressionStmt(Expression& stmt){
    evaluate(stmt.expression);
}
void Interpreter::visitFunctionStmt(Function& stmt){

}
void Interpreter::visitIfStmt(If& stmt){

}
void Interpreter::visitPrintStmt(Print& stmt){
    std::cout << stringify(evaluate(stmt.expression)) << std::endl;
}
void Interpreter::visitReturnStmt(Return& stmt){

}
void Interpreter::visitVarStmt(Var& stmt){

}
void Interpreter::visitWhileStmt(While& stmt){
    
}

void Interpreter::execute(StmtPtr& statement)
{
    statement->accept(*this);
}
void Interpreter::interpret(std::vector<StmtPtr>& statements){
    try{

        for(auto it = statements.begin(); it != statements.end(); ++it)
        {
            execute(*it);
        }

    }
    catch(const RuntimeError& err)
    {
        Lox::runtimeError(err);
    }
}

} // namespace lox