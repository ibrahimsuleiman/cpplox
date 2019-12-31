#include<iostream>
#include<memory>

#include"lox.h"


int main(int argc, char** argv)
{
    if(argc < 2) {
        auto lox = std::make_unique<lox::Lox>();
        lox->runPrompt();
    }
    else
    {
        auto lox = std::make_unique<lox::Lox>(argv[1]);
        lox->runFile();
    }

    return 0;
}