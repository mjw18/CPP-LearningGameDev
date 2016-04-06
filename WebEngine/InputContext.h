#pragma once
class InputContext
{
public:
	enum class EventType {
		ACTION, STATE, RANGE
	};

	InputContext();
	~InputContext();

	void init();
	 
	void addKeyBinding();

	void setKeyBinding();

private:

};

