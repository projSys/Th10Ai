#include "Th10Hook/Common.h"
#include "Th10Hook/Th10Context.h"

namespace th
{
	using namespace boost::interprocess;

	Th10Context::Th10Context() :
		m_memory(open_only, "Th10SharedMemory"),
		m_data(nullptr)
	{
		m_data = m_memory.find<Th10SharedData>("Th10SharedData").first;
		if (m_data == nullptr)
			BASE_THROW(Exception(u8"Th10SharedDataÎ´ÕÒµ½¡£"));
	}

	Th10Context::~Th10Context()
	{
	}

	HWND Th10Context::getWindow() const
	{
		return m_data->window;
	}

	void Th10Context::notifyHook()
	{
		scoped_lock<interprocess_mutex> lock(m_data->hookMutex);
		m_data->isHooked = true;
		m_data->hookCond.notify_one();
	}

	void Th10Context::waitHook()
	{
		scoped_lock<interprocess_mutex> lock(m_data->hookMutex);
		if (!m_data->isHooked)
			m_data->hookCond.wait(lock);
	}

	void Th10Context::notifyUnhook()
	{
		scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
		m_data->isUnhook = true;
		m_data->unhookCond.notify_one();
	}

	void Th10Context::waitUnhook()
	{
		scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
		if (!m_data->isUnhook)
			m_data->unhookCond.wait(lock);
	}
}
