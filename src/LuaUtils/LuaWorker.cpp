#include "LuaWorker.h"

#include <lua.hpp>

#include <QMutex>
#include <QThread>
#include <QDebug>

////////////////////////////////////////////////////////////////
LuaWorker::LuaWorker(lua_State* L)
: QObject()
, _luaState(L)
{
    Q_ASSERT(L != nullptr);

    _thread = new QThread();
    moveToThread(_thread);

    // connect(this, SIGNAL(failed(std::string)), Interpreter::getInstance(), SIGNAL(emitError(std::string)));
    // connect(this, SIGNAL(finished()), Interpreter::getInstance(), SIGNAL(workDone()));

    connect(this, SIGNAL(finished()), _thread, SLOT(quit()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));

// NOTE expecting that all created workers will be started with start or startProtected
    // qDebug() << "STARTED " << _thread;
}

////////////////////////////////////////////////////////////////
LuaWorker::~LuaWorker()
{
    // qDebug() << "FINISHED" << _thread;
}

////////////////////////////////////////////////////////////////
void LuaWorker::setScript(const std::string& script)
{
    Q_ASSERT(_function.empty());
    _script = script;
}

////////////////////////////////////////////////////////////////
void LuaWorker::setFunction(const std::string& name)
{
    Q_ASSERT(_script.empty());
    _function = name;
}

////////////////////////////////////////////////////////////////
void LuaWorker::setFile(const std::string& name)
{
    Q_ASSERT(_file.empty());
    _file = name;
}

////////////////////////////////////////////////////////////////
void LuaWorker::start(QMutex* mutex)
{
    _mutex = mutex;
    connect(_thread, SIGNAL(started()), this, SLOT(call()));
    _thread->start();
}

////////////////////////////////////////////////////////////////
void LuaWorker::startProtected(QMutex* mutex)
{
    _mutex = mutex;
    connect(_thread, SIGNAL(started()), this, SLOT(protectedCall()));
    _thread->start();
}

////////////////////////////////////////////////////////////////
void LuaWorker::protectedCall()
{
    if (_mutex != nullptr)
        _mutex->lock();

    emit started(_thread);

    try {
        prepareLuaState(_luaState);
        if (lua_pcall(_luaState, _arguments.size(), 0, 0))
            throw 2;
    }
    catch(...) {

        std::string message(lua_tostring(_luaState, -1));
        lua_pop(_luaState, 1);  /* pop error message from the stack */

        emit failed(message);
    }

    emit finished();

    if (_mutex != nullptr)
        _mutex->unlock();
}

////////////////////////////////////////////////////////////////
void LuaWorker::call()
{
    if (_mutex != nullptr)
        _mutex->lock();

    emit started(_thread);

    prepareLuaState(_luaState);
    lua_call(_luaState, _arguments.size(), 0);

    emit finished();

    if (_mutex != nullptr)
        _mutex->unlock();
}

////////////////////////////////////////////////////////////////
void LuaWorker::prepareLuaState(lua_State* L)
{
    // Push function and arguments
    if (!_function.empty()) {

        lua_getglobal(L, _function.c_str());

        for (Argument& arg : _arguments)
            arg.pushToState(L);
    }

    else if (!_file.empty() &&
        luaL_loadfile(L, _file.c_str()))
        throw 1;

    // Push script
    else if (!_script.empty() &&
        luaL_loadbuffer(L, _script.data(), _script.length(), "script"))
        throw 1;
}
