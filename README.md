# battlefield-2042-internal-sdk
Internal Software Development Kit for Battlefield 2042

SDK Includes the following:

- Entity Classes
- Player Classes
- Vehicle Classes
- Weapon Classes
- GameContext Class
- GameRenderer Class
- ClientPlayerManager Class
- WorldToScreen Utility
- Other Useful Utilities

Note: SDK may have issues as it is not fully complete yet, I am still working on it. Entity list iteration is kind of broken as I cannot find the array count to properly iterate it

Usage:

```cpp
	const auto GameRenderer = cGameRenderer::GetInstance();

	if (!IsValidPtr((void*)GameRenderer))
		return;

	const auto RenderView = GameRenderer->GetRenderView();

	if (!IsValidPtr((void*)RenderView))
		return;

	const auto GameContext = cGameContext::GetInstance();

	if (!IsValidPtr((void*)GameContext))
		return;

	const auto ClientPlayerManager = GameContext->GetClientPlayerMgr();

	if (!IsValidPtr((void*)ClientPlayerManager))
		return;

	const auto LocalPlayer = ClientPlayerManager->GetLocalPlayer();

	if (!IsValidPtr((void*)LocalPlayer))
		return;

	const auto lpTeamId = LocalPlayer->GetTeamId();

	const auto lpSoldierEntity = LocalPlayer->GetSoldierEntity();

	if (!IsValidPtr((void*)lpSoldierEntity))
		return;

	const auto lpPosition = lpSoldierEntity->GetPosition();

	if (lpPosition.empty())
		return;

	const auto ClientPlayerArray = ClientPlayerManager->GetClientPlayersArray();

	if (!IsValidPtr((void*)ClientPlayerArray))
		return;

	auto IsSoldier = [](cClientEntity* const clPlayer) -> bool {
		return IsValidPtr((void*)static_cast<cClientPlayer*>(clPlayer)->GetSoldierEntity());
	};

	auto IsVehicle = [](cClientEntity* const clPlayer) -> bool {
		return IsValidPtr((void*)static_cast<cClientVehicle*>(clPlayer)->GetVehicleEntity());
	};

	for (const auto ClientPlayer : 
		ClientPlayerManager->GetAllClientPlayers(ClientPlayerArray, LocalPlayer))
	{
		if (IsSoldier(ClientPlayer))
		{
			const auto SoldierEntity = static_cast<cClientPlayer*>(ClientPlayer)->GetSoldierEntity();

			if (!IsValidPtr((void*)SoldierEntity))
				continue;

			const auto TeamId = static_cast<cClientPlayer*>(ClientPlayer)->GetTeamId();

			if (TeamId == lpTeamId)
				continue;

			const auto HealthComponent = SoldierEntity->GetHealthComponent();

			if (!IsValidPtr((void*)HealthComponent))
				continue;

			auto Health = HealthComponent->GetHealth();
			auto MaxHealth = HealthComponent->GetMaxHealth();

			if (Health <= 0.f || MaxHealth <= 0.f)
				continue;

			auto PlayerName = static_cast<cClientPlayer*>(ClientPlayer)->GetName().c_str();

			const auto CurrentPosition = SoldierEntity->GetPosition();

			if (CurrentPosition.empty())
				continue;

			auto Distance = lpPosition.distance(CurrentPosition);

			utils::maths::vec3_t ScreenPosition;
			if (!functions::WorldToScreen(RenderView, CurrentPosition, &ScreenPosition))
				continue;

			if (ConfigSys.esp_player)
			{
				rendering::Renderer::GetInstance()->DrawNameTag(PlayerName,
					{ ScreenPosition.x, ScreenPosition.y + 5.f },
					{ 139.f, 69.f, 19.f, 255.f },
					Distance,
					ConfigSys.font_size,
					true,
					Health,
					MaxHealth);
			}
		}

		if (IsVehicle(ClientPlayer))
		{
			const auto VehicleEnity = static_cast<cClientVehicle*>(ClientPlayer)->GetVehicleEntity();

			if (!IsValidPtr((void*)VehicleEnity))
				continue;

			utils::maths::vec3_t CurrentPosition;
			if (!VehicleEnity->GetPosition(&CurrentPosition))
				continue;

			if (CurrentPosition.empty())
				continue;

			utils::maths::vec3_t ScreenPosition;
			if (!functions::WorldToScreen(RenderView, CurrentPosition, &ScreenPosition))
				continue;
        
			auto Distance = lpPosition.distance(CurrentPosition);

			if (ConfigSys.esp_player)
			{
				rendering::Renderer::GetInstance()->DrawNameTag("Vehicle",
					{ ScreenPosition.x, ScreenPosition.y + 5.f },
					{ 119.f, 80.f, 19.f, 255.f },
					Distance,
					ConfigSys.font_size,
					false);
			}
		}
	}  
  ```

