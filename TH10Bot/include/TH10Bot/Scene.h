#pragma once

namespace th
{
	class Scene
	{
	public:
		static Pointi ToWindowPos(const Pointf& scenePos);
		static Pointf ToScenePos(const Pointi& windowPos);
		static bool IsInScene(const Pointf& pos);
	};
}
