#include "engine/input_controller.h"
#include "fixed.h"

typedef struct _gamepad {
    bool available;

    f32 leftStickX;
    f32 leftStickY;

    f32 rightStickX;
    f32 rightStickY;

    f32 leftTrigger;
    f32 rightTrigger;

    bool leftBumperDown;
    bool leftBumperUp;
    bool leftBumperPressed;
    bool leftBumperReleased;

    bool rightBumperDown;
    bool rightBumperUp;
    bool rightBumperPressed;
    bool rightBumperReleased;

    bool dpadUpDown;
    bool dpadUpUp;
    bool dpadUpPressed;
    bool dpadUpReleased;

    bool dpadDownDown;
    bool dpadDownUp;
    bool dpadDownPressed;
    bool dpadDownReleased;

    bool dpadLeftDown;
    bool dpadLeftUp;
    bool dpadLeftPressed;
    bool dpadLeftReleased;

    bool dpadRightDown;
    bool dpadRightUp;
    bool dpadRightPressed;
    bool dpadRightReleased;

    bool yDown;
    bool yUp;
    bool yPressed;
    bool yReleased;

    bool aDown;
    bool aUp;
    bool aPressed;
    bool aReleased;

    bool xDown;
    bool xUp;
    bool xPressed;
    bool xReleased;

    bool bDown;
    bool bUp;
    bool bPressed;
    bool bReleased;
} Gamepad;

Gamepad gamepad = {
    .available = false,
};

void InputControllerUpdate() {
    const i32 g = 0;
    gamepad.available = IsGamepadAvailable(g);
    if (gamepad.available) {
        // sticks
        gamepad.leftStickX = GetGamepadAxisMovement(g, GAMEPAD_AXIS_LEFT_X);
        gamepad.leftStickY = GetGamepadAxisMovement(g, GAMEPAD_AXIS_LEFT_Y);
        gamepad.rightStickX = GetGamepadAxisMovement(g, GAMEPAD_AXIS_RIGHT_X);
        gamepad.rightStickY = GetGamepadAxisMovement(g, GAMEPAD_AXIS_RIGHT_Y);

        // triggers
        gamepad.leftTrigger =
            GetGamepadAxisMovement(g, GAMEPAD_AXIS_LEFT_TRIGGER);
        gamepad.rightTrigger =
            GetGamepadAxisMovement(g, GAMEPAD_AXIS_RIGHT_TRIGGER);

        // bumpers
        gamepad.leftBumperDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
        gamepad.leftBumperUp =
            IsGamepadButtonUp(g, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
        gamepad.leftBumperPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
        gamepad.leftBumperReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_LEFT_TRIGGER_2);

        gamepad.rightBumperDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
        gamepad.rightBumperUp =
            IsGamepadButtonUp(g, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
        gamepad.rightBumperPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
        gamepad.rightBumperReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);

        gamepad.dpadUpDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_FACE_UP);
        gamepad.dpadUpUp = IsGamepadButtonUp(g, GAMEPAD_BUTTON_LEFT_FACE_UP);
        gamepad.dpadUpPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_FACE_UP);
        gamepad.dpadUpReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_LEFT_FACE_UP);

        gamepad.dpadDownDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        gamepad.dpadDownUp =
            IsGamepadButtonUp(g, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        gamepad.dpadDownPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        gamepad.dpadDownReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_LEFT_FACE_DOWN);

        gamepad.dpadLeftDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        gamepad.dpadLeftUp =
            IsGamepadButtonUp(g, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        gamepad.dpadLeftPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        gamepad.dpadLeftReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_LEFT_FACE_LEFT);

        gamepad.dpadRightDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        gamepad.dpadRightUp =
            IsGamepadButtonUp(g, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        gamepad.dpadRightPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        gamepad.dpadRightReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);

        gamepad.dpadUpDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_FACE_UP);
        gamepad.dpadUpUp = IsGamepadButtonUp(g, GAMEPAD_BUTTON_LEFT_FACE_UP);
        gamepad.dpadUpPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_FACE_UP);
        gamepad.dpadUpReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_LEFT_FACE_UP);

        gamepad.dpadDownDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        gamepad.dpadDownUp =
            IsGamepadButtonUp(g, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        gamepad.dpadDownPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        gamepad.dpadDownReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_LEFT_FACE_DOWN);

        gamepad.dpadLeftDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        gamepad.dpadLeftUp =
            IsGamepadButtonUp(g, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        gamepad.dpadLeftPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        gamepad.dpadLeftReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_LEFT_FACE_LEFT);

        gamepad.dpadRightDown =
            IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        gamepad.dpadRightUp =
            IsGamepadButtonUp(g, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        gamepad.dpadRightPressed =
            IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        gamepad.dpadRightReleased =
            IsGamepadButtonReleased(g, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    }
}

bool InputControllerIsTokenPressed(InputControllerToken t) {
    bool res = false;
    switch (t) {
    case IC_UP: {
        if (gamepad.available) {
            res = gamepad.leftStickY < 0;
        }
        res = IsKeyPressed(KEY_W);
    } break;
    case IC_DOWN: {
        if (gamepad.available) {
            res = gamepad.leftStickY > 0;
        }
        res = IsKeyPressed(KEY_S);
    } break;
    case IC_LEFT: {
        if (gamepad.available) {
            res = gamepad.leftStickX < 0;
        }
        res = IsKeyPressed(KEY_A);
    } break;
    case IC_RIGHT: {
        if (gamepad.available) {
            res = gamepad.leftStickX > 0;
        }
        res = IsKeyPressed(KEY_D);
    } break;
    case IC_JUMP: {
        if (gamepad.available) {
            res = gamepad.aPressed;
        }
        res = IsKeyPressed(KEY_SPACE);
    } break;
    case IC_DASH: {
        if (gamepad.available) {
            res = gamepad.leftTrigger > -1.f;
        }
        res = IsKeyPressed(KEY_LEFT_SHIFT);
    } break;
    case IC_STOMP: {
        if (gamepad.available) {
            res = gamepad.rightTrigger > -1.f;
        }
        res = IsKeyPressed(KEY_S);
    } break;
    }
    return res;
}
bool InputControllerIsTokenReleased(InputControllerToken t) {
    bool res = false;
    switch (t) {
    case IC_UP: {
        if (gamepad.available) {
            res = gamepad.leftStickY < 0;
        }
        res = IsKeyReleased(KEY_W);
    } break;
    case IC_DOWN: {
        if (gamepad.available) {
            res = gamepad.leftStickY > 0;
        }
        res = IsKeyReleased(KEY_S);
    } break;
    case IC_LEFT: {
        if (gamepad.available) {
            res = gamepad.leftStickX < 0;
        }
        res = IsKeyReleased(KEY_A);
    } break;
    case IC_RIGHT: {
        if (gamepad.available) {
            res = gamepad.leftStickX > 0;
        }
        res = IsKeyReleased(KEY_D);
    } break;
    case IC_JUMP: {
        if (gamepad.available) {
            res = gamepad.aPressed;
        }
        res = IsKeyReleased(KEY_SPACE);
    } break;
    case IC_DASH: {
        if (gamepad.available) {
            res = gamepad.leftTrigger > -1.f;
        }
        res = IsKeyReleased(KEY_LEFT_SHIFT);
    } break;
    case IC_STOMP: {
        if (gamepad.available) {
            res = gamepad.rightTrigger > -1.f;
        }
        res = IsKeyReleased(KEY_S);
    } break;
    }
    return res;
}
bool InputControllerIsTokenDown(InputControllerToken t) { return false; }
bool InputControllerIsTokenUp(InputControllerToken t) { return false; }
