#ifndef C_MODULE_INPUT__H
#define C_MODULE_INPUT__H

#include "AModule.h"

class CModuleInput : public AModule
{
public:
	CModuleInput() = default;

	bool Update() override;
};

#endif // C_MODULE_INPUT__H
