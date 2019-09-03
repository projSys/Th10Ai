#pragma once

namespace gc
{
	class FrameListener
	{
	public:
		virtual ~FrameListener() = default;

		virtual void onPresentBegin(bool waited);
		virtual void onPresentEnd(bool waited);
	};
}
