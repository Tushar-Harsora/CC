#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cassert>

using namespace std;
/*
	E -> T E'
	E' -> + T E' | - T E' | ϵ
	T -> F T'
	T' -> * F T' | / F T' | ϵ
	F -> (E) | number | name
*/

void E();
void EP();
void T();
void TP();
void F();

int curr = 0;
string s;
#define CONSUME_SPACES() while(isspace(s[curr++]))
bool valid = true;

int main(){
	getline(cin, s);
	E();

	if(valid){
		cout << "Expression is\033[0;31m Valid\033[0m";
	}else{
		cout << "Expression is\033[0;31m not Valid\033[0m";
	}
	cout << endl;
	return 0;
}

void E(){
	T();
	EP();
}

void EP(){
	while(isspace(s[curr]))	curr++;
	// cout << "in EP " << curr << " char " << int(s[curr]) << endl;
	if(s[curr] == '+' || s[curr] == '-'){
		curr++;
		T();
		EP();
	}
}

void T(){
	F();
	TP();
}

void TP(){
	while(isspace(s[curr]))	curr++;
	if(s[curr] == '*' || s[curr] == '/'){
		curr++;
		F();
		TP();
	}
}

void F(){
	while(isspace(s[curr]))	curr++;
	if(s[curr] == '('){			// should be (E)
		curr++;
		E();
		while(isspace(s[curr]))	curr++;
		if(s[curr] == ')'){
			curr++;
		}else{
			valid = false;
		}
	}else if(isdigit(s[curr])){	// should be CONSTANT
		while(isdigit(s[curr]))	curr++;
		if(s[curr] == '.'){		// CONSTANT is floating point
			curr++;
			while(isdigit(s[curr]))	curr++;
		}
	}else if(isalnum(s[curr])) {						// should be variable name
		while(isalnum(s[curr]))	curr++;
	}else{
		valid = false;
	}
}