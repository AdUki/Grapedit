#pragma once

#include "pch.h"

#include "./LuaArg.h"

DECLARE_CLASS_PTR(LuaState);
DECLARE_CLASS_PTR(LuaWorker);

//////////////////////////////////////////////////////////////////////////////////////////////////
enum class WorkerState {
    /// Úloha sa začala vykonvávať
    Started,
    
    /// Úloha skončila
    Finished,
    
    /// Nastala chyba pri vykonávaní úlohy
    Error,
};

typedef std::function<void(WorkerState)> OnWorkerStateChangedFnc;

//////////////////////////////////////////////////////////////////////////////////////////////////
class LuaState
{
public:
    
    enum class ExecMode {
        /// Pre spustenie úlohy sa použije mutex, t. j. ak sa vykonáva úloha, nová čaká na jej dokončenie a keď skončí tak sa vyberie náhodná z čakajúcich
        Mutex,
        
        /// Pre spustenie úlohy sa FIFO rad, t. j. ak sa vykonáva úloha, nová čaká na jej dokončenie a keď skončí tak sa vyberie náhodná z čakajúcich
        Queue,
        
        /// Úlohy sa spúštajú paralelne, t. j. ak sa vykonávajú úlohý z inými módmi spustenia, tak sa čaká na ich dokončenie
        Paralel,
    };
    
    LuaState();
    LuaState(std::function<void(lua_State* L)> initialization);
    ~LuaState();
    
    template <typename... Arg>
    void execute(const Arg&... arg) { execute(_defaultExecMode, arg...); }

    template <typename... Arg>
    void execute(ExecMode execMode, Arg&&... arg) { execute(execMode, NULL, arg...); }
    
    template <typename... Arg>
    void execute(ExecMode execMode, const OnWorkerStateChangedFnc& callback, Arg&&... arg) {
        LuaArgsPtr arguments = std::make_shared<LuaArgs>();
        arguments->push_back(arg...);
        tryExec(execMode, arguments, callback);
    }
    
    void setDefaultExecMode(ExecMode execMode) { _defaultExecMode = execMode; }
    
private:
    
    struct QueuedTask {
        ExecMode mode;
        LuaArgsPtr arguments;
        OnWorkerStateChangedFnc callback;
    };
    
    typedef std::list<QueuedTask> TaskQueue;
    
    TaskQueue _taskQueue;
    
    lua_State* _luaState;
    
    ExecMode _defaultExecMode;
    
    void tryExec(ExecMode execMode, const LuaArgsPtr& arguments, const OnWorkerStateChangedFnc& callback);
};

