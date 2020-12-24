#pragma once
#include "BaseAnim.h"

class Anim_Directional : public BaseAnim
{
protected:
	void Framestep();
	void CropSprite();
	void ReadIn(std::stringstream& lStream);
};