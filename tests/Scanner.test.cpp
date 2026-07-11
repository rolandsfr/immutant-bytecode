#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "Scanner.hpp"

namespace {

std::vector<Token> scanAll(const char* source) {
    Scanner scanner;
    scanner.init(source);
    std::vector<Token> tokens;
    for (;;) {
        Token token = scanner.scanToken();
        tokens.push_back(token);
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERR) {
            break;
        }
    }
    return tokens;
}

std::string lexeme(const Token& token) {
    return std::string(token.start, static_cast<size_t>(token.len));
}

#define EXPECT_TOKEN_TYPE(tok, expected_type) EXPECT_EQ((tok).type, (expected_type))
#define EXPECT_TOKEN_LEXEME(tok, expected_text) EXPECT_EQ(lexeme(tok), (expected_text))
#define EXPECT_TOKEN_LINE(tok, expected_line) EXPECT_EQ((tok).line, (expected_line))

#define EXPECT_TOKEN(tok, expected_type, expected_text) \
    do { \
        EXPECT_TOKEN_TYPE(tok, expected_type); \
        EXPECT_TOKEN_LEXEME(tok, expected_text); \
    } while (0)

#define EXPECT_SCAN_SIZE(all_tokens, count) ASSERT_EQ((all_tokens).size(), (count))

#define EXPECT_SCAN_EOF(all_tokens, count) \
    do { \
        EXPECT_SCAN_SIZE(all_tokens, count); \
        EXPECT_TOKEN_TYPE((all_tokens).back(), TOKEN_EOF); \
    } while (0)

#define EXPECT_SCAN_ERR(all_tokens, message) \
    do { \
        EXPECT_SCAN_SIZE(all_tokens, 1u); \
        EXPECT_TOKEN((all_tokens)[0], TOKEN_ERR, message); \
    } while (0)

}  // namespace

TEST(ScannerTest, InitSetsLineToOne) {
    Scanner scanner;
    scanner.init("");

    EXPECT_EQ(scanner.line, 1);
    EXPECT_STREQ(scanner.start, "");
    EXPECT_STREQ(scanner.current, "");
}

TEST(ScannerTest, EmptySourceReturnsEof) {
    auto tokens = scanAll("");

    EXPECT_SCAN_EOF(tokens, 1u);
    EXPECT_TOKEN_LINE(tokens[0], 1);
}

TEST(ScannerTest, SingleCharacterTokens) {
    auto tokens = scanAll("(){}+-*%;,.");

    EXPECT_SCAN_EOF(tokens, 12u);
    EXPECT_TOKEN_TYPE(tokens[0], TOKEN_LEFT_PAREN);
    EXPECT_TOKEN_TYPE(tokens[1], TOKEN_RIGHT_PAREN);
    EXPECT_TOKEN_TYPE(tokens[2], TOKEN_LEFT_BRACE);
    EXPECT_TOKEN_TYPE(tokens[3], TOKEN_RIGHT_BRACE);
    EXPECT_TOKEN_TYPE(tokens[4], TOKEN_PLUS);
    EXPECT_TOKEN_TYPE(tokens[5], TOKEN_MINUS);
    EXPECT_TOKEN_TYPE(tokens[6], TOKEN_STAR);
    EXPECT_TOKEN_TYPE(tokens[7], TOKEN_MODULO);
    EXPECT_TOKEN_TYPE(tokens[8], TOKEN_SEMICOLON);
    EXPECT_TOKEN_TYPE(tokens[9], TOKEN_COMMA);
    EXPECT_TOKEN_TYPE(tokens[10], TOKEN_DOT);
}

TEST(ScannerTest, TwoCharacterComparisonTokens) {
    auto tokens = scanAll("!= == <= >= ! = < >");

    EXPECT_SCAN_EOF(tokens, 9u);
    EXPECT_TOKEN(tokens[0], TOKEN_BANG_EQUAL, "!=");
    EXPECT_TOKEN(tokens[1], TOKEN_EQUAL_EQUAL, "==");
    EXPECT_TOKEN(tokens[2], TOKEN_LESS_EQUAL, "<=");
    EXPECT_TOKEN(tokens[3], TOKEN_GREATER_EQUAL, ">=");
    EXPECT_TOKEN(tokens[4], TOKEN_BANG, "!");
    EXPECT_TOKEN(tokens[5], TOKEN_EQUAL, "=");
    EXPECT_TOKEN(tokens[6], TOKEN_LESS, "<");
    EXPECT_TOKEN(tokens[7], TOKEN_GREATER, ">");
}

TEST(ScannerTest, SlashTokenIsNotTreatedAsWhitespace) {
    auto tokens = scanAll("/");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_SLASH, "/");
}

TEST(ScannerTest, SkipsWhitespace) {
    auto tokens = scanAll(" \t\r+");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN_TYPE(tokens[0], TOKEN_PLUS);
}

TEST(ScannerTest, NewlineIncrementsLine) {
    Scanner scanner;
    scanner.init("+\n-");

    Token first = scanner.scanToken();
    Token second = scanner.scanToken();

    EXPECT_TOKEN_TYPE(first, TOKEN_PLUS);
    EXPECT_TOKEN_LINE(first, 1);
    EXPECT_TOKEN_TYPE(second, TOKEN_MINUS);
    EXPECT_TOKEN_LINE(second, 2);
}

TEST(ScannerTest, SkipsLineComments) {
    auto tokens = scanAll("// comment\n+");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN_TYPE(tokens[0], TOKEN_PLUS);
    EXPECT_TOKEN_LINE(tokens[0], 2);
}

TEST(ScannerTest, EmptyStringLiteral) {
    auto tokens = scanAll("\"\"");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_STRING, "\"\"");
}

TEST(ScannerTest, UnterminatedStringReportsError) {
    EXPECT_SCAN_ERR(scanAll("\"unterminated"), "Unterminated string");
}

TEST(ScannerTest, IntegerNumberLiteral) {
    auto tokens = scanAll("42");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_NUMBER, "42");
}

TEST(ScannerTest, DecimalNumberLiteral) {
    auto tokens = scanAll("3.14");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_NUMBER, "3.14");
}

TEST(ScannerTest, LeadingDotNumberLiteral) {
    auto tokens = scanAll(".5");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_NUMBER, ".5");
}

TEST(ScannerTest, UnexpectedCharacterReportsError) {
    EXPECT_SCAN_ERR(scanAll("@"), "Unexpected character.");
}

TEST(ScannerTest, ElseKeyword) {
    auto tokens = scanAll("else");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_ELSE, "else");
}

TEST(ScannerTest, MutKeyword) {
    auto tokens = scanAll("mut");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_MUTANT, "mut");
}

TEST(ScannerTest, NilKeyword) {
    auto tokens = scanAll("nil");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_NIL, "nil");
}

TEST(ScannerTest, PureKeyword) {
    auto tokens = scanAll("pure");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_PURE, "pure");
}

TEST(ScannerTest, ReturnKeyword) {
    auto tokens = scanAll("return");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_RETURN, "return");
}

TEST(ScannerTest, TrueKeyword) {
    auto tokens = scanAll("true");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_TRUE, "true");
}

TEST(ScannerTest, WhileKeyword) {
    auto tokens = scanAll("while");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_WHILE, "while");
}

TEST(ScannerTest, FnKeyword) {
    auto tokens = scanAll("fn");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_FN, "fn");
}

TEST(ScannerTest, FalseKeyword) {
    auto tokens = scanAll("false");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_FALSE, "false");
}

TEST(ScannerTest, IfKeyword) {
    auto tokens = scanAll("if");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_IF, "if");
}

TEST(ScannerTest, ImutKeyword) {
    auto tokens = scanAll("imut");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_IMMUTANT, "imut");
}

TEST(ScannerTest, ImpureKeyword) {
    auto tokens = scanAll("impure");

    EXPECT_SCAN_EOF(tokens, 2u);
    EXPECT_TOKEN(tokens[0], TOKEN_IMPURE, "impure");
}

TEST(ScannerTest, KeywordPrefixStaysIdentifier) {
    auto tokens = scanAll("trueish mutant fnn falsehood impurely imutable");

    EXPECT_SCAN_EOF(tokens, 7u);
    EXPECT_TOKEN(tokens[0], TOKEN_IDENTIFIER, "trueish");
    EXPECT_TOKEN(tokens[1], TOKEN_IDENTIFIER, "mutant");
    EXPECT_TOKEN(tokens[2], TOKEN_IDENTIFIER, "fnn");
    EXPECT_TOKEN(tokens[3], TOKEN_IDENTIFIER, "falsehood");
    EXPECT_TOKEN(tokens[4], TOKEN_IDENTIFIER, "impurely");
    EXPECT_TOKEN(tokens[5], TOKEN_IDENTIFIER, "imutable");
}
