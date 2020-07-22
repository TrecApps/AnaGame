# AnaGame (Draft - to be updated soon)
#### Language: C++
#### Compiler: Visual C++
#### IDE: Visual Studio 2015 (created), 2019 (Current) - both Community edition
##### Note: Professional or above should work, so long as MFC part of your installation

## Set-up

#### Update these fields

![Update Fields](https://github.com/TrecApps/AnaGame/blob/master/Documentation/Read_me_images/Properties%20Windows.PNG)

1. Clone the Project from Github (*git clone https://github.com/TrecApps/AnaGame.git *)
2. Open the solution in Visual Studio
3. Build the TrecLib project (you'll need a debug folder)
4. For each project (other than TrecLib), Open their properties window
5. Check the "Library Directories" and "Include Directories" in "VC++ Directories"
--Make sure they point to the directories on _your_ system
--Recently, I modified the project files to point to _relevant_ files/folders, meaning you shouldn't have too much work to do.
6. Check the lib file list ("Linker --> Input --> Additional Dependencies")

__Note:__ For both steps 5 and 6, check the notes on each individual project to make sure your set up meets requirements. Hopefully, you
	shouldn't have to change anything.

#### Include Example:

![Include example](https://github.com/TrecApps/AnaGame/blob/master/Documentation/Read_me_images/Include%20Directories.PNG)

#### Library Directory Example

![Library Example](https://github.com/TrecApps/AnaGame/blob/master/Documentation/Read_me_images/Library%20Directories.PNG)

#### Lib Files Example

![Lib Files Example](https://github.com/TrecApps/AnaGame/blob/master/Documentation/Read_me_images/Lib%20Files.PNG)

### TrecLib (MFC Extension DLL)

No configurtion needs to be done

### VideoGraphics (MFC Extension DLL)

Include Directories:
* DirectXTex\DirectXTex-master\DirectXTex (3rd Party Project)
* TrecLib
 
Library Directories: 
* DirectXTex\DirectXTex-master\DirectXTex\Bin\Desktop_2015\Win32\Debug (3rd Party Project)
* Debug

Lib Files:
* TrecLib.lib
* d3d11.lib
* evr.lib
* mf.lib
* mfplay.lib
* mfreadwrite.lib
* mfuuid.lib
* mfplat.lib
* dxva2.lib
* Strmiids.lib
* D3DCompiler.lib
* DirectXTex.lib

__Note:__ Starting with Windows 8, the development kit for Windows included DirectX. However, one common
	function involved with Textures in prior DirectX Development was removed from the kit. Consequently, 
	because AnaGame was developed on Windows 8 and 10, you'll need the DirectXTex project found here:
	https://github.com/Microsoft/DirectXTex

__Note 2:__ I highly advise you to clone the _DirectXTex_ repo from the same directory you clone the _Anagame_ repo. That way, you shouldn't have to modify the 
	_VideoGraphics_ project file to point to this repo.

### Anaface (MFC Extension DLL)

Include Directories:
* TrecLib
* VideoGraphics

Library Directories:
* Debug

Lib Files:
* dwrite.lib
* d2d1.lib
* TrecLib.lib
* VideoGraphics.lib
* evr.lib
* mf.lib
* mfplay.lib
* mfreadwrite.lib
* mfuuid.lib
* mfplat.lib
* dxva2.lib
* Strmiids.lib

__Note:__ Some of these Lib files might not be needed (I know the first four ARE needed).

### Tap (MFC Extension DLL)

Include Directories:
* TrecLib
* VideoGraphics
* Anaface

Library Directories:
* Debug

Lib Files:
* TrecLib.lib
* Anaface.lib
* VideoGraphics.lib
* d2d1.lib

### AnaGame Applications (AnaGame_Builder, AnimateBuilder, AnimateCentral, Web_Tours)

Include Directories:
* Tap
* Anaface
* VideoGraphics
* TrecLib

Library Directories:
* Debug

Lib Files:
* Tap.lib
* Anaface.lib
* VideoGraphics.lib
* TrecLib.lib
* d2d1.lib

## Pointers in AnaGame

When coding the AnaGame project, you will likely create a few objects on the heap. Unless you
	are keeping track of your objects extremely well, you wan to put them in a TrecPointer<Obj_type>.
	TrecPointers are AnaGame's way of managing objects on the heap. They allow for counters to be
	maintained and memory returned to the heap once the count reaches 0. You can also manually delete
	an object and any other TrecPointer referencing it will convert itself to NULL (as opposed to dangling.

Originally, TrecPointers all referred to a table held within the TrecLib library. However, this strategy
	was scrapped in favor of the approach used by Rust's Rc container, which acts in a way similar to 
	C++'s std::shared_ptr - ie utilizing a counter for the reference. However, it does contain restrictions
	absent in C++'s shared pointers and present in Rust.

### Creating an active TrecPointer

There are only two ways a TrecPointer get's initialized:
1. The TrecPointer is assigned via an already active TrecPointer
2. The TrecPointer is initialized through a method in the TrecPointerKey class.

_Note:_ You cannot assign a raw pointer to a TrecPointer or use a raw pointer to construct a TrecPointer -
	Only the TrecPointerKey can do that!

`
// Easy Way
TrecPointer<type_name> tc_ptr_1 = tc_ptr_0; // Assume tc_ptr_0 is another "TrecPointer<type_name>"
`

### Using the _TrecPointerKey_

The _TrecPointerKey_ is the class to use when initializing a TrecPointer with a new object. This class provides
	multiple methods for Object initialization depending on the circumstances.

1. Basic Case: TrecPointerKey::GetNewTrecPointer<type>(params);

Use this for basic scenarios such as primitive types or objects you don't plan to represent as a different type

2. You need to initialize a subclass but hold a reference to a base class:
	TrecPointerKey::GetNewTrecPointerAlt<base_class, sub_class>(params);

_Note:_ There is currently no mechanism for assigning a TrecPointer of a base class to one of a subclass or vice versa.

3. Basic Case - but the class has a "SetSelf" method:
	TrecPointerKey::GetNewSelfTrecPointer<self_type>(param);

Sometimes, you might have a class that holds a reference to itself. It could be that your class might want to return a reference to itself.
	In that case, it should implement the "SetSelf(TrecPointer<self_type>)" method that the TrecPointerKey calls during initialization.

4. 2, but with the "SetSelf" method:
	TrecPointerKey::GetNewSelfTrecPointerAlt<base_self, sub_self>(param);
	
This basically combines 2 and 3, but it only works when the base class has the _SetSelf_ method

### TrecPointerSoft: useful for self references

In the event that a class needs to return a TrecPointer to itself, it should have a "TrecPointerSoft" as one of it's attributes.
	_TrecPointerSoft_ is a variation of the TrecPointer that does _not_ increment (or decrement) the counter. It is similar to the "weak_ptr" of
	C++'s Standard Library. Obviously, you wouldn't want a class that holds a regular TrecPointer to itself - the counter would never hit zero.
	
`
void TControl::SetSelf(TrecPointer<TControl> self)
{
	if (this != self.Get())
		throw L"Error! Function expected to recieve a protected reference to 'this' Object!";
	tThis = TrecPointerKey::GetSoftPointerFromTrec<TControl>(self);
}
`

Here is the SetSelf method for the TControl class. It first checks to make sure that the TrecPointer is indeed pointing to itself before using the 
	_TrecPointerKey_ to retrieve a _TrecPointerSoft_ reference to itself.

### Null checks

As usual, you should perform a null check before using the underlying pointer.

`
if(t_obj1.Get())
	t_obj1->someMethod();
`

### Cleaning up

There are two ways to Clean up a TrecPointer: Nullifying it and Deleting it!

`
tc_ptr.Nullify();
`

Nullify() will make the TrecPointer NULL and decrement the count. It will not delete the object unless the
	counter reaches zero.

`
tc_ptr.Delete();
`

Delete() will automatically delete the memory, regardless of how many references still exist. Each TrecPointer that
	still referenced it when this method was call will simply return NULL when doing null checks.

_Note:_ DO NOT extract a raw pointer from a TrecPointer and call "delete". TrecPointers have no way of knowing this occured and in
	_release_ builds could cause undefined behavior.

`
tc_type* r_prt = tc_ptr.Get();
delete r_ptr; // BAD! Will cause problems
`

## Overall Structure

![Structure](https://github.com/TrecApps/AnaGame/blob/master/Documentation/Read_me_images/Project%20Overview.png)

AnaGame Currently Uses 5 Extension DLL's (libraries that provide C++ classes to multiple projects).
There might soon be more in the coming months as more features are added to AnaGame. These DLL's are used
by four MFC applications in the solution.

### DLLs

AnaGame has a stacked library structure where higher up libraries are dependent on the lower ones

* TrecLib: (Lowest layer) Provides the base of AnaGame including the TObject, TString, TFile, TrecPointers, and more
* VideoGraphics: Provides the 3D Graphics engine, manages 3D shaders, and 3D model management
* Anaface: User Interface Engine that provides the TControl and subclasses that draw themselves to screen and react to user input
* Tap:  Ties up the libraries together and provides an interface for applications to use AnaGame libraries effectively
* TrecCode: (Highest library (right now)) The newest addition, Not advanced at all at this point

### Applications

* AnaGame_Builder: the application intended to serve as AnaGame's IDE as well as oneday support video editing and other productivity functionality
* AnamateBuilder: Older version of _AnaGame_Builder_ that relies more on MFC and should be used for developing features in isolation
* AnimateCentral: Intended to serve as AnaGame's hub for watching videos and playing games. Currently used to develop Anaface
* Web_Tours: Serves as AnaGame's dedicated web browser, facilitating HTML/CSS support in Anaface

### Others

Currently, there are two other projects in AnaGame that were created but not under current development

* AnagameCompiler: Functional DLL that's going to be superceeded by an Extension DLL
* AnafaceSupport: Functional DLL that's intended to manage possible UI styling attributes, supporting CSS attributes as well as Anaface attributes

## Before Execution

Copy the Resources Folder into the Debug/Release folder (depending on your compile mode). AnaGame expects the "Resources" folder (and its contents) to be in the
same folder as the exe and dll files.
