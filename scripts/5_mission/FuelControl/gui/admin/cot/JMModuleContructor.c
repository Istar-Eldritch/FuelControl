#ifdef JM_COT
modded class JMModuleConstructor
{
	override void RegisterModules( out TTypenameArray modules )
	{
		super.RegisterModules( modules );
		modules.Insert(FCModule);
	}
};
#endif