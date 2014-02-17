#include "Console.h"

#include <QThread>

//////////////////////////////////////////////////////////////////////////////////////////////////
Console::Console()
{
	_commandReader = boost::make_shared<QThread>();

	moveToThread(_commandReader.get());
    
	connect(_commandReader.get(), SIGNAL(started()), this, SLOT(startReadingCommands()));
	
	_commandReader->start();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Console::~Console()
{
	_commandReader->quit();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
ConsolePtr Console::getInstance()
{
	static ConsolePtr instance = boost::make_shared<Console>();

	return instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Console::setTextColor(TextColor color)
{
#ifdef _WIN32
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
#else
	// TODO: not tested
	static char** ansiCodes = { "\033[30m", "", "\033[32m", "\033[36m", "", "", "", "", "\033[34m", "\033[32m", "", "\033[31m", "\033[35m", "\033[33m", "\033[37m" }
	printf(ansiCodes[color]);
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Console::resetTextColor()
{
#ifdef _WIN32
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, TextColor::Silver);
#else
	// TODO: not tested
	printf("\033[0m");
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Console::startReadingCommands()
{
	char command[1000];
	
	while (true) {
		if (fgets(command, sizeof(command), stdin) != NULL) {
			emit commandEntered(command);
		}
	}
}