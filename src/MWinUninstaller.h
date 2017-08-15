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


//v1.0 copyright Comine.com 20170815T0524
#ifndef MWinUninstaller_h
#define MWinUninstaller_h

/////////////////////////////////////////////////
#include "MStdLib.h"
#include "TArray.h"
#include "MString.h"

//******************************************************
//**  MWinUninstaller class
//******************************************************
class MWinUninstaller
	{
	////////////////////////////////////////////////
	TArray<MStringWide> mKeys;
	unsigned int mRegFlags;
	
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MWinUninstaller(void);
	~MWinUninstaller(void);
	bool Create(bool use64bit=false);
	bool Destroy(void);
	int GetKeyCount(void);
	const wchar_t *GetKey(int index);
	bool GetInfo(int index,MStringWide &displayname,MStringWide &displayversion,MStringWide &publisher
			,MStringWide &uninstallstring);
	bool Print(void);
	bool Uninstall(int index);			// Uninstall
	};

#endif // MWinUninstaller_h

