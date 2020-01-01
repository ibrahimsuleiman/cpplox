#include"scanner.h"
#include"lox.h"

namespace lox
{

Scanner::Scanner(const std::string& source): source(source),  start(0), current(0), line(1)
{

}

void Scanner::addToken(TokenType type, Object literal)
{
    std::string text(&source[start], &source[current]);
    tokens.push_back(std::shared_ptr<Token>(new Token(type, text, literal, line)));
}

std::vector<std::shared_ptr<Token> > Scanner::scanTokens()
{
    while(!isAtEnd())
    {
        start = current;
        scan();
    }

    tokens.push_back(std::shared_ptr<Token>(new Token(END_OF_FILE, "__null__", "EOF", line)));
    return tokens;
}

bool Scanner::match(const char expected)
{
    if(isAtEnd()) return false;
    if(source[current] != expected) return false;

    current++;
    return true;
}

void Scanner::matchString()
{
    while(peek() != '"' && !isAtEnd())
    {
        if(peek() == '\n') line++;
        advance();
    }

    if(isAtEnd())
    {
        Lox::error(line, "Unterminated string.");
        return;
    }
    /* consume closing "*/
    advance();
    std::string value(&source[start + 1], &source[current - 1]);
    addToken(STRING, value);
}

void Scanner::matchNumber()
{
    while(isDigit(peek())) advance();
    /* fractional part*/
    if(peek() == '.' && isDigit(peekNext()))
    {
        advance();
        while(isDigit(peek())) advance();
    }
    std::string num(&source[start], &source[current]);
    addToken(NUMBER, std::stod(num));
}

void Scanner::matchIdentifier()
{
    while(isalnum(peek()) || peek() == '_') advance();

    /*is it a keyword? */
    std::string text (&source[start], &source[current]);
    TokenType t;
    if(keywords.find(text) == keywords.end()) t = IDENTIFIER;
    else t = keywords[text];

    addToken(t);
}
void Scanner::scan()
{
    char c = advance();
    switch(c) {
    case '(':
        addToken(LEFT_PAREN);
        break;
    case ')':
        addToken(RIGHT_PAREN);
        break;
    case '{':
        addToken(RIGHT_BRACE);
        break;
    case '}':
        addToken(RIGHT_BRACE);
        break;
    case ',':
        addToken(COMMA);
        break;
    case '.':
        addToken(DOT);
        break;
    case '-':
        addToken(MINUS);
        break;
    case '+':
        addToken(PLUS);
        break;
    case ';':
        addToken(SEMI_COLON);
        break;
    case '*':
        addToken(STAR);
        break;
    /*We need to look at second character for these tokens*/
    case '!':
        addToken(match('=') ? BANG_EQUAL : BANG) ;
        break;
    case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
    case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
    case '>':
        addToken(match('=') ? GREATER_EQUAL: GREATER);
        break;
    /*ignore white space*/
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line++;
        break;
    case '"':
        matchString();
        break;
    default:
        if(isDigit(c))
        {
            matchNumber();
        }
        else if(isalpha(c) || c == '_')
        {
            matchIdentifier();
        }
        else
        {
            Lox::error(line, "Unexpected character.");
        }


        break;

    }

}
} // namespace lox

