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
/* allows lox to continue to accept input after a comment in interactive mode*/
    if(tokens[0]->type == END_OF_FILE) return;
    
    auto parser = std::make_unique<Parser>(tokens);

    /* We rely on copy elision to move construct statements from the non-copyable type
    ** returned by parse()
    */
    auto statements = parser->parse();

    if(hadError) return;
 
    //std::cout << AstNodePrinter().print(expression.get()) << std::endl;
    static std::unique_ptr<Interpreter> interpreter = std::make_unique<Interpreter>();
    interpreter->interpret(statements);   
}


void Lox::error(const Token& token, const std::string& msg)
{
    if(token.type == END_OF_FILE) report(token.line, " at end", msg);
    else report(token.line," at '" + token.lexeme + "'" , msg);
}

void Lox::report(int line, const std::string& where, const std::string& message)
{
    std::cout << "[line " << line << "] Error" << where << ": "
              << message << std::endl;
    hadError = true;
}

void Lox::runFile()
{
    std::string buf;
    std::string line;
    std::ifstream inputStream(source);

    while (getline(inputStream, line))
    {
        buf += line;
    }

    if(hadError) std::exit(-1);
    if(hadRuntimeError) std::exit(-2);

    run(buf);
}
}// namespace lox
