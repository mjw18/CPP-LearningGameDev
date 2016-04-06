#pragma once

enum class InputAction
{
	JUMP,
	MOVE_LEFT,
	MOVE_RIGHT,
	ATTACK
};

enum class InputState
{
	LOOK_UP,
	LOOK_LEFT,
	LOOK_RIGHT,
	RUN_LEFT,
	RUN_RIGHT,
	ATTACK_POS,
	DEFENSE_POS,
	COUNTER_POS
};