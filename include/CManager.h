#ifndef C_MANAGER__H
#define C_MANAGER__H

#include <vector>

class AModule;
class CModuleWindow;
class CModuleRenderer;
class CModuleInput;
class CModuleObjectManager;
class CModuleTimer;

class CManager
{
public:
	CManager() = default;

	void Play();

	CModuleTimer* mModuleTimer;
	CModuleWindow* mModuleWindow;
	CModuleRenderer* mModuleRenderer;
	CModuleInput* mModuleInput;
	CModuleObjectManager* mModuleObjectManager;

private:
	void InitModules();

	std::vector<AModule*> mModules; 
};

extern CManager* manager;

#endif // C_MANAGER__H
