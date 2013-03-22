/*******************************************************************************
 *
 * This file is part of the AEON Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 * 
 * See LICENSE.txt for licensing details.
 * 
 ******************************************************************************/
#ifndef IInput_h__
#define IInput_h__

#include <deque>
#include <algorithm>

#include <Aeon/Subsystem/ISubsystem.h>

namespace Aeon {
	/**
	 * The mouse buttons.
	 */
	enum MouseButton
	{
		MOUSE_BUTTON_NONE = 0,
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_MIDDLE,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_EXTRA1,
		MOUSE_BUTTON_EXTRA2
	};

	/**
	 * The keyboard modifier keys. 
	 * 
	 * Please note that these values do not indicate whether the left or right one are pressed. Use the "Key" for that.
	 */
	enum ModifierKeys
	{
		MODIFIER_SHIFT    = 1,
		MODIFIER_CTRL     = 2,
		MODIFIER_ALT      = 4
	};


	/**
	 * Keycodes.
	 */
	enum Key
	{
		KEY_UNKNOWN = 0,
		KEY_BACK = 0x08,
		KEY_TAB = 0x09,
		KEY_CLEAR = 0x0C,
		KEY_RETURN = 0x0D,
		KEY_SHIFT = 0x10,
		KEY_CONTROL = 0x11,
		KEY_MENU = 0x12,
		KEY_PAUSE = 0x13,
		KEY_CAPITAL = 0x14,
		KEY_HANGUL_KANA = 0x15,
		KEY_JUNJA = 0x17,
		KEY_FINAL = 0x18,
		KEY_HANJA_KANJI = 0x19,
		KEY_ESCAPE = 0x1B,
		KEY_CONVERT = 0x1C,
		KEY_NONCONVERT = 0x1D,
		KEY_ACCEPT = 0x1E,
		KEY_MODECHANGE = 0x1F,
		KEY_SPACE = 0x20,
		KEY_PAGE_UP = 0x21,
		KEY_PAGE_DOWN = 0x22,
		KEY_END = 0x23,
		KEY_HOME = 0x24,
		KEY_LEFT = 0x25,
		KEY_UP = 0x26,
		KEY_RIGHT = 0x27,
		KEY_DOWN = 0x28,
		KEY_SELECT = 0x29,
		KEY_PRINT = 0x2A,
		KEY_EXE = 0x2B, // Execute key
		KEY_SNAPSHOT = 0x2C,
		KEY_INSERT = 0x2D,
		KEY_DELETE = 0x2E,
		KEY_HELP = 0x2F,
		KEY_0 = 0x30,
		KEY_1 = 0x31,
		KEY_2 = 0x32,
		KEY_3 = 0x33,
		KEY_4 = 0x34,
		KEY_5 = 0x35,
		KEY_6 = 0x36,
		KEY_7 = 0x37,
		KEY_8 = 0x38,
		KEY_9 = 0x39,
		KEY_A = 0x41,
		KEY_B = 0x42,
		KEY_C = 0x43,
		KEY_D = 0x44,
		KEY_E = 0x45,
		KEY_F = 0x46,
		KEY_G = 0x47,
		KEY_H = 0x48,
		KEY_I = 0x49,
		KEY_J = 0x4A,
		KEY_K = 0x4B,
		KEY_L = 0x4C,
		KEY_M = 0x4D,
		KEY_N = 0x4E,
		KEY_O = 0x4F,
		KEY_P = 0x50,
		KEY_Q = 0x51,
		KEY_R = 0x52,
		KEY_S = 0x53,
		KEY_T = 0x54,
		KEY_U = 0x55,
		KEY_V = 0x56,
		KEY_W = 0x57,
		KEY_X = 0x58,
		KEY_Y = 0x59,
		KEY_Z = 0x5A,
		KEY_LWIN = 0x5B,
		KEY_RWIN = 0x5C,
		KEY_APPS = 0x5D,
		KEY_SLEEP = 0x5F,
		KEY_NUMPAD0 = 0x60,
		KEY_NUMPAD1 = 0x61,
		KEY_NUMPAD2 = 0x62,
		KEY_NUMPAD3 = 0x63,
		KEY_NUMPAD4 = 0x64,
		KEY_NUMPAD5 = 0x65,
		KEY_NUMPAD6 = 0x66,
		KEY_NUMPAD7 = 0x67,
		KEY_NUMPAD8 = 0x68,
		KEY_NUMPAD9 = 0x69,
		KEY_MULTIPLY = 0x6A,
		KEY_ADD = 0x6B,
		KEY_SEPARATOR = 0x6C,
		KEY_SUBTRACT = 0x6D,
		KEY_DECIMAL = 0x6E,
		KEY_DIVIDE = 0x6F,
		KEY_F1 = 0x70,
		KEY_F2 = 0x71,
		KEY_F3 = 0x72,
		KEY_F4 = 0x73,
		KEY_F5 = 0x74,
		KEY_F6 = 0x75,
		KEY_F7 = 0x76,
		KEY_F8 = 0x77,
		KEY_F9 = 0x78,
		KEY_F10 = 0x79,
		KEY_F11 = 0x7A,
		KEY_F12 = 0x7B,
		KEY_F13 = 0x7C,
		KEY_F14 = 0x7D,
		KEY_F15 = 0x7E,
		KEY_F16 = 0x7F,
		KEY_F17 = 0x80,
		KEY_F18 = 0x81,
		KEY_F19 = 0x82,
		KEY_F20 = 0x83,
		KEY_F21 = 0x84,
		KEY_F22 = 0x85,
		KEY_F23 = 0x86,
		KEY_F24 = 0x87,
		KEY_NUMLOCK = 0x90,
		KEY_SCROLL = 0x91,
		KEY_LSHIFT = 0xA0,
		KEY_RSHIFT = 0xA1,
		KEY_LCONTROL = 0xA2,
		KEY_RCONTROL = 0xA3,
		KEY_LMENU = 0xA4,
		KEY_RMENU = 0xA5,
		KEY_BROWSER_BACK = 0xA6,
		KEY_BROWSER_FORWARD = 0xA7,
		KEY_BROWSER_REFRESH = 0xA8,
		KEY_BROWSER_STOP = 0xA9,
		KEY_BROWSER_SEARCH = 0xAA,
		KEY_BROWSER_FAVORITES = 0xAB,
		KEY_BROWSER_HOME = 0xAC,
		KEY_VOLUME_MUTE = 0xAD,
		KEY_VOLUME_DOWN = 0xAE,
		KEY_VOLUME_UP = 0xAF,
		KEY_MEDIA_NEXT_TRACK = 0xB0,
		KEY_MEDIA_PREV_TRACK = 0xB1,
		KEY_MEDIA_STOP = 0xB2,
		KEY_MEDIA_PLAY_PAUSE = 0xB3,
		KEY_MEDIA_LAUNCH_MAIL = 0xB4,
		KEY_MEDIA_LAUNCH_MEDIA_SELECT = 0xB5,
		KEY_MEDIA_LAUNCH_APP1 = 0xB6,
		KEY_MEDIA_LAUNCH_APP2 = 0xB7,
		KEY_OEM_1 = 0xBA,
		KEY_OEM_PLUS = 0xBB,
		KEY_OEM_COMMA = 0xBC,
		KEY_OEM_MINUS = 0xBD,
		KEY_OEM_PERIOD = 0xBE,
		KEY_OEM_2 = 0xBF,
		KEY_OEM_3 = 0xC0,
		KEY_OEM_4 = 0xDB,
		KEY_OEM_5 = 0xDC,
		KEY_OEM_6 = 0xDD,
		KEY_OEM_7 = 0xDE,
		KEY_OEM_8 = 0xDF,
		KEY_OEM_102 = 0xE2,
		KEY_PROCESSKEY = 0xE5,
		KEY_PACKET = 0xE7,
		KEY_ATTN = 0xF6,
		KEY_CRSEL = 0xF7,
		KEY_EXSEL = 0xF8,
		KEY_EREOF = 0xF9,
		KEY_PLAY = 0xFA,
		KEY_ZOOM = 0xFB,
		KEY_NONAME = 0xFC,
		KEY_PA1 = 0xFD,
		KEY_OEM_CLEAR = 0xFE
	};

	/**
	 * @brief Interface for input subscriber classes, use this class if you want to be notified of input events by the input subsystem.
	 */
	class AEON_SUBSYSTEM_EXPORT IInputSubscriber
	{
	public:
		/**
		 * Called by the input subsystem when a key is first pressed. If the key is held down for an extended amount of time, this function will only be called the first time
		 * (i.e. at the moment this key is pressed down).
		 *
		 * @param key: The key that was pressed (to compare with actual values, use values from the Aeon::Key enum prefixed with KEY_).
		 * @param modifiers: The keyboard modifiers that were pressed at the time.
		 *
		 * @return Whether the key was handled or not.
		 */
		virtual bool OnKeyDown(Aeon::Key /*key*/, uint8_t /*modifiers*/) { return false;  }

		/**
		 * Called by the input subsystem when a key is released (after it was held down or pressed down).
		 *
		 * @param key: The key that was released (to compare with actual values, use values from the Aeon::Key enum prefixed with KEY_).
		 *
		 * @return Whether the event was handled or not.
		 */
		virtual bool OnKeyReleased(Aeon::Key /*key*/) { return false;  }

		/**
		 * Called when a mouse button is pressed down.
		 *
		 * @param button: The mouse button that was pressed down.
		 * @param modifiers: The keyboard modifiers that were pressed at the time.
		 *
		 * @return Whether the event was handled or not.
		 */
		virtual bool OnMouseDown(MouseButton /*button*/, uint8_t /*modifiers*/) { return false;  }

		/**
		 * Called when a mouse button is released.
		 *
		 * @param button: The mouse button that was pressed down.
		 * @param modifiers: The keyboard modifiers that were pressed at the time.
		 *
		 * @return Whether the event was handled or not.
		 */
		virtual bool OnMouseUp(MouseButton /*button*/, uint8_t /*modifiers*/) { return false; }

		/**
		 * Called when the mouse moves.
		 *
		 * @param deltaX: The amount the mouse moved horizontally (may be negative to indicate movement to the left).
		 * @param deltaY: The amount the mouse moved vertically (may be negative to indicate movement to the top).
		 *
		 * @return Whether the event was handled or not.
		 */
		virtual bool OnMouseMove(int /*deltaX*/, int /*deltaY*/) { return false; }

		/**
		 * Called when the (vertical) mouse wheel is scrolled.	
		 *
		 * @param delta: The amount the mouse was scrolled.
		 *
		 * @return Whether the event was handled or not.
		 */
		virtual bool OnMouseScroll(int /*delta*/) { return false; }
	};

	/**
	 * Interface for input subsystems.
	 */
	class AEON_SUBSYSTEM_EXPORT IInput : public ISubsystem
	{
	protected:
		/**
		 * List of subscribers.
		 */
		std::deque<IInputSubscriber *> subscriptions;

	public:
		/**
		 * Retrieves a bitset of the currently set keyboard modifiers.
		 */
		virtual uint8_t GetCurrentModifiers() const = 0;

		/**
		 * Indicates if the specified key is currently down (i.e. pressed).
		 *
		 * @note If you use this to check and then perform actions based on whether a key is down, you STILL have to return 'true' for that key
		 * in OnKeyDown and OnKeyReleased! Otherwise another input subscriber may also try to handle this key event.
		 */
		virtual bool IsKeyDown(Aeon::Key key) const = 0;

	public:
		/**
		 * Polls for input and notifies subscribers if necessary.
		 */
		virtual void Update() = 0;

	public:
		/**
		 * Retrieves the subsystem type.
		 */
		SubsystemType GetSubsystemType() const override
		{
			return SUBSYS_INPUT;
		}

		/**
		 * Lets the specified subscriber subscribe to the input subsystem. The subscriber will be notified when appropriate.
		 *
		 * @param s: The object subscribing to timer events.
		 */
		void Subscribe(IInputSubscriber *s)
		{
			if(s)
				subscriptions.push_back(s);
		}

		/**
		 * Lets the specified subscriber unsubscribe from the input subsystem. 
		 *
		 * @param s: The object unsubscribing from timer events.
		 */
		void Unsubscribe(IInputSubscriber *s)
		{
			auto it = std::find_if(subscriptions.begin(), subscriptions.end(), [s](const IInputSubscriber* ptr){
				return ptr == s;
			});

			if(it != subscriptions.end())
			{
				subscriptions[(it - subscriptions.begin())] = std::move(subscriptions.back());
				subscriptions.pop_back();
			}
		}
	};
}

#endif // IInput_h__
