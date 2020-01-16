#include<sstream>

#include"environment.h"
#include"lox.h"
#include"scanner.h"
#include"parser.h"


namespace lox
{
bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;

void Lox::runPrompt()
{
    for(;;)
    {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);
        run(input);
        hadError = false;
    }
}


void Lox::run(const std::string& buf)
{

    if(hadError) exit(-1);
    auto scanner = std::unique_ptr<Scanner>(new Scanner(buf));

    auto tokens = scanner->scanTokens();

    auto parser = std::make_unique<Parser>(std::move(tokens));

    auto statements = parser->parse();

    if(hadError) return;

    static std::unique_ptr<Interpreter> interpreter = std::make_unique<Interpreter>();
    interpreter->interpret(statements);

}


void Lox::error(const Token& token, const std::string& msg)
{
    if(token.type == END_OF_FILE) report(token.line, " at end", msg);
    else report(token.line," at '" + token.lexeme + "'", msg);
}

void Lox::report(int line, const std::string& where, const std::string& message)
{
    std::cout << "[line " << line << "] Error" << where << ": "
              << message << std::endl;
    hadError = true;
}


void Lox::runFile()
{
    std::ostringstream buf;
    std::ifstream inputStream(source);

    buf << inputStream.rdbuf();

    if(hadError) std::exit(-1);
    if(hadRuntimeError) std::exit(-2);

    run(buf.str());
}
}// namespace lox
