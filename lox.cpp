#include"lox.h"
#include"scanner.h"



namespace lox
{
    bool Lox::hadError = false;

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

    for(auto t = tokens.begin(); t != tokens.end(); t++)
    {
        std::cout << (*t)->toString() << std::endl;
    }
}

void Lox::report(int line, const std::string& where, const std::string& message)
{
    std::cout << "[line " << line << "] Error" << where << ": "
              << message;
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

    run(buf);
}
}// namespace lox
