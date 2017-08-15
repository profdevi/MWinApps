/*    
    MWinApps.exe : Displays and Unnstalls Windows Installed Applications
    Copyright (C) 2017  Comine.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


//v2.2 copyright Comine.com 20170815T0526
#ifndef MWinRegKey_h
#define MWinRegKey_h

////////////////////////////////////////////////////
#include <windows.h>
#include "MString.h"
#include "MBuffer.h"

// Data types are REG_BINARY, REG_DWORD, REG_DWORD_LITTLE_ENDIAN
// REG_DWORD_BIG_ENDIAN  REG_EXPAND_SZ  REG_LINK  REG_MULTI_SZ REG_NONE
// REG_RESOURCE_LIST  REG_SZ

////////////////////////////////////////////////////
class MWinRegKey
	{
	static DWORD mTmpType;
	static DWORD mTmpSize;
	HKEY mKeyHandle;

	//////////////////////////////////////////////
	void ClearObject(void);
	bool CreateNewKey(const char *keyname,HKEY root,bool setvolatile,REGSAM access);
	bool CreateNewKey(const wchar_t *keyname,HKEY root,bool setvolatile,REGSAM access);
	bool OpenExistingKey(const char *keyname,HKEY root,REGSAM access);
	bool OpenExistingKey(const wchar_t *keyname,HKEY root,REGSAM access);

	////////////////////////////////////////////////
	public:
	MWinRegKey(void);
	~MWinRegKey(void);
	bool Create(bool create			// =true if create a fresh key
			,const char *keyname
			,HKEY root=HKEY_CURRENT_USER
			,bool setvolatile=true  // =true if key will be gone by shutdown
			,REGSAM access=KEY_READ|KEY_WRITE); // Create/Open Key
	bool Create(bool create			// =true if create a fresh key
			,const wchar_t *keyname
			,HKEY root=HKEY_CURRENT_USER
			,bool setvolatile=true  // =true if key will be gone by shutdown
			,REGSAM access=KEY_READ|KEY_WRITE); // Create/Open Key
	bool Destroy(void);
	HKEY GetKey(void);
	bool SetValueData(const char *name,const char *data);
	bool SetValueData(const wchar_t *name,const wchar_t *data);
	bool SetValueDataExpand(const char *name,const char *data);
	bool SetValueDataExpand(const wchar_t *name,const wchar_t *data);

	bool SetValueData(const char *name,DWORD value);
	bool SetValueData(const wchar_t *name,DWORD value);
	bool GetValueData(const char *name,MString &outvalue); // outvalue is a return value
	bool GetValueData(const wchar_t *name,MStringWide &outvalue); // outvalue is a return value
	bool GetValueData(const char *name,DWORD &value);
	bool GetValueData(const wchar_t *name,DWORD &value);
	bool GetValueData(const char *name,MBuffer &buffer);
	bool GetValueData(const wchar_t *name,MBuffer &buffer);
	bool DeleteValue(const char *name);
	bool DeleteValue(const wchar_t *name);
	bool GetValueTypeSize(const char *name,DWORD &type,DWORD &size=mTmpSize);
	bool GetValueTypeSize(const wchar_t *name,DWORD &type,DWORD &size=mTmpSize);
	bool GetSubKeyName(int index,MString &name);		// Keep increasing index until false
	bool GetSubKeyName(int index,MStringWide &name);	// Keep increasing index until false
	bool GetValueName(int index,MString &name,DWORD &type=mTmpType,DWORD &size=mTmpSize);
	bool GetValueName(int index,MStringWide &name,DWORD &type=mTmpType,DWORD &size=mTmpSize);
	bool DoesNameExist(const char *name);
	bool DoesNameExist(const wchar_t *name);

	/////// Class Methods //////////////////
	static bool Delete(const char *keyname,HKEY root=HKEY_CURRENT_USER);	// fails if child keys exist
	static bool Delete(const wchar_t *keyname,HKEY root=HKEY_CURRENT_USER);	// fails if child keys exist
	static bool DeleteTree(const char *keyname,HKEY root);					// Recursively delete key
	static bool DeleteTree(const wchar_t *keyname,HKEY root);				// Recursively delete key
	static bool DoesKeyExist(const char *keyname,HKEY root=HKEY_CURRENT_USER);
	static bool DoesKeyExist(const wchar_t *keyname,HKEY root=HKEY_CURRENT_USER);
	};


#endif // MWinRegKey_h

