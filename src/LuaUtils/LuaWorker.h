#pragma once

#include "pch.h"

////////////////////////////////////////////////////////////////
class LuaWorker
{
public:
//    typedef std::list<Argument> Arguments;

    explicit LuaWorker();
    ~LuaWorker();

    void setScript(const std::string& script);
    void setFunction(const std::string& name);
    void setFile(const std::string& name);

    /// Argumenty sa pridaju automaticky podla typu.
    ///
    /// @attention Ked pridavamae char* tak sa prida pointer, potrebujeme specifikovat typ templatu addArgument<std::string>("text");
//    template<typename T> void addArgument(const T& arg) {
//        _arguments.push_back(Argument(arg));
//    }
    
    void startProtected(lua_State* luaState);
    void start(lua_State* luaState);

//    void started(QThread* thread);
//    void finished();
//    void stop();
//    void failed(const std::string& error);

private:

    std::string _function;
    std::string _script;
    std::string _file;
//    Arguments _arguments;

    void prepareLuaState(lua_State* L);
};
