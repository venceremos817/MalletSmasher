#pragma once
//==================================================
//	CMyColor.h
//--------------------------------------------------
//	author:大平哲士
//--------------------------------------------------
//	RGB,HSVの数値を扱う
//	各数値は 0.0f ～ 1.0f の範囲の値とする
//--- 更新履歴 ---
//	20/09/06	製作開始
//==================================================

#include "CommonClass.h"



class CMyColor
{
public:
	CMyColor();
	~CMyColor();

	void SetRGB(float, float, float);
	void SetRGB(float, float, float, float);
	void SetRGB(Float3);
	void SetRGB(Float4);
	void SetRed(float);
	void SetGreen(float);
	void SetBlue(float);
	Float3 GetRBG();
	float GetRed();
	float GetGreen();
	float GetBlue();

	void SetHSV(float, float, float);
	void SetHSV(float, float, float, float);
	void SetHSV(Float3);
	void SetHSV(Float4);
	void SetHue(float);
	void SetSaturation(float);
	void SetValue(float);
	Float3 GetHSV();
	float GetHue();
	float GetSaturation();
	float GetValue();

	void SetAlpha(float);
	float GetAlpha();

private:
	Float3 m_RGB;
	Float3 m_HSV;
	float m_Alpha;

	// RGB→HSV
	void ConvertRGBintoHSV();
	// HSV→RGB
	void ConvertHSVintoRGB();
};

