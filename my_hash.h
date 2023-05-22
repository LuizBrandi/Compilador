#ifndef MY_HASH

#define MY_HASH

#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib>

using namespace std;

typedef struct{
	string varName;
	string type;
} SYMBOL_TYPE;

void insertElement(unordered_map<string, SYMBOL_TYPE>& hash, string key, SYMBOL_TYPE value);
void removeElement(unordered_map<string, SYMBOL_TYPE>& hash, string key);
void findElement(unordered_map<string, SYMBOL_TYPE> hash, string key);
SYMBOL_TYPE returnElement(unordered_map<string, SYMBOL_TYPE>& hash, string key);
void printHash(unordered_map<string, SYMBOL_TYPE> hash);

#endif
