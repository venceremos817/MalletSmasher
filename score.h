#ifndef  __SCORE_H__
#define __SCORE_H__

#include "DirectX.h"
#include "Source.h"

typedef struct
{
	int Score;

}ScoreInfo;


HRESULT InitScore();
void UninitScore();
void UpdateScore();
void DrawScore();
void AddScore_1P(int);
void AddScore_2P(int);





#endif // ! __SCORE_H__
