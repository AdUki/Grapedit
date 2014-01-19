#pragma once

#include "pch.h"

DECLARE_CLASS_PTR(GraphicTextState)

class GraphicTextState
{
public:
	GraphicTextState();
	~GraphicTextState();

	/// Funkcia na reparsovanie noveho textu, volanie sa spusta na novom threade a nasledne su volane callbacky
	///
	/// @param text, ktory sa bude parsovat
	void reparseText(const std::string& text);

	/// Funkcia nam nacita subor s gramatikov
	///
	/// @param nazov suboru gramatiky bez pripony
	void loadGrammarFile(const std::string& filename);

signals:

	void addElements();
	void removeElements();
	void updateElements();

private:

	lua_State* _state;
};
