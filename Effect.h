#ifndef __EFFECT_H__
#define __EFFECT_H__

// ç\ë¢ëÃ
typedef struct
{
	float width;
	float height;
	float posx;
	float posy;
	float Alpha;
	bool exist;
	int Count;
	int EffectTime;

}GoalEffect;

void InitEffect();
void UninitEffect();
void UpdateEffect();
void DrawEffect();
void StartEffect(int side);



#endif // !__EFFECT_H__

