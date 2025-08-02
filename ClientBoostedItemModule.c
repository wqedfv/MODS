// File created: 2025-07-30 19:10:52
// Author: wqedfv
// Client-side module for BoostedItemSystem

class ClientBoostedItemModule
{
    ref Timer m_ClientTimer;
    protected ref map<string, bool> m_ActiveEffects;
    static const float CLIENT_CHECK_INTERVAL = 2.0;

    void ClientBoostedItemModule()
    {
        Print("[BoostedItemSystem-Client] Клиентский модуль создан");
        ClientBoostedItemsConfig.Init();
        m_ActiveEffects = new map<string, bool>();
    }

    void OnInit()
    {
        Print("[BoostedItemSystem-Client] Клиентская инициализация начата");
        if (!m_ClientTimer)
        {
            m_ClientTimer = new Timer(CALL_CATEGORY_SYSTEM);
            m_ClientTimer.Run(CLIENT_CHECK_INTERVAL, this, "CheckClientEffects", null, true);
            Print("[BoostedItemSystem-Client] Клиентский таймер запущен");
        }
    }

    void CheckClientEffects()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.GetIdentity()) return;
        
        string playerID = player.GetIdentity().GetId();
        bool hasAnyEffect = false;
        array<string> activeStones = new array<string>();

        // Проверяем наличие камней в инвентаре
        for (int i = 0; i < ClientBoostedItemsConfig.ITEMS.Count(); i++)
        {
            string itemName = ClientBoostedItemsConfig.ITEMS.GetKey(i);
            ClientBoostEffect effect = ClientBoostedItemsConfig.ITEMS.GetElement(i);
            EntityAI foundItem = FindItem(player, itemName);
            
            if (foundItem)
            {
                hasAnyEffect = true;
                activeStones.Insert(itemName);
                Print("[BoostedItemSystem-Client] Активный камень: " + itemName);
            }
        }

        // Обновляем статус эффектов
        if (hasAnyEffect)
        {
            if (!m_ActiveEffects.Contains(playerID))
            {
                m_ActiveEffects.Insert(playerID, true);
                OnEffectsActivated(activeStones);
            }
            
            OnEffectsUpdate(activeStones);
        }
        else
        {
            if (m_ActiveEffects.Contains(playerID))
            {
                m_ActiveEffects.Remove(playerID);
                OnEffectsDeactivated();
            }
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

    // Методы для обработки клиентских эффектов
    void OnEffectsActivated(array<string> stones)
    {
        Print("[BoostedItemSystem-Client] Эффекты активированы: " + stones.Count().ToString() + " камней");
        // Здесь можно добавить клиентские эффекты: звуки, частицы, UI элементы
    }

    void OnEffectsUpdate(array<string> stones)
    {
        Print("[BoostedItemSystem-Client] Обновление эффектов: " + stones.Count().ToString() + " камней");
        // Здесь можно обновлять клиентские эффекты
    }

    void OnEffectsDeactivated()
    {
        Print("[BoostedItemSystem-Client] Эффекты деактивированы");
        // Здесь можно останавливать клиентские эффекты
    }
}