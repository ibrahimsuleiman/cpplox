#include"lox.h"
#include<memory>
#include<iostream>


int main(int argc, char** argv)
{
    if(argc < 2) {
        auto lox = std::make_unique<Lox>(Lox());
        lox->runPrompt();
    }
    else
    {
        auto lox = std::make_unique<Lox>(Lox(argv[1]));
        lox->runFile();
    }

    return 0;
}