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
#include "MStdLib.h"
#include "MString.h"
#include "TArray.h"
#include "MWinRegKey.h"
#include "MBuffer.h"
#include "MWinUninstaller.h"


//******************************************************
//**  Module Elements
//******************************************************
// Windows Seems to return uninstall keys for both 64 and 32 togather from 32 bit api.  There is no
// need to look under HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall
static const HKEY GHive=HKEY_LOCAL_MACHINE;
static const wchar_t GHiveKeyUninstall[]=L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
//static const wchar_t GHiveKeyUninstall[]=L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

//******************************************************
//**  MWinUninstaller class
//******************************************************
void MWinUninstaller::ClearObject(void)
	{
	mRegFlags=0;
	}


////////////////////////////////////////////////
MWinUninstaller::MWinUninstaller(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MWinUninstaller::~MWinUninstaller(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MWinUninstaller::Create(bool use64bit)
	{
	Destroy();

	if(use64bit==true)
		{ mRegFlags=KEY_WOW64_64KEY; }
	else
		{ mRegFlags=0U; }

	MWinRegKey regkey;
	if(regkey.Create(false,GHiveKeyUninstall,GHive,true,KEY_READ|mRegFlags)==false)
		{
		Destroy();
		return false;
		}

	if(mKeys.Create(500)==false)
		{
		Destroy();
		return false;
		}

	MStringWide subkey;
	for(int i=0;regkey.GetSubKeyName(i,subkey)==true;++i)
		{
		const int index=mKeys.Insert();
		if(index<0)
			{
			Destroy();
			return false;
			}

		if(mKeys.GetElement(index)->Create(subkey.Get() )==false)
			{
			Destroy();
			return false;
			}
		}
	
	regkey.Destroy();

	// Clean out the
	return true;
	}


////////////////////////////////////////////////
bool MWinUninstaller::Destroy(void)
	{
	mKeys.Destroy();
	ClearObject();
	return true;
	}


/////////////////////////////////////////////////
int MWinUninstaller::GetKeyCount(void)
	{
	return mKeys.GetLength();
	}


//////////////////////////////////////////////////
const wchar_t *MWinUninstaller::GetKey(int index)
	{
	return mKeys.GetElement(index)->Get();
	}


/////////////////////////////////////////////////
bool MWinUninstaller::GetInfo(int index,MStringWide &displayname,MStringWide &displayversion
		,MStringWide &publisher,MStringWide &uninstallstring)
	{
	const wchar_t *key=mKeys.GetElement(index)->Get();
	if(key==0)
		{
		return false;
		}

	MStringWide uninstallkey=GHiveKeyUninstall;
	uninstallkey.Append(L"\\");
	uninstallkey.Append(key);

	MWinRegKey appkey;
	if(appkey.Create(false,uninstallkey.Get(),GHive,true,KEY_READ|mRegFlags)==false)
		{
		return false;
		}

	if(appkey.DoesNameExist(L"DisplayName")==false || appkey.GetValueData(L"DisplayName",displayname)==false)
		{
		displayname.Create(L"");
		}

	if(appkey.DoesNameExist(L"DisplayVersion")==false || appkey.GetValueData(L"DisplayVersion",displayversion)==false)
		{
		displayversion.Create("");
		}

	if(appkey.DoesNameExist(L"Publisher")==false || appkey.GetValueData(L"Publisher",publisher)==false)
		{
		publisher.Create(L"");
		}	

	if(appkey.DoesNameExist("UninstallString")==false || appkey.GetValueData(L"UninstallString",uninstallstring)==false)
		{
		uninstallstring.Create(L"");
		}

	appkey.Destroy();
	return true;
	}


//////////////////////////////////////////////////////////
bool MWinUninstaller::Print(void)
	{
	const int count=mKeys.GetLength();
	for(int i=0;i<count;++i)
		{
		MStdPrintf("%-4d  ",i);
		MStringWide displayname;
		MStringWide displayversion;
		MStringWide publisher;
		MStringWide uninstallstring;

		if(GetInfo(i,displayname,displayversion,publisher,uninstallstring)==false) { continue; }

		if(displayname.Length()!=0)
			{
			MStdPrintf("%-38S ",displayname.Get() );
			}
		else
			{
			MStdPrintf("%-38S ",mKeys.GetElement(i)->Get() );
			}

		if(displayversion.Length()!=0)
			{
			MStdPrintf("[vers: %S] ",displayversion.Get() );
			}

		if(publisher.Length()!=0)
			{
			MStdPrintf("%S",publisher.Get() );
			}

		MStdPrintf("\n");
		}

	return true;
	}


////////////////////////////////////////////////////////
bool MWinUninstaller::Uninstall(int index)
	{
	MStringWide displayname;
	MStringWide displayversion;
	MStringWide publisher;
	MStringWide uninstallstring;

	if(GetInfo(index,displayname,displayversion,publisher,uninstallstring)==false)
		{
		return false;
		}

	MString shortinstallstring;
	shortinstallstring.Create(uninstallstring.Get());

	if(MStdExec(shortinstallstring.Get())==false)
		{
		return false;
		}

	return true;
	}


