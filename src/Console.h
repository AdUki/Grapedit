#pragma once

#include "pch.h"

DECLARE_CLASS_PTR(QThread);
DECLARE_CLASS_PTR(Console);

class Console :
	public QObject
{
	Q_OBJECT //-V524

public:

	Console();
	~Console();

	static ConsolePtr getInstance();

signals:

	/// Pouzivatel zadal prikaz do konzoly, vyvola sa vtedy ak pouzivatel stlacil enter
	///
	/// @param zadany riadok prikazu
	void commandEntered(const std::string& command);

private slots:

	void startReadingCommands();

private:

	QThreadPtr _commandReader;

	static std::vector<const char*> initializeAnsiCodes();
};
