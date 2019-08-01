#pragma once

namespace base
{
	class Process
	{
	public:
		static const DWORD INVALID_ID = 0xFFFFFFFF;

		static DWORD FindIdByName(const std::string& name);

		Process();
		explicit Process(DWORD id, DWORD desiredAccess = PROCESS_ALL_ACCESS,
			BOOL inheritHandle = FALSE);
		Process(const Process&) = delete;
		Process(Process&& other);
		~Process();
		Process& operator =(const Process&) = delete;
		Process& operator =(Process&& other);
		void swap(Process& other);

		HMODULE findModuleByName(const std::string& moduleName) const;

		// 判断进程是不是运行在WOW64虚拟环境下
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
