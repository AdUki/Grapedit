#include "Console.h"

#include <QThread>

//////////////////////////////////////////////////////////////////////////////////////////////////
Console::Console()
{
	_commandReader = std::make_shared<QThread>();

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
	static ConsolePtr instance = std::make_shared<Console>();

	return instance;
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
