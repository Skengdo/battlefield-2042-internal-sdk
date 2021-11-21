#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <cstdint>
#include <d3dx10math.h>

#include "Vector.hpp"
#include "Offsets.hpp"
#include "Enums.hpp"
#include "Functions.hpp"

using namespace SDK;

std::pair<float, float> ScreenMetrics;
std::uintptr_t GameModule;

#define AFIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))

inline auto IsValidPtr(void* const pointer) -> bool { 
	return (pointer && pointer > (void*)0xFFFFFF && pointer < (void*)0x7FFFFFFFFFFF);
}

namespace SDK
{
#define MAX_PLAYER_COUNT 128
#define MAX_ENTITY_COUNT 1024

	void InitSDK()
	{
		ScreenMetrics = std::make_pair(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		GameModule = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));
	}

	template <typename TElement>
	class cArray
	{
	public:
		inline auto get_count() -> size_t
		{
			return this->_size;
		}

		inline auto get_max_size() -> size_t
		{
			return this->_maxsize;
		}

		inline auto get_buffer() -> TElement 
		{
			return _buffer;
		}

		inline auto get_by_idx(int _index) -> TElement
		{
			return _buffer[_index];
		}

	private:
		char pad_0x01[0x10];
		size_t _size;
		size_t _maxsize;
		TElement _buffer[];
	};

	class cString
	{
	public:
		inline auto ptr() -> bool
		{
			return IsValidPtr((void*)_pbuffer);
		}

		inline auto c_str() -> const char*
		{
			if (this->ptr())
				return this->_pbuffer;

			return this->_buffer;
		}

		inline auto to_str() -> std::string
		{
			if (this->ptr())
				return std::string(this->_pbuffer);

			return std::string(this->_buffer);
		}

	private:
		union {
			char* _pbuffer;
			char _buffer[16];
		};
	};

	class cGameRenderer
	{
	public:
		auto GetRenderView() -> uintptr_t
		{
			return *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(this) + offsets::RenderView);
		}

		static auto GetInstance(uintptr_t Base = GameModule) -> cGameRenderer*
		{
			return *reinterpret_cast<cGameRenderer**>(Base + offsets::GameRenderer);
		}
	};

	class cHealthComponent
	{
	public:
		auto GetHealth() -> float
		{
			return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::Health);
		}

		auto GetMaxHealth() -> float
		{
			return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::MaxHealth);
		}
	};

	class cWeapon
	{
	public:
		auto GetName() -> cString
		{
			return *reinterpret_cast<cString*>(reinterpret_cast<uintptr_t>(this) + offsets::WeaponName);
		}
	};

	class cActiveWeapon
	{
	public:
		auto GetWeapon() -> cWeapon*
		{
			return *reinterpret_cast<cWeapon**>(reinterpret_cast<uintptr_t>(this) + offsets::Weapon);
		}

		auto GetActiveSlot() -> int
		{
			return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::ActiveSlot);
		}
	};

	class cWeaponComponent
	{
	public:
		auto GetWeapons() -> cArray<cWeapon*>*
		{
			return *reinterpret_cast<cArray<cWeapon*>**>(reinterpret_cast<uintptr_t>(this) + offsets::WeaponArray);
		}

		auto GetActiveWeapon() -> cActiveWeapon*
		{
			return *reinterpret_cast<cActiveWeapon**>(reinterpret_cast<uintptr_t>(this) + offsets::ActiveWeapon);
		}
	};

	class cClEntity
	{
	public:
		auto GetTransformInternal(D3DMATRIX* Transform)
		{
			return functions::VirtualCall<offsets::GetTransform, void>(this, Transform);
		}

		auto GetTransformAABBInternal(D3DMATRIX* Transform)
		{
			return functions::VirtualCall<offsets::GetTransformAABB, void>(this, Transform);
		}
	};

	class cSoldierEntity : public cClEntity
	{
	public:
		auto GetPosition() -> utils::maths::vec3_t
		{
			return *reinterpret_cast<utils::maths::vec3_t*>(reinterpret_cast<uintptr_t>(this) + offsets::RootPosition);
		}

		auto GetHealthComponent() -> cHealthComponent*
		{
			return *reinterpret_cast<cHealthComponent**>(reinterpret_cast<uintptr_t>(this) + offsets::HealthComponent);
		}

		auto GetWeaponComponent() -> cWeaponComponent*
		{
			return *reinterpret_cast<cWeaponComponent**>(reinterpret_cast<uintptr_t>(this) + offsets::WeaponComponent);
		}

		auto GetActiveWeaponName() -> const char*
		{
			const auto WeaponComponent = this->GetWeaponComponent();

			if (!IsValidPtr((void*)WeaponComponent))
				return nullptr;

			const auto ActiveWeapon = WeaponComponent->GetActiveWeapon();

			if (!IsValidPtr((void*)ActiveWeapon))
				return nullptr;

			const auto Weapon = ActiveWeapon->GetWeapon();

			if (!IsValidPtr((void*)Weapon))
				return nullptr;

			const auto WeaponName = Weapon->GetName().c_str();

			if (!IsValidPtr((void*)WeaponName))
				return nullptr;
	
			return WeaponName;
		}

		auto IsVisible() -> bool
		{
			return !*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + offsets::IsHidden);
		}

		auto GetYawAngle() -> float
		{
			return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets::YawAngle);
		}
	};

	class cClientEntity
	{
	public:

	};

	class cClientPlayer : public cClientEntity
	{
	public:
		auto GetName() -> cString
		{
			return *reinterpret_cast<cString*>(reinterpret_cast<uintptr_t>(this) + offsets::PlayerName);
		}

		auto GetTeamId() -> int
		{
			return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + offsets::PlayerTeamId);
		}

		auto GetSoldierEntity() -> cSoldierEntity*
		{
			return *reinterpret_cast<cSoldierEntity**>(reinterpret_cast<uintptr_t>(this) + offsets::PlayerSoldierEntity);
		}
	};

	class cVehicleData
	{
	public:
		auto GetNameSid() -> cString
		{
			return *reinterpret_cast<cString*>(reinterpret_cast<uintptr_t>(this) + offsets::VehicleNameSid);
		}
	};

	class cVehicleEntity : public cClEntity
	{
	public:
		auto GetPosition(utils::maths::vec3_t* Position, bool Internal = true) -> bool
		{
			D3DMATRIX Transform{};

			if (!Internal)
			{
				auto pCollection =
					*reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(this) + offsets::TransformCollection);

				if (!IsValidPtr((void*)pCollection))
					return false;

				int8_t _9 = *reinterpret_cast<int8_t*>(pCollection + 9);
				int8_t _10 = *reinterpret_cast<int8_t*>(pCollection + 10);

				uintptr_t ComponentCollectionOffset = 0x20 * (_9 + (2 * _10));

				Transform = *reinterpret_cast<D3DMATRIX*>(pCollection + ComponentCollectionOffset + 0x10);
			}
			else
			{
				this->GetTransformAABBInternal(&Transform);
			}

			*Position = functions::ExtractPos(Transform);

			return true;
		}

		auto GetVehicleData() -> cVehicleData*
		{
			return *reinterpret_cast<cVehicleData**>(reinterpret_cast<uintptr_t>(this) + offsets::VehicleData);
		}
	};

	class cClientVehicle : public cClientEntity
	{
	public:
		auto GetVehicleEntity() -> cVehicleEntity*
		{
			return *reinterpret_cast<cVehicleEntity**>(reinterpret_cast<uintptr_t>(this) + offsets::VehicleEntity);
		}
	};

	class cLocalPlayer : public cClientPlayer
	{
	public:

	};

	class cClientPlayerMgr
	{
	public:
		auto GetLocalPlayerArray() -> uintptr_t*
		{
			return *reinterpret_cast<uintptr_t**>(reinterpret_cast<uintptr_t>(this) + offsets::LocalPlayerArray);
		}

		auto GetClientPlayersArray() -> cArray<cClientEntity*>*
		{
			return *reinterpret_cast<cArray<cClientEntity*>**>(reinterpret_cast<uintptr_t>(this) + offsets::ClientPlayerArray);
		}

		auto GetLocalPlayer() -> cLocalPlayer*
		{
			auto lpArray = this->GetLocalPlayerArray();

			if (!IsValidPtr((void*)lpArray))
				return nullptr;

			return (cLocalPlayer*)lpArray[0];
		}

		auto GetAllClientPlayers(cArray<cClientEntity*>* const clpArray, cLocalPlayer* const lPlayer) -> std::vector<cClientEntity*>
		{
			std::vector<cClientEntity*> clBuffer;

			for (int i = 0; i < MAX_PLAYER_COUNT; i++)
			{
				const auto ClientPlayer = clpArray->get_by_idx(i);

				if (!IsValidPtr((void*)ClientPlayer))
					continue;

				if ((uintptr_t)ClientPlayer == (uintptr_t)lPlayer)
					continue;

				clBuffer.push_back(ClientPlayer);
			}

			return clBuffer;
		}
	};

	class cGameContext
	{
	public:
		auto GetClientPlayerMgr() -> cClientPlayerMgr*
		{
			return *reinterpret_cast<cClientPlayerMgr**>(reinterpret_cast<uintptr_t>(this) + offsets::ClientPlayerManager);
		}

		static auto GetInstance(uintptr_t Base = GameModule) -> cGameContext*
		{
			return *reinterpret_cast<cGameContext**>(Base + offsets::GameContext);
		}
	};
}