#include "Argument.h"

////////////////////////////////////////////////////////////////
Argument::Argument()
    : _state(State::Uninitialized)
{

}

////////////////////////////////////////////////////////////////
Argument::Argument(int number)
    : _number(number)
    , _state(State::Number)
{

}

////////////////////////////////////////////////////////////////
Argument::Argument(const std::string& string)
    : _string(string)
    , _state(State::String)
{

}

////////////////////////////////////////////////////////////////
Argument::Argument(const void* pointer)
    : _pointer(pointer)
    , _state(State::Pointer)
{

}

////////////////////////////////////////////////////////////////
void Argument::pushToState(lua_State* L)
{
    switch (_state) {

    case State::String:
        lua_pushlstring(L, _string.c_str(), _string.size());
        break;

    case State::Number:
        lua_pushinteger(L, _number);
        break;

    case State::Pointer:
        lua_pushlightuserdata(L, const_cast<void*>(_pointer));
        break;

    case State::Uninitialized:
        Q_ASSERT(false);
        break;
    }
}
