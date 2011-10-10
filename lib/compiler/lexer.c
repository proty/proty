#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "error.h"

Lexer* Lexer_new(FILE* stream, const char* filename) {
    Lexer* lexer = malloc(sizeof(Lexer));

    lexer->stream = stream;
    lexer->filename = filename;
    lexer->lineno = 1;
    lexer->buflen = -1;
}

static void Lexer_add(Lexer* lexer, TokenType type,
                      const char* value, int precedence) {
    Token token;
    token.type = type;
    token.value = value;
    token.lineno = lexer->lineno;
    token.precedence = precedence;
}

/*
bool Lexer_isNext(Token::Type type) {
    return tokens->at(pos).getType() == type;
}

Token Lexer::match(TokenType to_match, const char* expected) {
    Token t = Lexer_next(lexer);
    if (t.getType() != to_match) {
        printf("unexcpected token");
    }
    return t;
}
*/

static char fpeek(FILE* stream) {
    char c = fgetc(stream);
    ungetc(c, stream);
    return c;
}

void Lexer_tokenize(Lexer* self) {
    char currch = fgetc(self->stream);
    char nextch = fpeek(self->stream);

    if (feof(lexer->stream)) {
        Lexer_add(self, Token_eof, "<eof>", 0);
    }

    while (1) {
        // whitespace
        if (isspace(currch)) {
            if (currch == '\n') {
                Lexer_add(self, Token_newline, "new line", 0);
                lexer->lineno++;
            }
            // ignore
        }

        // integer and decimal
        else if (isdigit(currch)) {
            char buffer[20];
            while(isdigit(fpeek(self->stream))) {
                strncat(buffer, fgetc(self->stream));
            }
            // decimal
            if (fpeek(self->stream) == '.') {
                while(isdigit(fpeek(self->stream))) {
                    buf << (char)stream->get();
                }
                Lexer_add(self, Token_decimal, buf.str());
            }
            else {
                Lexer_add(self, Token_integer, buf.str());
            }
        }

        // name and keywords
        else if (isalpha(currch)) {
            std::stringstream buf;
            buf << currch;
            while(isalpha(stream->peek())) {
                buf << (char)stream->get();
            }
            std::string value = buf.str();

            if (value == "do")         Lexer_add(self, Token_doKw, value, 0);
            else if (value == "if")    Lexer_add(self, Token_ifKw, value, 0);
            else if (value == "else")  Lexer_add(self, Token_elseKw, value, 0);
            else if (value == "while") Lexer_add(self, Token_whileKw, value, 0);
            else if (value == "end")   Lexer_add(self, Token_endKw, value, 0);
            else if (value == "try")   Lexer_add(self, Token_tryKw, value, 0);
            else if (value == "catch") Lexer_add(self, Token_catchKw, value, 0);
            else Lexer_add(self, Token_name, value, 0);
        }

        // string
        else if (currch == '"') {
            std::stringstream buf;
            while (stream->peek() != '"') {
                if (stream->peek() == '\\') {
                    stream->get();
                    switch (stream->get()) {
                    case 'a':
                        buf << '\a';
                        break;
                    case 'b':
                        buf << '\b';
                        break;
                    case 'n':
                        buf << '\n';
                        break;
                    case 'r':
                        buf << '\r';
                        break;
                    case 't':
                        buf << '\t';
                        break;
                    case '\\':
                        buf << '\\';
                        break;
                    case '"':
                        buf << '"';
                        break;
                    default:
                        buf << '\\';
                        buf << stream->unget();
                        buf << stream->get();
                    }
                }
                else {
                    buf << (char)stream->get();
                }
            }
            fgetc(self->stream);
            add(Token(Token::string, buf.str()));
        }

        else if (currch == '(') Lexer_add(self, Token_lpar, "(", 0);
        else if (currch == ')') Lexer_add(self, Token_rpar, ")", 0);
        else if (currch == '[') Lexer_add(self, Token_lsqb, "]", 0);
        else if (currch == ']') Lexer_add(self, Token_rsqb, "[", 0);
        else if (currch == '{') Lexer_add(self, Token_lbrace, "{", 0);
        else if (currch == '}') Lexer_add(self, Token_rbrace, "}", 0);
        else if (currch == ',') Lexer_add(self, Token_comma, ",", 0);

        // = and ==
        else if (currch == '=') {
            if (nextch == '=') {
                fgetc(stream);
                Lexer_add(self, Token_binaryop, "==", 20);
            }
            else if (nextch == '>') {
                fgetc(stream);
                Lexer_add(self, Token_arrow, "=>", 20);
            }
            else Lexer_add(self, Token_assign, "=", 0);
        }

        // > and >=
        else if (currch == '>') {
            if (nextch == '=') {
                fgetc(self->stream);
                Lexer_add(self, Token_binaryop, ">=", 20);
            }
            else Lexer_add(self, Token_binaryop, ">", 20);
        }

        // < and <=
        else if (currch == '<') {
            if (nextch == '=') {
                fgetc(self->stream);
                Lexer_add(self, Token_binaryop, "<=", 20);
            }
            else Lexer_add(self, Token_binaryop, "<", 20);
        }

        // != and !
        else if (currch == '!') {
            if (nextch == '=') {
                fgetc(self->stream);
                Lexer_add(self, Token_binaryop, "!=", 20);
            }
            else Lexer_add(self, Token_excl, "!", 0);
        }

        // &&
        else if (currch == '&' && nextch == '&') {
            fgetc(self->stream);
            Lexer_add(self, Token_binaryop, "&&", 5);
        }

        // ||
        else if (currch == '|' && nextch == '|') {
            fgetc(self->stream);
            Lexer_add(self, Token_binaryop, "||", 5);
        }

        else if (currch == '+') Lexer_add(self, Token_binaryop, "+", 10);
        else if (currch == '-') Lexer_add(self, Token_binaryop, "-", 10);
        else if (currch == '*') Lexer_add(self, Token_binaryop, "*", 15);
        else if (currch == '%') Lexer_add(self, Token_binaryop, "%", 15);

        else if (currch == '.') Lexer_add(self, Token_dot, ".", 0);
        else if (currch == ':') Lexer_add(self, Token_colon, ":", 0);
        else if (currch == ';') Lexer_add(self, Token_semicolon, ";", 0);

        // division and comments
        else if (currch == '/') {
            if (nextch == '/') {
                while (!feof(self->stream) && fgetc(self->stream) != '\n');
                Lexer_add(self, Token_newline, "new line", 0);
                self->lineno++;
            }
            else if (nextch == '*') {
                fgetc(self->stream);
                while (!feof(self->stream) && !(fgetc(self->stream) == '*'
                                                && fgetc(self->stream) == '/')) {
                    if (fpeek(self->stream) == '\n') self->lineno++;
                }
                fgetc(self->stream);
            }
            else {
                Lexer_add(self, Token_binaryop, "/", 15);
            }
        }

        else if (currch == '#') {
            // ignore shebang
            if (nextch == '!') {
                while (!feof(self->stream) && fgetc(self->stream) != '\n');
                Lexer_add(self, Token_newline, "new line", 0);
                self->lineno++;
            }
            else {
                Lexer_add(self, Token_hash, "#", 0);
            }
        }

        else {
            // ignore eof
            if (currch != -1) {
                Lexer_add(self, Token_unknown, "" + currch, 0);
            }
        }

        if (feof(self->stream)) {
            Lexer_add(self, Token_eof, "<eof>", 0);
            break;
        }

        currch = fgetc(self->stream);
        nextch = fpeek(self->stream);
    }
}
