// File created: 2025-07-31 04:45:30
// Author: wqedfv
// Client-side configuration for BoostedItemSystem with quantity consumption

class ClientBoostEffect
{
    string m_BoostType;
    float m_BoostValue;
    float m_ConsumptionRate;

    void ClientBoostEffect(string type, float value, float consumptionRate = 0.1)
    {
        m_BoostType = type;
        m_BoostValue = value;
        m_ConsumptionRate = consumptionRate;
    }
}

class ClientSystemSettings
{
    bool SHOW_EFFECTS;
    bool PLAY_SOUNDS;
    bool SHOW_PARTICLES;
    float UI_UPDATE_INTERVAL;
    
    void ClientSystemSettings()
    {
        SHOW_EFFECTS = true;
        PLAY_SOUNDS = true;
        SHOW_PARTICLES = false;
        UI_UPDATE_INTERVAL = 1.0;
    }
}

class ClientBoostedItemsConfig
{
    static const string CONFIG_DIR = "$profile:A1_MOD_CLIENT";
    static const string CONFIG_PATH = "$profile:A1_MOD_CLIENT\\BoostedItemSystem_Client.json";
    
    static ref map<string, ref ClientBoostEffect> ITEMS;
    static ref ClientSystemSettings SETTINGS;
    
    static void Init()
    {
        Print("[BoostedItemSystem-Client] Инициализация клиентского конфига с расходом quantity, версия от 2025-07-31 04:45:30");
        
        if (!ITEMS)
        {
            ITEMS = new map<string, ref ClientBoostEffect>();
        }
            
        if (!SETTINGS)
        {
            SETTINGS = new ClientSystemSettings();
        }
        
        EnsureDirectoryExists();
        
        bool configExists = FileExist(CONFIG_PATH);
        
        if (!configExists)
        {
            Print("[BoostedItemSystem-Client] Клиентский конфиг не найден, создаем новый...");
            SetDefaultConfig();
            SaveJsonConfig();
        }
        else
        {
            if (!LoadConfig())
            {
                Print("[BoostedItemSystem-Client] Не удалось загрузить клиентский конфиг, создаем новый...");
                SetDefaultConfig();
                SaveJsonConfig();
            }
        }
        
        Print("[BoostedItemSystem-Client] Всего загружено предметов: " + ITEMS.Count().ToString());
        LogConfigInfo();
    }
    
    static void LogConfigInfo()
    {
        Print("[BoostedItemSystem-Client] Клиентские настройки:");
        Print("[BoostedItemSystem-Client] SHOW_EFFECTS: " + SETTINGS.SHOW_EFFECTS.ToString());
        Print("[BoostedItemSystem-Client] PLAY_SOUNDS: " + SETTINGS.PLAY_SOUNDS.ToString());
        Print("[BoostedItemSystem-Client] SHOW_PARTICLES: " + SETTINGS.SHOW_PARTICLES.ToString());
        Print("[BoostedItemSystem-Client] UI_UPDATE_INTERVAL: " + SETTINGS.UI_UPDATE_INTERVAL.ToString());
        
        Print("[BoostedItemSystem-Client] Камни с расходом quantity:");
        
        foreach (string itemName, ClientBoostEffect effect : ITEMS)
        {
            Print("[BoostedItemSystem-Client] Предмет: " + itemName + " | Тип: " + effect.m_BoostType + " | Расход: " + effect.m_ConsumptionRate.ToString() + "%/сек");
        }
    }
    
    static private void EnsureDirectoryExists()
    {
        if (!FileExist(CONFIG_DIR))
        {
            Print("[BoostedItemSystem-Client] Создание клиентской директории: " + CONFIG_DIR);
            
            if (MakeDirectory(CONFIG_DIR))
            {
                Print("[BoostedItemSystem-Client] Клиентская директория успешно создана: " + CONFIG_DIR);
                
                string testFilePath = CONFIG_DIR + "\\test_client.txt";
                FileHandle testFile = OpenFile(testFilePath, FileMode.WRITE);
                if (testFile != 0)
                {
                    FPrintln(testFile, "Клиентская директория создана: " + GetDate());
                    CloseFile(testFile);
                    Print("[BoostedItemSystem-Client] Клиентская директория подтверждена: " + CONFIG_DIR);
                }
                else
                {
                    Print("[BoostedItemSystem-Client] ОШИБКА: Не удалось создать тестовый файл");
                }
            }
            else
            {
                Print("[BoostedItemSystem-Client] ОШИБКА: Не удалось создать клиентскую директорию");
            }
        }
        else
        {
            Print("[BoostedItemSystem-Client] Клиентская директория уже существует: " + CONFIG_DIR);
        }
    }
    
    static private string GetDate()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        
        string result = year.ToString() + "-" + month.ToString() + "-" + day.ToString();
        return result;
    }

    static private bool LoadConfig()
    {
        Print("[BoostedItemSystem-Client] Загрузка клиентского конфига из: " + CONFIG_PATH);
        
        ITEMS.Clear();
        SetDefaultConfig();
        
        Print("[BoostedItemSystem-Client] Загружено предметов: " + ITEMS.Count().ToString());
        return true;
    }

    static private void SaveJsonConfig()
    {
        string json = "{\n";
        string settingsJson = CreateSettingsJson();
        json = json + settingsJson;
        
        if (ITEMS.Count() > 0)
        {
            json = json + ",\n";
            string itemsJson = CreateItemsJson();
            json = json + itemsJson;
        }
        
        json = json + "\n}";
        
        if (!FileExist(CONFIG_DIR))
        {
            MakeDirectory(CONFIG_DIR);
        }
        
        FileHandle file = OpenFile(CONFIG_PATH, FileMode.WRITE);
        
        if (file != 0)
        {
            FPrintln(file, json);
            CloseFile(file);
            Print("[BoostedItemSystem-Client] Клиентский конфиг сохранен: " + CONFIG_PATH);
        }
        else
        {
            Print("[BoostedItemSystem-Client] ОШИБКА: Не удалось сохранить клиентский конфиг");
        }
    }
    
    static private string CreateSettingsJson()
    {
        string settingsJson = "    \"ClientSettings\": {\n";
        settingsJson = settingsJson + "        \"SHOW_EFFECTS\": " + SETTINGS.SHOW_EFFECTS.ToString() + ",\n";
        settingsJson = settingsJson + "        \"PLAY_SOUNDS\": " + SETTINGS.PLAY_SOUNDS.ToString() + ",\n";
        settingsJson = settingsJson + "        \"SHOW_PARTICLES\": " + SETTINGS.SHOW_PARTICLES.ToString() + ",\n";
        settingsJson = settingsJson + "        \"UI_UPDATE_INTERVAL\": " + SETTINGS.UI_UPDATE_INTERVAL.ToString() + "\n";
        settingsJson = settingsJson + "    }";
        
        return settingsJson;
    }
    
    static private string CreateItemsJson()
    {
        string itemsJson = "    \"ClientItems\": {\n";
        int count = 0;
        int total = ITEMS.Count();
        
        foreach (string itemName, ClientBoostEffect effect : ITEMS)
        {
            count = count + 1;
            
            string itemJson = "        \"" + itemName + "\": {\n";
            itemJson = itemJson + "            \"Type\": \"" + effect.m_BoostType + "\",\n";
            itemJson = itemJson + "            \"Value\": " + effect.m_BoostValue.ToString() + ",\n";
            itemJson = itemJson + "            \"ConsumptionRate\": " + effect.m_ConsumptionRate.ToString() + "\n";
            
            if (count < total)
            {
                itemJson = itemJson + "        },\n";
            }
            else
            {
                itemJson = itemJson + "        }\n";
            }
            
            itemsJson = itemsJson + itemJson;
        }
        
        itemsJson = itemsJson + "    }";
        return itemsJson;
    }

    static private void SetDefaultConfig()
    {
        Print("[BoostedItemSystem-Client] Создание клиентского конфига с расходом quantity");
        
        ITEMS.Clear();
        
        // Клиентские настройки камней с расходом quantity
        ITEMS.Insert("A1_StoneHealth", new ClientBoostEffect("health", 15.0, 0.05));
        ITEMS.Insert("A1_StoneBlood", new ClientBoostEffect("blood", 800.0, 0.08));
        ITEMS.Insert("A1_StoneStamina", new ClientBoostEffect("stamina", 999.0, 0.03));
        ITEMS.Insert("A1_StoneWater", new ClientBoostEffect("water", 1500.0, 0.06));
        ITEMS.Insert("A1_StoneEnergy", new ClientBoostEffect("energy", 1500.0, 0.06));
        ITEMS.Insert("A1_StoneShock", new ClientBoostEffect("shock", 300.0, 0.10));
    }

    static void ReloadConfig()
    {
        Print("[BoostedItemSystem-Client] Перезагрузка клиентского конфига");
        Init();
    }
}