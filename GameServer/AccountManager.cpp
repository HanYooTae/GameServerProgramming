#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void AccountManager::ProcessLogin()
{
	// account Lock
	lock_guard<mutex> guard(_mutex);

	// user Lock
	User* user = UserManager::Instance()->GetUser(100);
}