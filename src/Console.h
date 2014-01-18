#pragma once

#include "pch.h"

DECLARE_CLASS_PTR(QThread)
DECLARE_CLASS_PTR(Console)

class Console : 
	public QObject
{
	Q_OBJECT

public:

	/// Farby ako na tomto obrazku:
	/// http://unindented.org/articles/2005/02/color-in-a-console-application/image-01.png
	enum TextColor { Black = 0, Navy, Green, Teal, Maroon, Purple, Silver, Gray, Blue, Lime, Aqua, Red, Fuchisa, Yellow, White };

	Console();
	~Console();

	static ConsolePtr getInstance();

	static void setTextColor(TextColor color);
	static void resetTextColor();

signals:

	/// Pouzivatel zadal prikaz do konzoly, vyvola sa vtedy ak pouzivatel stlacil enter
	///
	/// @param zadany riadok prikazu
	void commandEntered(const std::string& command);

private slots:

	void startReadingCommands();

private:

	QThreadPtr _commandReader;
};
