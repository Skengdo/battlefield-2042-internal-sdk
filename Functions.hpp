#pragma once
#include "Classes.hpp"

namespace SDK
{
	namespace functions
	{
		template< size_t index, typename type, typename ...argument_list >
		type VirtualCall(void* instance, argument_list ...list) {
			using fn_t = type(__thiscall*)(void*, decltype(list) ...);
			auto fn = (*reinterpret_cast<fn_t**>(instance))[index];
			return fn(instance, list ...);
		}

		inline auto ExtractPos(D3DMATRIX Transform) -> utils::maths::vec3_t {
			return utils::maths::vec3_t(Transform._41, Transform._42, Transform._43);
		};

		bool WorldToScreen(uintptr_t pRenderView, utils::maths::vec3_t WorldPos, utils::maths::vec3_t* ScreenPos, float Width = ScreenMetrics.first, float Height = ScreenMetrics.second)
		{
			D3DXMATRIX ViewProjection = *reinterpret_cast<D3DXMATRIX*>(pRenderView + offsets::ViewProjection);

			float mX = Width * 0.5f;
			float mY = Height * 0.5f;

			float w = ViewProjection.m[0][3] * WorldPos.x + ViewProjection.m[1][3] * WorldPos.y + ViewProjection.m[2][3] * WorldPos.z + ViewProjection.m[3][3];

			if (w < 0.01)
				return false;

			ScreenPos->x = ViewProjection.m[0][0] * WorldPos.x + ViewProjection.m[1][0] * WorldPos.y + ViewProjection.m[2][0] * WorldPos.z + ViewProjection.m[3][0];
			ScreenPos->y = ViewProjection.m[0][1] * WorldPos.x + ViewProjection.m[1][1] * WorldPos.y + ViewProjection.m[2][1] * WorldPos.z + ViewProjection.m[3][1];

			float invw = 1.0f / w;

			ScreenPos->x *= invw;
			ScreenPos->y *= invw;

			float x = (float)Width / 2;
			float y = (float)Height / 2;

			x += 0.5 * ScreenPos->x * (float)Width + 0.5;
			y -= 0.5 * ScreenPos->y * (float)Height + 0.5;

			ScreenPos->x = x;
			ScreenPos->y = y;

			if (ScreenPos->x > Width || ScreenPos->y > Height || ScreenPos->x < 0 || ScreenPos->y < 0)
				return false;

			return true;
		}
	}
}
