// File created: 2025-07-31 04:45:30
// Author: wqedfv
// Server-side configuration for BoostedItemSystem with quantity consumption

class ServerBoostEffect
{
    string m_BoostType;
    float m_BoostValue;
    float m_ConsumptionRate;
    ref array<ref ServerBoostEffect> m_AdditionalEffects;

    void ServerBoostEffect(string type, float value, float consumptionRate = 0.1)
    {
        m_BoostType = type;
        m_BoostValue = value;
        m_ConsumptionRate = consumptionRate;
        m_AdditionalEffects = new array<ref ServerBoostEffect>();
    }

    void AddEffect(string type, float value)
    {
        m_AdditionalEffects.Insert(new ServerBoostEffect(type, value));
    }
}

class ServerSystemSettings
{
    float HEALTH_RATE;
    float BLOOD_RATE;
    float WATER_RATE;
    float ENERGY_RATE;
    float STAMINA_RATE;
    float SHOCK_RATE;
    
    float CONSUMPTION_RATE_MULTIPLIER;
    int CONSUMPTION_LOG_THRESHOLD;
    
    void ServerSystemSettings()
    {
        HEALTH_RATE = 0.1;
        BLOOD_RATE = 0.1;
        WATER_RATE = 0.1;
        ENERGY_RATE = 0.1;
        STAMINA_RATE = 0.5;
        SHOCK_RATE = 0.2;
        
        CONSUMPTION_RATE_MULTIPLIER = 1.0;
        CONSUMPTION_LOG_THRESHOLD = 10;
    }
}

class ServerBoostedItemsConfig
{
    static const string CONFIG_DIR = "$profile:A1_MOD_SERVER";
    static const string CONFIG_PATH = "$profile:A1_MOD_SERVER\\BoostedItemSystem_Server.json";
    
    static ref map<string, ref ServerBoostEffect> ITEMS;
    static ref ServerSystemSettings SETTINGS;
    
    static void Init()
    {
        Print("[BoostedItemSystem-Server] Инициализация серверного конфига с расходом quantity, версия от 2025-07-31 04:45:30");
        
        if (!ITEMS)
        {
            ITEMS = new map<string, ref ServerBoostEffect>();
        }
            
        if (!SETTINGS)
        {
            SETTINGS = new ServerSystemSettings();
        }
        
        EnsureDirectoryExists();
        
        bool configExists = FileExist(CONFIG_PATH);
        
        if (!configExists)
        {
            Print("[BoostedItemSystem-Server] Серверный конфиг не найден, создаем новый...");
            SetDefaultConfig();
            SaveJsonConfig();
        }
        else
        {
            if (!LoadConfig())
            {
                Print("[BoostedItemSystem-Server] Не удалось загрузить серверный конфиг, создаем новый...");
                SetDefaultConfig();
                SaveJsonConfig();
            }
        }
        
        Print("[BoostedItemSystem-Server] Всего загружено предметов: " + ITEMS.Count().ToString());
        LogConfigInfo();
    }
    
    static void LogConfigInfo()
    {
        Print("[BoostedItemSystem-Server] Серверные настройки восстановления:");
        Print("[BoostedItemSystem-Server] HEALTH_RATE: " + SETTINGS.HEALTH_RATE.ToString());
        Print("[BoostedItemSystem-Server] BLOOD_RATE: " + SETTINGS.BLOOD_RATE.ToString());
        Print("[BoostedItemSystem-Server] WATER_RATE: " + SETTINGS.WATER_RATE.ToString());
        Print("[BoostedItemSystem-Server] ENERGY_RATE: " + SETTINGS.ENERGY_RATE.ToString());
        Print("[BoostedItemSystem-Server] STAMINA_RATE: " + SETTINGS.STAMINA_RATE.ToString());
        Print("[BoostedItemSystem-Server] SHOCK_RATE: " + SETTINGS.SHOCK_RATE.ToString());
        
        Print("[BoostedItemSystem-Server] Серверные настройки расхода quantity:");
        Print("[BoostedItemSystem-Server] CONSUMPTION_RATE_MULTIPLIER: " + SETTINGS.CONSUMPTION_RATE_MULTIPLIER.ToString());
        Print("[BoostedItemSystem-Server] CONSUMPTION_LOG_THRESHOLD: " + SETTINGS.CONSUMPTION_LOG_THRESHOLD.ToString());
        
        foreach (string itemName, ServerBoostEffect effect : ITEMS)
        {
            Print("[BoostedItemSystem-Server] Предмет: " + itemName + " | Тип: " + effect.m_BoostType + " | Расход: " + effect.m_ConsumptionRate.ToString() + "%/сек");
        }
    }
    
    static private void EnsureDirectoryExists()
    {
        if (!FileExist(CONFIG_DIR))
        {
            Print("[BoostedItemSystem-Server] Создание серверной директории: " + CONFIG_DIR);
            
            if (MakeDirectory(CONFIG_DIR))
            {
                Print("[BoostedItemSystem-Server] Серверная директория успешно создана: " + CONFIG_DIR);
                
                string testFilePath = CONFIG_DIR + "\\test_server.txt";
                FileHandle testFile = OpenFile(testFilePath, FileMode.WRITE);
                if (testFile != 0)
                {
                    FPrintln(testFile, "Серверная директория создана: " + GetDate());
                    CloseFile(testFile);
                    Print("[BoostedItemSystem-Server] Серверная директория подтверждена: " + CONFIG_DIR);
                }
                else
                {
                    Print("[BoostedItemSystem-Server] ОШИБКА: Не удалось создать тестовый файл");
                }
            }
            else
            {
                Print("[BoostedItemSystem-Server] ОШИБКА: Не удалось создать серверную директорию");
            }
        }
        else
        {
            Print("[BoostedItemSystem-Server] Серверная директория уже существует: " + CONFIG_DIR);
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
        Print("[BoostedItemSystem-Server] Загрузка серверного конфига из: " + CONFIG_PATH);
        
        ITEMS.Clear();
        SetDefaultConfig();
        
        Print("[BoostedItemSystem-Server] Загружено предметов: " + ITEMS.Count().ToString());
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
            Print("[BoostedItemSystem-Server] Серверный конфиг сохранен: " + CONFIG_PATH);
        }
        else
        {
            Print("[BoostedItemSystem-Server] ОШИБКА: Не удалось сохранить серверный конфиг");
        }
    }
    
    static private string CreateSettingsJson()
    {
        string settingsJson = "    \"ServerSettings\": {\n";
        settingsJson = settingsJson + "        \"HEALTH_RATE\": " + SETTINGS.HEALTH_RATE.ToString() + ",\n";
        settingsJson = settingsJson + "        \"BLOOD_RATE\": " + SETTINGS.BLOOD_RATE.ToString() + ",\n";
        settingsJson = settingsJson + "        \"WATER_RATE\": " + SETTINGS.WATER_RATE.ToString() + ",\n";
        settingsJson = settingsJson + "        \"ENERGY_RATE\": " + SETTINGS.ENERGY_RATE.ToString() + ",\n";
        settingsJson = settingsJson + "        \"STAMINA_RATE\": " + SETTINGS.STAMINA_RATE.ToString() + ",\n";
        settingsJson = settingsJson + "        \"SHOCK_RATE\": " + SETTINGS.SHOCK_RATE.ToString() + ",\n";
        settingsJson = settingsJson + "        \"CONSUMPTION_RATE_MULTIPLIER\": " + SETTINGS.CONSUMPTION_RATE_MULTIPLIER.ToString() + ",\n";
        settingsJson = settingsJson + "        \"CONSUMPTION_LOG_THRESHOLD\": " + SETTINGS.CONSUMPTION_LOG_THRESHOLD.ToString() + "\n";
        settingsJson = settingsJson + "    }";
        
        return settingsJson;
    }
    
    static private string CreateItemsJson()
    {
        string itemsJson = "    \"ServerItems\": {\n";
        int count = 0;
        int total = ITEMS.Count();
        
        foreach (string itemName, ServerBoostEffect effect : ITEMS)
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
        Print("[BoostedItemSystem-Server] Создание серверного конфига с расходом quantity");
        
        ITEMS.Clear();
        
        // Серверные настройки камней с расходом quantity
        ITEMS.Insert("A1_StoneHealth", new ServerBoostEffect("health", 15.0, 0.05));    // 0.05% в секунду
        ITEMS.Insert("A1_StoneBlood", new ServerBoostEffect("blood", 800.0, 0.08));     // 0.08% в секунду
        ITEMS.Insert("A1_StoneStamina", new ServerBoostEffect("stamina", 999.0, 0.03)); // 0.03% в секунду (медленно)
        ITEMS.Insert("A1_StoneWater", new ServerBoostEffect("water", 1500.0, 0.06));    // 0.06% в секунду
        ITEMS.Insert("A1_StoneEnergy", new ServerBoostEffect("energy", 1500.0, 0.06));  // 0.06% в секунду
        ITEMS.Insert("A1_StoneShock", new ServerBoostEffect("shock", 300.0, 0.10));     // 0.10% в секунду
    }

    static void ReloadConfig()
    {
        Print("[BoostedItemSystem-Server] Перезагрузка серверного конфига");
        Init();
    }
}