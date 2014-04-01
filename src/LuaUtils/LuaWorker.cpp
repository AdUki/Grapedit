#include "LuaWorker.h"

#include <QDebug>

////////////////////////////////////////////////////////////////
LuaWorker::LuaWorker()
{
}

////////////////////////////////////////////////////////////////
LuaWorker::~LuaWorker()
{
    // qDebug() << "FINISHED" << _thread;
}

////////////////////////////////////////////////////////////////
void LuaWorker::setScript(const std::string& script)
{
    assert(_function.empty());
    _script = script;
}

////////////////////////////////////////////////////////////////
void LuaWorker::setFunction(const std::string& name)
{
    assert(_script.empty());
    _function = name;
}

////////////////////////////////////////////////////////////////
void LuaWorker::setFile(const std::string& name)
{
    assert(_file.empty());
    _file = name;
}

////////////////////////////////////////////////////////////////
void LuaWorker::start(lua_State* luaState)
{
    //    emit started(_thread);
    
    prepareLuaState(luaState);
//    lua_call(luaState, _arguments.size(), 0);
    
    //    emit finished();
}

////////////////////////////////////////////////////////////////
void LuaWorker::startProtected(lua_State* luaState)
{
    //    emit started(_thread);
    
    try {
        prepareLuaState(luaState);
//        if (lua_pcall(luaState, _arguments.size(), 0, 0))
//            throw 2;
    }
    catch(...) {
        
        std::string message(lua_tostring(luaState, -1));
        lua_pop(luaState, 1);  /* pop error message from the stack */
        
        //        emit failed(message);
    }
    
    //    emit finished();
}

////////////////////////////////////////////////////////////////
void LuaWorker::prepareLuaState(lua_State* L)
{
    // Push function and arguments
    if (!_function.empty()) {

        lua_getglobal(L, _function.c_str());

//        for (Argument& arg : _arguments)
//            arg.pushToState(L);
    }

    else if (!_file.empty() &&
        luaL_loadfile(L, _file.c_str()))
        throw 1;

    // Push script
    else if (!_script.empty() &&
        luaL_loadbuffer(L, _script.data(), _script.length(), "script"))
        throw 1;
}
