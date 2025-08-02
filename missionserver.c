// File created: 2025-07-30 22:22:15
// Author: wqedfv
// Server mission file for BoostedItemSystem

modded class MissionServer
{
    ref ServerBoostedItemModule m_ServerBoostedItemModule;

    void MissionServer()
    {
        Print("[BoostedItemSystem-Server] MissionServer инициализация");
    }

    override void OnInit()
    {
        super.OnInit();
        Print("[BoostedItemSystem-Server] MissionServer OnInit");
        
        if (!m_ServerBoostedItemModule)
        {
            m_ServerBoostedItemModule = new ServerBoostedItemModule();
            m_ServerBoostedItemModule.OnInit();
            Print("[BoostedItemSystem-Server] ServerBoostedItemModule создан и инициализирован");
        }
    }

    override void OnMissionFinish()
    {
        super.OnMissionFinish();
        
        if (m_ServerBoostedItemModule)
        {
            delete m_ServerBoostedItemModule;
            Print("[BoostedItemSystem-Server] ServerBoostedItemModule удален");
        }
    }
}