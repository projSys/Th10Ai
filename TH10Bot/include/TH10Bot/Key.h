#pragma once

namespace th
{
	class Key
	{
	public:
		explicit Key(WORD vkey);
		~Key();

		void press();
		void release();
		bool isPressed() const;

	private:
		WORD m_vkey;
		WORD m_scan;
		bool m_pressed;
	};
}
