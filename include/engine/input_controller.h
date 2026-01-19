#include <raylib.h>

typedef enum _ict {
	IC_UP,
	IC_DOWN,
	IC_LEFT,
	IC_RIGHT,
	IC_JUMP,
	IC_DASH,
	IC_STOMP
} InputControllerToken;

void InputControllerUpdate();
bool InputControllerIsTokenPressed(InputControllerToken t);
bool InputControllerIsTokenReleased(InputControllerToken t);
bool InputControllerIsTokenDown(InputControllerToken t);
bool InputControllerIsTokenUp(InputControllerToken t);
