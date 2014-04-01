#pragma once

#include "pch.h"

class LuaState;

namespace LuaArg {

    //////////////////////////////////////////////////////////////////////////////////////////////////
    class Base
    {
        friend class LuaState;
        
        virtual void initState(lua_State* luaState) const = 0;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////
    class Function : public Base
    {
    public:
        Function(const std::string& functionName) : _functionName(functionName) {}
        
    private:
        const std::string& _functionName;
        
        void initState(lua_State* luaState) const;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////
    class Script : public Base
    {
    public:
        Script(const std::string& script) : _script(script) {}
        
    private:
        const std::string& _script;
        
        void initState(lua_State* luaState) const;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////
    class Pointer : public Base
    {
    public:
        Pointer(const void* const rawPointer) : _rawPointer(rawPointer) {}
        
    private:
        const void* const _rawPointer;
        
        void initState(lua_State* luaState) const;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////
    class String : public Base
    {
    public:
        String(const std::string& string) : _string(string) {}
        
    private:
        const std::string& _string;
        
        void initState(lua_State* luaState) const;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////
    class Number : public Base
    {
    public:
        Number(double number) : _number(number) {}
        
    private:
        double _number;
        
        void initState(lua_State* luaState) const;
    };
}

typedef std::list<LuaArg::Base> LuaArgs;
typedef std::shared_ptr<LuaArgs> LuaArgsPtr;
