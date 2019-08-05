#include "GraphCap/Common.h"
#include "GraphCap/DI8Input.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <boost/interprocess/sync/scoped_lock.hpp>

namespace gc
{
	const BYTE DI8Input::KEY_MAP[KEY_MAXCOUNT] =
	{
		0,

		DIK_ESCAPE,

		DIK_F1, DIK_F2, DIK_F3, DIK_F4, DIK_F5, DIK_F6, DIK_F7, DIK_F8, DIK_F9, DIK_F10,
		DIK_F11, DIK_F12,

		DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, DIK_6, DIK_7, DIK_8, DIK_9, DIK_0,

		DIK_Q, DIK_W, DIK_E, DIK_R, DIK_T, DIK_Y, DIK_U, DIK_I, DIK_O, DIK_P,
		DIK_A, DIK_S, DIK_D, DIK_F, DIK_G, DIK_H, DIK_J, DIK_K, DIK_L,
		DIK_Z, DIK_X, DIK_C, DIK_V, DIK_B, DIK_N, DIK_M,

		DIK_LSHIFT, DIK_RSHIFT, DIK_LCONTROL, DIK_RCONTROL, DIK_LMENU, DIK_RMENU,

		DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT
	};

	DI8Input::DI8Input()
	{
		m_memory = bip::managed_shared_memory(bip::open_only, "DI8SharedMemory");
		m_data = m_memory.find<DI8SharedData>("DI8SharedData").first;
		if (m_data == nullptr)
			THROW_BASE_EXCEPTION(Exception() << err_str("DI8SharedData not found."));

		clear();
	}

	void DI8Input::clear()
	{
		memset(m_keyState, KS_NONE, sizeof(m_keyState));
	}

	void DI8Input::keyClear(Key key)
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		m_keyState[KEY_MAP[key]] = KS_NONE;
	}

	void DI8Input::keyPress(Key key)
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		m_keyState[KEY_MAP[key]] = KS_PRESS;
	}

	void DI8Input::keyRelease(Key key)
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		m_keyState[KEY_MAP[key]] = KS_RELEASE;
	}

	bool DI8Input::isKeyPressed(Key key) const
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		return m_keyState[KEY_MAP[key]] == KS_PRESS;
	}

	void DI8Input::commit()
	{
		bip::scoped_lock<bip::interprocess_mutex> lock(m_data->keyMutex);
		memcpy_s(m_data->keyState, sizeof(m_data->keyState), m_keyState, sizeof(m_keyState));
	}
}
