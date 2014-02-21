#pragma once

#include <QByteArray>
#include <QString>

class lua_State;

///
/// Use implicit constructors to initialize
/// e. g. Interpreter::getInstance()->addArgument(8)
///       Interpreter::getInstance()->addArgument("value=8")
///
class Argument
{
public:
    Argument();
    Argument(int number);
    Argument(const std::string& string);
    Argument(const void* pointer);

    void pushToState(lua_State* L);

private:
    enum class State {
        Uninitialized = 0,
        Number,
        String,
        Pointer
    };

    std::string _string;
    int _number;
    const void* _pointer;

    State _state;
};
