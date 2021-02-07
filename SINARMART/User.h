#pragma once
#ifndef _USER_
#define _USER_

struct DataUser {
	bool Blocked;
	Int8 Username[15];
	Int8 Password[15];
};

class User :public BinaryFile<DataUser> {
public:
	User() :BinaryFile("User.bin") {}
} $User;

#endif // !_USER_