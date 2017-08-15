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


//v1.0 copyright Comine.com 20170815T0539
#include "MStdLib.h"
#include "MCommandArg.h"
#include "MWinUninstaller.h"


//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="MWinApps";	// Used in Help
static const char *GApplicationVersion="1.0";			// Used in Help

////////////////////////////////////////////////////
static void GDisplayHelp(void);

////////////////////////////////////////////////////
int main(int argn,const char *argv[])
	{
	MCommandArg args(argn,argv);

	///////////////////////////////////////////////
	if(args.CheckRemoveHelp()==true)
		{
		GDisplayHelp();
		return 0;
		}

	bool use64bit=true;
	if(args.CheckRemoveArg("-32")==true) { use64bit=false; }

	MWinUninstaller uninstaller;
	if(uninstaller.Create(use64bit)==false)
		{
		MStdPrintf("**Unable to init system\n");
		return 1;
		}

	const int keycount=uninstaller.GetKeyCount();
	
	// -u : Uninstall application
	if(args.CheckRemoveArg("-u")==true && args.GetArgCount()>1)
		{
		const int index=MStdAToI(args.GetArg(1));
		if(index<0 || index>=keycount)
			{
			MStdPrintf("**Uninstall Index %d is bad\n",index);
			return 1;
			}

		if(uninstaller.Uninstall(index)==false)
			{
			MStdPrintf("**Uninstall failed for index %d\n",index);
			return 1;
			}

		MStdPrintf("Done...\n");
		return 0;
		}

	// -s : Show Information
	if(args.CheckRemoveArg("-i")==true && args.GetArgCount()>1)
		{
		const int index=MStdAToI(args.GetArg(1));
		if(index<0 || index>=keycount)
			{
			MStdPrintf("**Uninstall Index %d is bad\n",index);
			return 1;
			}

		MStringWide displayname;
		MStringWide displayversion;
		MStringWide publisher;
		MStringWide uninstallstring;

		if(uninstaller.GetInfo(index,displayname,displayversion,publisher,uninstallstring)==false)
			{
			MStdPrintf("**Unable to get information for software %d\n",index);
			return 2;
			}

		MStdPrintf("Index    :  %d\n",index);
		MStdPrintf("Key      :  %S\n",uninstaller.GetKey(index) );
		MStdPrintf("Name     :  %S\n",displayname.Get() );
		MStdPrintf("Ver      :  %S\n",displayversion.Get() );
		MStdPrintf("Pub      :  %S\n",publisher.Get() );
		MStdPrintf("Remove   :  %S\n",uninstallstring.Get() );
		return 0;
		}
	
	uninstaller.Print();

	return 0;
	}


////////////////////////////////////////////////////
static void GDisplayHelp(void)
	{
	MStdPrintf(	"\n"
				"   usage:  %s [-?] [-32] ([-u <Key Index>]|[-i <key index>]) \n"
				"           v%s copyright Comine.com\n"
				"\n"
				"   Displays list of applications installed on windows as displayed.\n"
				"\n"
				"      -32  :  Show 32 bit installed application\n"
				"      -u   :  Uninstall application\n"
				"      -i   :  Show Information about application\n"
				"\n"
				,GApplicationName,GApplicationVersion);
	}


