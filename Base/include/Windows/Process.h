#pragma once

namespace win
{
	class Process
	{
	public:
		static const DWORD INVALID_ID = 0xFFFFFFFF;

		static Process FindByName(const std::string& name,
			DWORD desiredAccess = PROCESS_ALL_ACCESS, BOOL inheritHandle = FALSE);

		Process();
		explicit Process(DWORD id, DWORD desiredAccess = PROCESS_ALL_ACCESS,
			BOOL inheritHandle = FALSE);
		~Process();
		Process(const Process&) = delete;
		Process(Process&& other);
		Process& operator =(const Process&) = delete;
		Process& operator =(Process&& other);
		void swap(Process& other);

		HMODULE findModuleByName(const std::string& moduleName) const;

		// �жϽ����ǲ���������WOW64���⻷����
		// 64-bit process on 64-bit Windows : FALSE
		// 32-bit process on 64-bit Windows : TRUE
		// 32-bit process on 32-bit Windows : FALSE
		bool isWow64() const;
		bool isActive() const;

		operator HANDLE() const;
		DWORD getId() const;

	private:
		HANDLE m_process;
		DWORD m_id;
	};
}
