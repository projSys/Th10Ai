#pragma once

namespace th
{
	class Scene
	{
	public:
		static Pointi ToWindowPos(const Pointf& gamePos);
		static Pointf ToGamePos(const Pointi& windowPos);
		static bool IsInScene(const Pointf& pos);
	};
}
