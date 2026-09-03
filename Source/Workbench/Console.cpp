#include "Workbench/Console.hpp"
#include "Workbench/Commands.hpp"
#include "NoLib/Text/StringStreamAdapters.hpp"
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace Workbench
{
    namespace
    {
        using NoLib::String;
        using NoLib::ULong;
        using NoLib::UInt;

        class Terminal
        {
            bool enabled = false;
            bool amber = false;
            bool forced = false;
#ifdef _WIN32
            HANDLE handle = INVALID_HANDLE_VALUE;
            DWORD originalMode = 0;
            bool changed = false;
#endif
        public:
            explicit Terminal(bool plain, bool forceColor)
            {
                forced = !plain && forceColor;
#ifdef _WIN32
                handle = GetStdHandle(STD_OUTPUT_HANDLE);
                if (!plain && GetConsoleMode(handle, &originalMode))
                {
                    changed = SetConsoleMode(handle, originalMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
                    enabled = changed || forced;
                }
#else
                enabled = !plain && forceColor;
#endif
                if (!plain && forceColor) enabled = true;
            }
            ~Terminal()
            {
                reset();
#ifdef _WIN32
                if (changed) SetConsoleMode(handle, originalMode);
#endif
            }
            void accent() const { if (enabled) std::cout << (amber ? "\x1b[38;5;220m" : "\x1b[38;5;51m"); }
            void muted() const { if (enabled) std::cout << "\x1b[38;5;245m"; }
            void green() const { if (enabled) std::cout << "\x1b[38;5;120m"; }
            void red() const { if (enabled) std::cout << "\x1b[38;5;203m"; }
            void reset() const { if (enabled) std::cout << "\x1b[0m"; }
            void clear() const { if (enabled) std::cout << "\x1b[2J\x1b[H"; else std::cout << '\n'; }
            bool theme(const String& name)
            {
                if (equalText(name, "plain")) { reset(); enabled = false; return true; }
                if (equalText(name, "cyan") || equalText(name, "amber"))
                {
#ifdef _WIN32
                    if (!changed && GetConsoleMode(handle, &originalMode))
                        changed = SetConsoleMode(handle, originalMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
                    enabled = changed || forced;
#else
                    enabled = true;
#endif
                    amber = equalText(name, "amber");
                    return true;
                }
                return false;
            }
        };

        void banner(const Terminal& terminal)
        {
            terminal.accent();
            std::cout << "\n"
                "  +------------------------------------------------------------------+\n"
                "  |   N O L I B   /   M A T H                        WORKBENCH  1.0  |\n"
                "  |   Type an expression. Make the math happen.                      |\n"
                "  +------------------------------------------------------------------+\n";
            terminal.reset();
            std::cout << "\n  DERIVE                   EVALUATE                 INTEGRATE\n";
            terminal.muted();
            std::cout << "  x^3 + 2x + 1             x = 2                    + C\n";
            terminal.reset();
            std::cout << "\n  Try   action[dirive(x^3 + 2x + 1)]\n";
            terminal.muted();
            std::cout << "  help / examples / history / theme(amber) / quit\n"
                "  ---------------------------------------------------------------\n\n";
            terminal.reset();
        }

        void help(const Terminal& terminal, const String& topic)
        {
            terminal.accent();
            std::cout << "\n  COMMAND REFERENCE\n";
            terminal.reset();
            if (!topic.empty()) std::cout << "  Requested topic: " << topic << " (full reference below)\n";
            std::cout <<
                "\n  action[command(expression)]   Select the operation inside [].\n"
                "  command(expression)           Short form; the wrapper is optional.\n"
                "\n  derive(expr)      Differentiate with respect to x.\n"
                "                    Aliases: dirive, diff.\n"
                "  integrate(expr)   Find a supported antiderivative and append + C.\n"
                "                    Alias: int. Some expressions have no supported rule.\n"
                "  simplify(expr)    Expand/collect polynomials (degree <= 32).\n"
                "                    Other expressions are displayed with clear grouping.\n"
                "                    Alias: expand. Bare expressions also work.\n"
                "  eval(expr, value) Evaluate with x set to value; value may be pi/2.\n"
                "  eval(expr)        Evaluate a constant expression. Alias: evaluate.\n"
                "\n  help              Show all commands. Also help(), action[help()].\n"
                "  examples          Show things to copy and try.\n"
                "  history           Show the last 12 calculation inputs.\n"
                "  clear             Clear the screen. Alias: cls.\n"
                "  theme(cyan)       Cyan, amber, or plain.\n"
                "  quit              Exit. Alias: exit; end-of-input exits too.\n"
                "\n  INPUT SYNTAX\n"
                "  + - * / ^         Operators; x is the variable.\n"
                "  2x  3(x+1)        Implicit multiplication; (x+1)(x-1) also works.\n"
                "  0.5  .5  1e-3     Decimal and scientific notation.\n"
                "  sin cos tan atan  Functions use parentheses; angles are radians.\n"
                "  exp ln lnabs      ln(x) needs x>0; lnabs(x) means ln(|x|), x!=0.\n"
                "  pi  e             Constants; use 2*e (2e3 means 2000).\n"
                "\n  Powers use constant integers [-1024,1024]: x^3, x^-2, x^(1+2).\n"
                "  ^ is right-associative: 2^3^2 = 512. -x^2 means -(x^2).\n"
                "  * and /, including implicit *, run left to right: 6/2x = 3x.\n"
                "  Use 6/(2x) when you mean a denominator of 2x.\n"
                "  Use x*x, not xx. Function/command names are case-insensitive.\n"
                "\n  LIMITS\n"
                "  One variable; no equations, fractional/variable powers, or sqrt.\n"
                "  2048 characters, 512 nodes, depth 48. Trig inputs: +/-1e6 radians.\n"
                "  Symbolic integration uses the original restricted rule engine.\n"
                "  Calculations use floating-point arithmetic, not exact fractions.\n\n";
        }

        void examples(const Terminal& terminal)
        {
            terminal.accent();
            std::cout << "\n  TRY THESE\n";
            terminal.reset();
            std::cout <<
                "\n  action[dirive(x^3 + 2x + 1)]      -> 3x^2 + 2\n"
                "  action[eval(x^3 + 2x + 1, 2)]    -> 13\n"
                "  action[integrate(3x^2 + 2)]      -> x^3 + 2x + C\n"
                "  action[simplify((x+1)(x-1))]     -> x^2 - 1\n"
                "  derive(sin(x))                  -> cos(x)\n"
                "  eval(2^3^2)                     -> 512\n"
                "  eval(sin(x), pi/2)              -> approximately 1\n"
                "  eval(1.5e2 + .25)               -> 150.25\n"
                "  2x + 3x + 1                     -> 5x + 1\n\n";
        }

        void showError(const Terminal& terminal, const String& line, const Diagnostic& error)
        {
            terminal.red();
            std::cout << "\n  ! " << error.message << '\n';
            terminal.reset();
            // Crop around the offending position so even long pasted lines have
            // an aligned caret in narrow consoles. Expand tabs to one space.
            const ULong position = error.position < line.length() ? error.position : line.length();
            const ULong start = position > 45 ? position - 45 : 0;
            const ULong end = start + 65 < line.length() ? start + 65 : line.length();
            std::cout << "    ";
            if (start > 0) std::cout << "...";
            for (ULong i = start; i < end; ++i) std::cout << (line[i] < 32 ? ' ' : line[i]);
            if (end < line.length()) std::cout << "...";
            std::cout << "\n    ";
            if (start > 0) std::cout << "   ";
            for (ULong i = start; i < position; ++i) std::cout << ' ';
            terminal.red();
            std::cout << "^  column " << position + 1 << "\n\n";
            terminal.reset();
        }

        bool readLine(String& text, bool& tooLong)
        {
            text.clear();
            tooLong = false;
            char c = '\0';
            bool gotAny = false;
            while (std::cin.get(c))
            {
                gotAny = true;
                if (c == '\n') break;
                if (c == '\r') continue;
                if (text.length() < LineLimit) text += c;
                else tooLong = true; // consume the entire line; never execute a prefix
            }
            return gotAny;
        }

        struct Session
        {
            String history[12];
            UInt next = 0;
            UInt count = 0;
            bool quit = false;

            bool execute(const Terminal& terminal, const String& line, Command& command, Diagnostic& error)
            {
                if (!parseCommand(line, command, error)) return false;
                switch (command.kind)
                {
                case CommandKind::Help: help(terminal, command.arguments[0]); return true;
                case CommandKind::Examples: examples(terminal); return true;
                case CommandKind::Quit: quit = true; return true;
                case CommandKind::Clear: terminal.clear(); banner(terminal); return true;
                case CommandKind::History:
                    std::cout << "\n  RECENT CALCULATIONS\n";
                    if (count == 0) std::cout << "  Your first calculation will appear here.\n";
                    for (UInt i = 0; i < count; ++i)
                        std::cout << "  " << i + 1 << "  " << history[(next + 12 - count + i) % 12] << '\n';
                    std::cout << '\n';
                    return true;
                case CommandKind::Theme: return true; // terminal mutates in caller
                default: break;
                }
                history[next] = line;
                next = (next + 1) % 12;
                if (count < 12) ++count;
                Result result;
                if (!calculate(command, result, error)) return false;
                terminal.muted();
                std::cout << "\n  " << result.label << '\n';
                terminal.green();
                std::cout << "  = " << result.text << '\n';
                terminal.reset();
                if (!result.note.empty()) { terminal.muted(); std::cout << "  " << result.note << '\n'; terminal.reset(); }
                std::cout << '\n';
                return true;
            }
        };
    }

    int runConsole(int argc, char** argv)
    {
        bool plain = false, forceColor = false, single = false;
        String singleLine;
        for (int i = 1; i < argc; ++i)
        {
            const String argument(argv[i]);
            if (equalText(argument, "--plain")) plain = true;
            else if (equalText(argument, "--color")) forceColor = true;
            else if (equalText(argument, "--command") && i + 1 < argc)
            {
                single = true;
                singleLine = argv[++i];
            }
            else if (equalText(argument, "--help")) { single = true; singleLine = "help"; }
            else
            {
                std::cerr << "Usage: NoLibMath [--plain|--color] [--command \"action[derive(x^3)]\"]\n";
                return 2;
            }
        }
        Terminal terminal(plain, forceColor);
        Session session;
        if (!single) banner(terminal);
        bool lastSuccess = true;
        while (!session.quit)
        {
            String line;
            bool tooLong = false;
            if (single) line = singleLine;
            else
            {
                terminal.accent();
                std::cout << "  math > ";
                terminal.reset();
                std::cout.flush();
                if (!readLine(line, tooLong)) break;
            }
            bool blank = true;
            for (ULong i = 0; i < line.length(); ++i) if (line[i] > 32) blank = false;
            if (blank && !tooLong && !single) continue;
            Diagnostic error;
            Command command;
            if (tooLong)
            {
                error.message = "Input exceeds 2048 characters; shorten the expression.";
                error.position = LineLimit;
                lastSuccess = false;
            }
            else lastSuccess = session.execute(terminal, line, command, error);
            if (lastSuccess && command.kind == CommandKind::Theme)
            {
                if (command.argumentCount == 0) std::cout << "\n  Themes: theme(cyan), theme(amber), theme(plain)\n\n";
                else if (!terminal.theme(command.arguments[0]))
                {
                    error.message = "Choose theme(cyan), theme(amber), or theme(plain).";
                    error.position = command.offsets[0];
                    lastSuccess = false;
                }
                else { terminal.accent(); std::cout << "\n  Theme updated.\n\n"; terminal.reset(); }
            }
            if (!lastSuccess) showError(terminal, line, error);
            if (single) return lastSuccess ? 0 : 1;
        }
        std::cout << "\n  See you next calculation.\n";
        return 0;
    }
}
