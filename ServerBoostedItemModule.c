// File created: 2025-07-31 04:45:30
// Author: wqedfv
// Server-side module for BoostedItemSystem with quantity consumption

class ServerPlayerEffects
{
    float health;
    float blood;
    float water;
    float energy;
    float stamina;
    float shock;

    void ServerPlayerEffects()
    {
        health = 0;
        blood = 0;
        water = 0;
        energy = 0;
        stamina = 0;
        shock = 0;
    }
}

class ServerBoostedItemModule
{
    ref Timer m_BoostTimer;
    protected ref map<string, ref ServerPlayerEffects> m_PlayerEffects;
    protected ref map<string, bool> m_PlayerStaminaEffects;
    static const float EFFECT_TIMER_INTERVAL = 1.0;

    void ServerBoostedItemModule()
    {
        Print("[BoostedItemSystem-Server] Серверный модуль создан");
        ServerBoostedItemsConfig.Init();
        m_PlayerEffects = new map<string, ref ServerPlayerEffects>();
        m_PlayerStaminaEffects = new map<string, bool>();
    }

    void OnInit()
    {
        Print("[BoostedItemSystem-Server] Серверная инициализация начата");
        if (!m_BoostTimer)
        {
            m_BoostTimer = new Timer(CALL_CATEGORY_SYSTEM);
            m_BoostTimer.Run(EFFECT_TIMER_INTERVAL, this, "CheckBoostedItems", null, true);
            Print("[BoostedItemSystem-Server] Серверный таймер запущен");
        }
    }

    void CheckBoostedItems()
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity()) continue;
            
            string playerID = player.GetIdentity().GetId();
            ref ServerPlayerEffects totalEffects = new ServerPlayerEffects();
            bool hasAnyEffect = false;
            bool hasStaminaStone = false;
            array<EntityAI> foundItems = new array<EntityAI>();

            for (int i = 0; i < ServerBoostedItemsConfig.ITEMS.Count(); i++)
            {
                string itemName = ServerBoostedItemsConfig.ITEMS.GetKey(i);
                ServerBoostEffect effect = ServerBoostedItemsConfig.ITEMS.GetElement(i);
                EntityAI foundItem = FindItem(player, itemName);
                
                if (foundItem)
                {
                    // Проверяем quantity камня
                    float currentQuantity = foundItem.GetQuantity();
                    if (currentQuantity > 0)
                    {
                        hasAnyEffect = true;
                        foundItems.Insert(foundItem);
                        AddEffectToTotal(totalEffects, effect);
                        
                        if (effect.m_BoostType == "stamina")
                        {
                            hasStaminaStone = true;
                        }
                        
                        Print("[BoostedItemSystem-Server] Найден предмет: " + itemName + " (quantity: " + currentQuantity.ToString() + ")");
                    }
                    else
                    {
                        Print("[BoostedItemSystem-Server] Камень " + itemName + " полностью израсходован (quantity: 0)");
                    }
                }
            }

            if (hasAnyEffect)
            {
                if (!m_PlayerEffects.Contains(playerID))
                {
                    m_PlayerEffects.Insert(playerID, new ServerPlayerEffects());
                }
                
                if (hasStaminaStone)
                {
                    if (!m_PlayerStaminaEffects.Contains(playerID))
                    {
                        m_PlayerStaminaEffects.Insert(playerID, true);
                    }
                    ApplyInfiniteStamina(player);
                }
                else
                {
                    if (m_PlayerStaminaEffects.Contains(playerID))
                    {
                        m_PlayerStaminaEffects.Remove(playerID);
                    }
                }
                
                ApplyEffects(player, totalEffects, playerID, hasStaminaStone);
                
                // Расходуем quantity у активных камней
                if (foundItems.Count() > 0)
                {
                    foreach (EntityAI item : foundItems)
                    {
                        ConsumeItemQuantity(item);
                    }
                }
            }
            else
            {
                if (m_PlayerEffects.Contains(playerID))
                {
                    m_PlayerEffects.Remove(playerID);
                }
                
                if (m_PlayerStaminaEffects.Contains(playerID))
                {
                    m_PlayerStaminaEffects.Remove(playerID);
                }
            }
        }
    }

    void AddEffectToTotal(ServerPlayerEffects totalEffects, ServerBoostEffect effect)
    {
        Print("[BoostedItemSystem-Server] Добавление эффекта: " + effect.m_BoostType);
        
        string effectType = effect.m_BoostType;
        effectType.ToLower();
        
        switch (effectType)
        {
            case "health":
                totalEffects.health = totalEffects.health + effect.m_BoostValue;
                break;
            case "blood":
                totalEffects.blood = totalEffects.blood + effect.m_BoostValue;
                break;
            case "water":
                totalEffects.water = totalEffects.water + effect.m_BoostValue;
                break;
            case "energy":
                totalEffects.energy = totalEffects.energy + effect.m_BoostValue;
                break;
            case "stamina":
                totalEffects.stamina = totalEffects.stamina + effect.m_BoostValue;
                break;
            case "shock":
                totalEffects.shock = totalEffects.shock + effect.m_BoostValue;
                break;
            default:
                Print("[BoostedItemSystem-Server] ПРЕДУПРЕЖДЕНИЕ: Неизвестный тип эффекта: " + effect.m_BoostType);
                break;
        }
    }

    void ApplyInfiniteStamina(PlayerBase player)
    {
        if (player.GetStaminaHandler())
        {
            float maxStamina = player.GetStaminaHandler().GetStaminaMax();
            float currentStamina = player.GetStaminaHandler().GetStamina();
            
            if (currentStamina < maxStamina)
            {
                player.GetStaminaHandler().SetStamina(maxStamina);
                Print("[BoostedItemSystem-Server] Бесконечная стамина: " + currentStamina.ToString() + " -> " + maxStamina.ToString());
            }
        }
    }

    void ApplyEffects(PlayerBase player, ServerPlayerEffects effects, string playerID, bool hasStaminaStone)
    {
        // Применяем здоровье
        if (effects.health > 0)
        {
            float currentHealth = player.GetHealth("", "Health");
            float addValue = effects.health * ServerBoostedItemsConfig.SETTINGS.HEALTH_RATE;
            player.SetHealth("", "Health", Math.Min(player.GetMaxHealth("", "Health"), currentHealth + addValue));
            float newHealth = player.GetHealth("", "Health");
            Print("[BoostedItemSystem-Server] Здоровье: " + currentHealth.ToString() + " -> " + newHealth.ToString());
        }

        // Применяем кровь
        if (effects.blood > 0)
        {
            float currentBlood = player.GetHealth("", "Blood");
            float addBlood = effects.blood * ServerBoostedItemsConfig.SETTINGS.BLOOD_RATE;
            player.SetHealth("", "Blood", Math.Min(player.GetMaxHealth("", "Blood"), currentBlood + addBlood));
            float newBlood = player.GetHealth("", "Blood");
            Print("[BoostedItemSystem-Server] Кровь: " + currentBlood.ToString() + " -> " + newBlood.ToString());
        }

        // Применяем воду
        if (effects.water > 0 && player.GetStatWater())
        {
            float currentWater = player.GetStatWater().Get();
            float addWater = effects.water * ServerBoostedItemsConfig.SETTINGS.WATER_RATE;
            player.GetStatWater().Set(Math.Min(player.GetStatWater().GetMax(), currentWater + addWater));
            float newWater = player.GetStatWater().Get();
            Print("[BoostedItemSystem-Server] Вода: " + currentWater.ToString() + " -> " + newWater.ToString());
        }

        // Применяем энергию
        if (effects.energy > 0 && player.GetStatEnergy())
        {
            float currentEnergy = player.GetStatEnergy().Get();
            float addEnergy = effects.energy * ServerBoostedItemsConfig.SETTINGS.ENERGY_RATE;
            player.GetStatEnergy().Set(Math.Min(player.GetStatEnergy().GetMax(), currentEnergy + addEnergy));
            float newEnergy = player.GetStatEnergy().Get();
            Print("[BoostedItemSystem-Server] Энергия: " + currentEnergy.ToString() + " -> " + newEnergy.ToString());
        }

        // Применяем выносливость (только если НЕТ камня стамины)
        if (effects.stamina > 0 && !hasStaminaStone && player.GetStaminaHandler())
        {
            float currentStamina = player.GetStaminaHandler().GetStamina();
            float maxStamina = player.GetStaminaHandler().GetStaminaMax();
            float addStamina = effects.stamina * ServerBoostedItemsConfig.SETTINGS.STAMINA_RATE;
            player.GetStaminaHandler().SetStamina(Math.Min(maxStamina, currentStamina + addStamina));
            float newStamina = player.GetStaminaHandler().GetStamina();
            Print("[BoostedItemSystem-Server] Выносливость: " + currentStamina.ToString() + " -> " + newStamina.ToString());
        }

        // Применяем шок
        if (effects.shock > 0)
        {
            float currentShock = player.GetHealth("", "Shock");
            float addShock = effects.shock * ServerBoostedItemsConfig.SETTINGS.SHOCK_RATE;
            player.SetHealth("", "Shock", Math.Min(player.GetMaxHealth("", "Shock"), currentShock + addShock));
            float newShock = player.GetHealth("", "Shock");
            Print("[BoostedItemSystem-Server] Шок: " + currentShock.ToString() + " -> " + newShock.ToString());
        }
    }

    EntityAI FindItem(PlayerBase player, string itemName)
    {
        array<EntityAI> items = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        
        foreach (EntityAI item : items)
        {
            if (item && item.GetType() == itemName)
                return item;
        }
        return null;
    }

    void ConsumeItemQuantity(EntityAI item)
    {
        if (!item) return;

        ServerBoostEffect effect = ServerBoostedItemsConfig.ITEMS.Get(item.GetType());
        if (effect && effect.m_ConsumptionRate > 0)
        {
            float currentQuantity = item.GetQuantity();
            if (currentQuantity <= 0) return;
            
            // Вычисляем расход quantity
            float consumption = effect.m_ConsumptionRate * ServerBoostedItemsConfig.SETTINGS.CONSUMPTION_RATE_MULTIPLIER;
            float newQuantity = Math.Max(0.0, currentQuantity - consumption);
            
            item.SetQuantity(newQuantity);
            
            // Логируем расход каждые 10% или когда quantity становится низким
            int quantityPercent = Math.Floor((newQuantity / item.GetQuantityMax()) * 100);
            
            if (quantityPercent % ServerBoostedItemsConfig.SETTINGS.CONSUMPTION_LOG_THRESHOLD == 0 || newQuantity <= 10)
            {
                Print("[BoostedItemSystem-Server] Расход камня: " + item.GetType() + " quantity: " + newQuantity.ToString() + " (" + quantityPercent.ToString() + "%)");
            }
            
            // Если quantity достигло 0, предмет израсходован
            if (newQuantity <= 0)
            {
                Print("[BoostedItemSystem-Server] КАМЕНЬ ИЗРАСХОДОВАН: " + item.GetType() + " полностью использован!");
            }
        }
    }
}