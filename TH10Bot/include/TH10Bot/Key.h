#pragma once

namespace th
{
	class Key
	{
	public:
		// press后立刻up，up后立刻press，都会导致前一个输入失效，需要延时一段时间
		static void Delay(time_t ms = 16);

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
