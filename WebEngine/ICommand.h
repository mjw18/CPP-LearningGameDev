/*
* Command Interface
*/
#pragma once

namespace WebEngine
{
	class ICommand
	{
	public:
		ICommand();
		~ICommand();

		//Where the function call occurs
		virtual void execute() = 0;
		//Undo command, not necessarily overriden
		virtual void undo();
	};
}