#pragma once

#include "pch.h"

#include <QObject>
#include <QVector>

#include "./Argument.h"

////////////////////////////////////////////////////////////////
class LuaWorker : public QObject
{
    Q_OBJECT

public:
    typedef QVector<Argument> Arguments;

    explicit LuaWorker(lua_State* L);
    ~LuaWorker();

    void setScript(const std::string& script);
    void setFunction(const std::string& name);
    void setFile(const std::string& name);

    /// Argumenty sa pridaju automaticky podla typu.
    ///
    /// @attention Ked pridavamae char* tak sa prida pointer, potrebujeme specifikovat typ templatu addArgument<std::string>("text");
    template<typename T> void addArgument(const T& arg) {
        _arguments.append(Argument(arg));
    }
    
    void startProtected(bool onThread);
    void start(bool onThread);

private slots:
    void protectedCall();
    void call();

signals:
    void started(QThread* thread);
    void finished();
    void stop();
    void failed(const std::string& error);

private:
    
    QThread* _thread;
    lua_State* _luaState;

    std::string _function;
    std::string _script;
    std::string _file;
    Arguments _arguments;

    void prepareLuaState(lua_State* L);
};
