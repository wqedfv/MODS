// File created: 2025-07-30 22:22:15
// Author: wqedfv
// Client mission file for BoostedItemSystem

modded class MissionGameplay
{
    ref ClientBoostedItemModule m_ClientBoostedItemModule;

    void MissionGameplay()
    {
        Print("[BoostedItemSystem-Client] MissionGameplay инициализация");
    }

    override void OnInit()
    {
        super.OnInit();
        Print("[BoostedItemSystem-Client] MissionGameplay OnInit");
        
        if (!m_ClientBoostedItemModule)
        {
            m_ClientBoostedItemModule = new ClientBoostedItemModule();
            m_ClientBoostedItemModule.OnInit();
            Print("[BoostedItemSystem-Client] ClientBoostedItemModule создан и инициализирован");
        }
    }

    override void OnMissionFinish()
    {
        super.OnMissionFinish();
        
        if (m_ClientBoostedItemModule)
        {
            delete m_ClientBoostedItemModule;
            Print("[BoostedItemSystem-Client] ClientBoostedItemModule удален");
        }
    }
}