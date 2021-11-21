#pragma once

#include "Classes.hpp"

namespace SDK
{
	namespace offsets
	{
		// Renderer
		static constexpr auto GameRenderer = 0x62CF960;
		static constexpr auto RenderView = 0x60;
		static constexpr auto ViewProjection = 0x480;

		// Game
		static constexpr auto GameContext = 0x621BE48;
		static constexpr auto ClientPlayerManager = 0xA8;
		static constexpr auto ClientPlayerArray = 0x570;
		static constexpr auto LocalPlayerArray = 0x560;

		// Entity
		static constexpr auto PlayerSoldierEntity = 0xC8;
		static constexpr auto VehicleEntity = 0xB8;

		// Player
		static constexpr auto PlayerName = 0x20;
		static constexpr auto PlayerTeamId = 0x60;

		// Entity
		static constexpr auto GetTransform = 27; /* VTable Index */
		static constexpr auto GetTransformAABB = 26; /* VTable Index */

		// SoldierEntity
		static constexpr auto WeaponComponent = 0x1590;
		static constexpr auto HealthComponent = 0x1C60;
		static constexpr auto RootPosition = 0x20E0;
		static constexpr auto IsHidden = 0x2A0;
		static constexpr auto YawAngle = 0x1318;

		// VehicleEntity
		static constexpr auto TransformCollection = 0x60;
		static constexpr auto VehicleData = 0x38;
		static constexpr auto VehicleNameSid = 0x280;

		// HealthComponent
		static constexpr auto Health = 0x40;
		static constexpr auto MaxHealth = 0x44;

		// WeaponComponent
		static constexpr auto WeaponArray = 0x4A70;
		static constexpr auto ActiveWeapon = 0x4AB8;

		// ActiveWeapon
		static constexpr auto Weapon = 0x38;
		static constexpr auto ActiveSlot = 0x40;

		// Weapon
		static constexpr auto WeaponName = 0x1D0;
	}
}