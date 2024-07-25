#ifndef C_MODULE_TIMER__H
#define C_MODULE_TIMER__H

#include "AModule.h"

class CModuleTimer : public AModule
{
public:
	CModuleTimer() = default;

	bool Update() override;

	const float& GetDeltaTime() const;

private:
	float mDeltaTime = 0;
	float mLastTime = 0;
};

#endif
