#pragma once

namespace th
{
	class Key
	{
	public:
		// press������up��up������press�����ᵼ��ǰһ������ʧЧ����Ҫ��ʱһ��ʱ��
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
