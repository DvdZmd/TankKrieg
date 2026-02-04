#include "InputManager.h"
#include <cmath>

static Int2 QuantizeStickTo8Visual(const Vector2& v, float dz)
{
	if (v.Length() < dz) return { 0, 0 };

	Int2 s{ 0,0 };
	s.x = (v.x > dz) ? 1 : (v.x < -dz ? -1 : 0);
	s.y = (v.y > dz) ? 1 : (v.y < -dz ? -1 : 0);
	return s; // esto es “visual”: x=izq/der, y=arr/ab en pantalla
}

static Int2 VisualToIsoGridStep(const Int2& v)
{
	// 4-dir
	if (v.x == 0 && v.y == -1) return { -1,-1 }; // up
	if (v.x == 0 && v.y == 1) return { 1, 1 }; // down
	if (v.x == -1 && v.y == 0) return { -1, 1 }; // left
	if (v.x == 1 && v.y == 0) return { 1,-1 }; // right

	// diagonales visuales
	if (v.x == -1 && v.y == -1) return { -1, 0 }; // up-left
	if (v.x == 1 && v.y == -1) return { 0,-1 }; // up-right
	if (v.x == -1 && v.y == 1) return { 0, 1 }; // down-left
	if (v.x == 1 && v.y == 1) return { 1, 0 }; // down-right

	return { 0,0 };
}

static float ApplyDeadzone(float v, float dz) {
	return (std::fabs(v) < dz) ? 0.0f : v;
}

void InputManager::Initialize() {
	// SDL must be initialized before this.
	if (SDL_HasGamepad()) { // true if at least one gamepad is connected
		int count = 0;
		SDL_JoystickID* ids = SDL_GetGamepads(&count);
		if (!ids || count <= 0) {
			SDL_Log("SDL_GetGamepads failed: %s", SDL_GetError());
		}
		else {
			SDL_JoystickID firstId = ids[0];              // this is the instance id
			SDL_free(ids);

			gamepad = SDL_OpenGamepad(firstId);
			if (!gamepad) {
				SDL_Log("SDL_OpenGamepad failed: %s", SDL_GetError());
			}
			else {
				SDL_Log("Gamepad opened OK!");
			}
		}
	}
}

void InputManager::Shutdown() {
	if (gamepad) {
		SDL_CloseGamepad(gamepad);
		gamepad = nullptr;
	}
}

void InputManager::Update(float dt) {
	quit = false;

	// 1) move now state to prev state
	upPrev = upNow; downPrev = downNow; leftPrev = leftNow; rightPrev = rightNow;
	dpadUpPrev = dpadUpNow; dpadDownPrev = dpadDownNow; dpadLeftPrev = dpadLeftNow; dpadRightPrev = dpadRightNow;

	// 2) process events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ProcessEvent(e);
	}

	UpdateKeyboardState();
	UpdateGamepadState();
	ComposeFinalMove();


	// Si ya hubo un cursorStep por teclado/D-pad en este frame, reseteo repeat
	if (cursorStep.x != 0 || cursorStep.y != 0) {
		cursorRepeatTimer = 0.0f;
		return;
	}

	// Si no hubo edge, uso el stick analógico para “stepping” con repeat
	if (analogStep.x != 0 || analogStep.y != 0) {
		cursorRepeatTimer += dt;
		if (cursorRepeatTimer >= cursorRepeatDelay) {
			cursorRepeatTimer = 0.0f;
			cursorStep = VisualToIsoGridStep(analogStep);
		}
	}
	else {
		cursorRepeatTimer = 0.0f;
	}

}

void InputManager::ProcessEvent(const SDL_Event& e) {
	if (e.type == SDL_EVENT_QUIT) {
		quit = true;
	}

	// Optional (future): handle controller connect/disconnect events here.
}

void InputManager::UpdateKeyboardState() {
	keyboardMove = {};

	const bool* k = SDL_GetKeyboardState(nullptr);

	upNow = k[SDL_SCANCODE_UP];
	downNow = k[SDL_SCANCODE_DOWN];
	leftNow = k[SDL_SCANCODE_LEFT];
	rightNow = k[SDL_SCANCODE_RIGHT];

	if (upNow)    keyboardMove.y -= 1.0f;
	if (downNow)  keyboardMove.y += 1.0f;
	if (leftNow)  keyboardMove.x -= 1.0f;
	if (rightNow) keyboardMove.x += 1.0f;

	if (keyboardMove.Length() > 0.0f) {
		keyboardMove.Normalize();
	}
}

void InputManager::UpdateGamepadState() {
	gamepadMove = {};
	gamepadAim = {}; // NEW

	// Reset dpad state each frame
	dpadUpNow = dpadDownNow = dpadLeftNow = dpadRightNow = false;

	if (!gamepad) return;

	// Left stick for continuous movement
	float x = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f;
	float y = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f;

	x = ApplyDeadzone(x, 0.2f);
	y = ApplyDeadzone(y, 0.2f);

	gamepadMove = { x, y };
	if (gamepadMove.Length() > 1.0f) gamepadMove.Normalize();


	// Right stick for aim (turret)
	float rx = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f;
	float ry = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / 32767.0f;

	rx = ApplyDeadzone(rx, 0.2f);
	ry = ApplyDeadzone(ry, 0.2f);

	gamepadAim = { rx, ry };
	if (gamepadAim.Length() > 1.0f) gamepadAim.Normalize();

	// D-pad for discrete cursor stepping
	dpadUpNow = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
	dpadDownNow = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
	dpadLeftNow = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
	dpadRightNow = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
}

void InputManager::ComposeFinalMove() {
	// 1) CHASSIS MOVE: if left stick is moved, use it; otherwise keyboard.
	finalMove = (gamepadMove.Length() > 0.0f) ? gamepadMove : keyboardMove;
	analogStep = QuantizeStickTo8Visual(gamepadMove, 0.35f);

	// 2) AIM: right stick 
	aim = gamepadAim; // si está en cero, significa "no apuntar / mantener"

	//3) CURSOR STEP: (edge-trigger + “visual directions” mapping) =====
	cursorStep = { 0, 0 };

	// keyboard edges
	const bool leftEdge = leftNow && !leftPrev;
	const bool rightEdge = rightNow && !rightPrev;
	const bool upEdge = upNow && !upPrev;
	const bool downEdge = downNow && !downPrev;

	// dpad edges
	const bool dLeftEdge = dpadLeftNow && !dpadLeftPrev;
	const bool dRightEdge = dpadRightNow && !dpadRightPrev;
	const bool dUpEdge = dpadUpNow && !dpadUpPrev;
	const bool dDownEdge = dpadDownNow && !dpadDownPrev;

	const bool anyEdge = leftEdge || rightEdge || upEdge || downEdge ||
		dLeftEdge || dRightEdge || dUpEdge || dDownEdge;

	if (!anyEdge) return;


	const bool leftNowCombined = leftNow || dpadLeftNow;
	const bool rightNowCombined = rightNow || dpadRightNow;
	const bool upNowCombined = upNow || dpadUpNow;
	const bool downNowCombined = downNow || dpadDownNow;


	// Queremos permitir diagonales aunque una tecla esté sostenida y la otra recién presionada.
	// Por eso elegimos "visual" en base al estado actual (now), pero solo cuando hubo un edge.
	int vx = 0, vy = 0;
	if (leftNowCombined && !rightNowCombined)  vx = -1;
	else if (rightNowCombined && !leftNowCombined) vx = 1;

	if (upNowCombined && !downNowCombined) vy = -1;
	else if (downNowCombined && !upNowCombined) vy = 1;

	// mapeo VISUAL -> GRID
	if (vx == 0 && vy == -1)      cursorStep = { -1, -1 }; // up
	else if (vx == 0 && vy == 1)  cursorStep = { 1,  1 }; // down
	else if (vx == -1 && vy == 0) cursorStep = { -1,  1 }; // left
	else if (vx == 1 && vy == 0)  cursorStep = { 1, -1 }; // right
	else if (vx == -1 && vy == -1) cursorStep = { -1,  0 }; // up-left
	else if (vx == 1 && vy == -1)  cursorStep = { 0, -1 }; // up-right
	else if (vx == -1 && vy == 1)  cursorStep = { 0,  1 }; // down-left
	else if (vx == 1 && vy == 1)   cursorStep = { 1,  0 }; // down-right
}


Vector2 InputManager::GetMovementVector() const {
	return finalMove;
}

Int2 InputManager::GetCursorStep() const { return cursorStep; }

bool InputManager::QuitRequested() const {
	return quit;
}
