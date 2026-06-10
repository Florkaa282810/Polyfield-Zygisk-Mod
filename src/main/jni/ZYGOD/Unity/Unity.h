#ifndef IMGUIANDROID_UNITY_H
#define IMGUIANDROID_UNITY_H

#include <codecvt>

#include <string>
#include <iostream>

int GetObscuredIntValue(uint64_t location){
    int cryptoKey = *(int *)location;
    int obfuscatedValue = *(int *)(location + 0x4);

    return obfuscatedValue ^ cryptoKey;
}
bool GetObscuredBoolValue(uint64_t location){
    int cryptoKey = *(int *)location;
    int obfuscatedValue = *(int *)(location + 0x4);

    return (bool)obfuscatedValue ^ cryptoKey;
}
/*
Here are some functions to safely get/set values for types from Anti Cheat Toolkit (https://assetstore.unity.com/packages/tools/utilities/anti-cheat-toolkit-10395)

I will add more to this as I go along.
*/

union intfloat {
	int i;
	float f;
};

/*
Get the real value of an ObscuredInt.

Parameters:
	- location: the location of the ObscuredInt
*/
int GetObscuredIntValue(uint64_t location){
	int cryptoKey = *(int *)location;
	int obfuscatedValue = *(int *)(location + 0x8);
	
	return obfuscatedValue ^ cryptoKey;
}


int GetObscuredIntAB(uint64_t location){
	int cryptoKey = *(int *)location;
    int encryptedValue = *(int *)(location + 0x4);
 	int realValue = encryptedValue ^ cryptoKey;
    return (realValue + 1) * -1;
}

/*
Set the real value of an ObscuredInt.

Parameters:
	- location: the location of the ObscuredInt
	- value: the value we're setting the ObscuredInt to
*/

void SetObscuredIntAB(uint64_t location, int value){
int cryptoKey = *(int *)location;
    int encryptedValue = *(int *)(location + 0x4);
    *(int *)(location + 0x4) = (value + 1) * -1;
}

void SetObscuredIntValue(uint64_t location, int value){
	int cryptoKey = *(int *)location;
	
	*(int *)(location + 0x8) = value ^ cryptoKey;
}

/*
Get the real value of an ObscuredFloat.

Parameters:
	- location: the location of the ObscuredFloat
*/
float GetObscuredFloatValue(uint64_t location){
	int cryptoKey = *(int *)location;
	int obfuscatedValue = *(int *)(location + 0x8);
	
	/* use this intfloat to set the integer representation of our parameter value, which will also set the float value */
	intfloat IF;
	IF.i = obfuscatedValue ^ cryptoKey;
	
	return IF.f;
}

/*
Set the real value of an ObscuredFloat.

Parameters:
	- location: the location of the ObscuredFloat
	- value: the value we're setting the ObscuredFloat to
*/
void SetObscuredFloatValue(uint64_t location, float value){
	int cryptoKey = *(int *)location;

	/* use this intfloat to get the integer representation of our parameter value */
	intfloat IF;
	IF.f = value;
	
	/* use this intfloat to generate our hacked ObscuredFloat */
	intfloat IF2;
	IF2.i = IF.i ^ cryptoKey;
	
	*(float *)(location + 0x8) = IF2.f;
}

#endif IMGUIANDROID_UNITY_H
