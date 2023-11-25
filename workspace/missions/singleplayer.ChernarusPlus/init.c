class CustomMission: MissionGameplay
{
 void CustomMission()
 {
 }
};Mission CreateCustomMission(string path)
{
 return new CustomMission();
}void main()
{
 // Create player
 PlayerBase player = PlayerBase.Cast( ( GetGame().CreatePlayer( NULL, "SurvivorF_Linda", "2200 10 2200", 0, "NONE") ) );// Set your gear
 player.CreateInInventory("TShirt_Black");// Select player
 GetGame().SelectPlayer(NULL, player);
}