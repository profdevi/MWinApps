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


//v3.4 copyright Comine.com 20150816U2341
#ifndef TArray_h
#define TArray_h

//////////////////////////////////////////////////
#include "MStdLib.h"

//******************************************************
//**  TArray class
//******************************************************
template <class Element>
class TArray
	{
	Element **mArray;
	int mCount;
	int mMaxCount;
	int mIncrementCount;
	
	////////////////////////////////////////////////
	void ClearObject(void)
		{
		mArray=NULL;
		mCount=0;
		mIncrementCount=0;
		mMaxCount=0;
		}
	
	////////////////////////////////////////////////
	public:
	TArray(void)
		{
		ClearObject();
		}

	////////////////////////////////////////////////
	TArray(int incrementcount)
		{
		ClearObject();
		if(Create(incrementcount)==false)
			{
			return;
			}
		}

	////////////////////////////////////////////////
	~TArray(void)
		{
		Destroy();
		}

	////////////////////////////////////////////////
	bool Create(int incrementcount)
		{
		Destroy();

		MStdAssert(incrementcount>0);

		mArray=new Element *[incrementcount];
		if(mArray==NULL)
			{
			Destroy();
			return false;
			}

		// Update Data Members
		mCount=0;
		mIncrementCount=incrementcount;
		mMaxCount=incrementcount;

		//init block
		int i;
		for(i=0;i<mMaxCount;++i)
			{
			mArray[i] = NULL;
			}
		
		return true;
		}


	////////////////////////////////////////////////
	bool Destroy(void)
		{
		if(mArray!=NULL)
			{
			int i;
			for(i=0;i<mMaxCount;++i)
				{
				if(mArray[i]!=NULL)
					{
					delete mArray[i];
					}

				mArray[i]=NULL;
				}

			delete mArray;
			mArray=NULL;
			}

		ClearObject();
		return true;
		}
	

	///////////////////////////////////////////////
	Element *GetElement(int i)							// Get an element
		{		
		MStdAssert(i>=0 && i<mCount);
		return mArray[i];
		}


	///////////////////////////////////////////////
	int GetLength(void)									// Get Length
		{
		return mCount;
		}


	///////////////////////////////////////////////
	int GetCapacity(void)								// Get Capacity
		{
		return mMaxCount;
		}


	///////////////////////////////////////////////
	bool Delete(int delpos)
		{
		if(delpos<0 && delpos>=mCount)
			{
			return false;
			}

		// Now Attempt to delete item
		if(mArray[delpos]!=NULL)
			{
			delete mArray[delpos];
			mArray[delpos]=NULL;
			}

		// Move every item down by one
		int k;
		for(k=delpos;k<mCount;++k)
			{
			mArray[k] = mArray[k+1];
			}

		mArray[mCount-1] = NULL;

		mCount = mCount - 1;
		MStdAssert(mCount>=0);
		return true;
		}


	///////////////////////////////////////////////
	bool Insert(int itempos)									// Insert an element
		{
		if(itempos<0 || itempos>mCount)
			{
			return false;
			}

		if(Resize(mCount+1)==false)
			{
			return false;
			}

		Element *lastelement=mArray[mCount-1];
		MStdAssert(lastelement!=NULL);

		// Shift all the elements by 
		int i;
		for(i=mCount-2;i>=itempos;--i)
			{
			mArray[i+1] = mArray[i];
			}

		mArray[itempos]=lastelement;
		return true;
		}


	///////////////////////////////////////////////
	int Insert(void)								// Insert element at end
		{
		MStdAssert(mCount>=0);
		const int newposition=mCount;
		if(Insert(newposition)==false)
			{
			return -1;
			}

		return newposition;
		}

	///////////////////////////////////////////////
	bool Resize(int newlength)
		{
		MStdAssert(newlength>=0);
		if(newlength<=mCount)
			{
			int i;
			for(i=newlength;i<mCount;++i)
				{
				delete mArray[i];
				mArray[i] = NULL;
				}

			mCount = newlength;
			return true;
			}

		if(newlength<=mMaxCount)
			{
			// Allocate new elements
			int i;
			for(i=mCount;i<newlength;++i)
				{
				mArray[i]=new Element;
				if(mArray[i]==NULL)
					{
					// Backtrack and release memory
					int k;
					for(k=mCount;k<i;++k)
						{
						delete mArray[i];
						mArray[i]=NULL;
						}

					return false;
					}
				}

			// Update the information about
			mCount = newlength;

			return true;
			}

		// Calculate new max size based
		const int newmaxsize=((newlength+mIncrementCount-1)/mIncrementCount)
				*mIncrementCount;

		// Allocate new Array
		Element **newarray;
		newarray = new Element *[newmaxsize];
		if(newarray==NULL)
			{
			return false;
			}

		// Copy into new array
		int i;
		for(i=0;i<mCount;++i)
			{
			newarray[i] = mArray[i];
			}

		for(i=mCount;i<newlength;++i)
			{
			newarray[i] = new Element;
			if(newarray[i]==NULL)
				{
				// Release allocated memory
				int k;
				for(k=mCount; k<i;++k)
					{
					delete newarray[k];
					newarray[k]=NULL;
					}

				delete[] newarray;
				return false;
				}
			}

		// Place NULLs in extra space
		for(i=newlength;i<newmaxsize;++i)
			{
			newarray[i]=NULL;
			}

		// Release array
		delete[] mArray;

		mArray=newarray;
		mMaxCount = newmaxsize;
		mCount=newlength;
		return true;
		}


	//////////////////////////////////////////////
	bool Swap(int pos1,int pos2)
		{
		if(pos1<0 || pos1>=mCount || pos2<0 || pos2>=mCount)
			{
			return false;
			}

		Element *tmp=mArray[pos1];
		mArray[pos1] = mArray[pos2];
		mArray[pos2] = tmp;

		return true;
		}


	//////////////////////////////////////////////
	bool Swap(int pos1,TArray<Element> &obj,int pos2)
		{
		if(pos1<0 || pos1>=mCount)
			{
			return false;
			}

		const int otherlength=obj.GetLength();
		if(pos2<0 || pos2>=otherlength)
			{
			return false;
			}

		Element * const tmp=mArray[pos1];
		mArray[pos1] = obj.mArray[pos2];
		obj.mArray[pos2] = tmp;
		return true;
		}
	};


#endif // TArray_h

