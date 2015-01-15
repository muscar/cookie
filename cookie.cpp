#include <iostream>
#include <fstream>
#include <functional>
#include <map>

#include <ncurses.h>

class buffer
{
    std::string contents_;
    std::string::iterator start_pos_, end_pos_;

public:
    buffer() { }

    buffer(std::string contents)
    : contents_{contents}, start_pos_{std::begin(contents_)}, end_pos_{std::begin(contents_)}
    { }

    void append(const std::string &text)
    {
        contents_ += text;
    }

    void insert(const std::string &text)
    {
        contents_.replace(start_pos_, end_pos_, text);
    }

    std::string contents() const
    {
        return contents_;
    }

    void set_start_pos(std::string::size_type pos)
    {
        start_pos_ = std::begin(contents_) + pos;
    }

    void set_end_pos(std::string::size_type pos)
    {
        end_pos_ = std::begin(contents_) + pos;
    }
};

class editor_state
{
    bool run_ = true;
    buffer buf_;

public:
    bool is_running() const
    {
        return run_;
    }

    void stop()
    {
        run_ = false;
    }

    buffer &buf()
    {
        return buf_;
    }

    void set_buf(buffer buf)
    {
        buf_ = buf;
    }
};

std::string get_string(int length=255)
{
    char cmd[length];
    getnstr(cmd, length);
    return cmd;
}

buffer load_file(std::string path)
{
    std::ifstream in(path);
    buffer buf("");
    std::string line;
    while(std::getline(in, line)) {
        buf.append(line);
        buf.append("\n");
    }
    in.close();
    return buf;
}

int main(int argc, char const *argv[])
{
    editor_state state;

    std::map<char, std::function<void(std::string)>> commands = {
        {'a', [&state](auto cmd) {
            auto line = get_string(1024);
            while (line.compare(".") != 0) {
                state.buf().append(line);
                state.buf().append("\n");
                line = get_string(1024);
            }
        }},
        {'e', [&state](auto cmd) {
            state.set_buf(load_file(cmd.substr(2)));
        }},
        {'i', [&state](auto cmd) {
            std::string acc;
            auto line = get_string(1024);
            while (line.compare(".") != 0) {
                acc += line;
                line = get_string(1024);
            }
            state.buf().insert(acc);
        }},
        {'p', [&state](auto cmd) {
            printw(state.buf().contents().c_str());
            refresh();
        }},
        {'s', [&state](auto cmd) {
            auto span = cmd.substr(2);
            auto sep_pos = span.find(',');
            state.buf().set_start_pos(std::stoi(span.substr(0, sep_pos)));
            state.buf().set_end_pos(std::stoi(span.substr(sep_pos + 1)));
        }},
        {'w', [&state](auto cmd) {
            std::ofstream out(cmd.substr(2));
            out << state.buf().contents();
            out.close();
        }},
        {'q', [&state](auto cmd) {
            state.stop();
        }}
    };

    initscr();

    if (argc > 1) {
        state.set_buf(load_file(argv[1]));
    }

    while (state.is_running()) {
        auto cmd = get_string(255);
        auto impl = commands.find(cmd[0]);
        if (impl != commands.end()) {
            impl->second(cmd);
        }
    }

    endwin();

    return 0;
}

