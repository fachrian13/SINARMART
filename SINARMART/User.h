#pragma once
#ifndef _USER_
#define _USER_

struct DatabaseUser {
	bool Blocked;
	Int8 Username[15];
	Int8 Password[15];
};

class User :public BinaryFile<DatabaseUser> {
public:
	User() :BinaryFile("User.bin") {}
} $User;

#endif // !_USER_