#ifndef __GOAL_H__
#define __GOAL_H__


#include "Source.h"

#define GOALHEIGHT (MALLETRADIUS*3.5)


// ç\ë¢ëÃ
typedef struct
{
	float width;
	float height;
	float posx;
	float posy;
	float Alpha;
	float Red;
	float Blue;
	float Green;

}GoalInfo;




void InitGoal();
void UninitGoal();
void UpdateGoal();
void DrawGoal();




#endif // !__GOAL_H__

