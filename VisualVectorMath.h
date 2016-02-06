/**********************************************************************
*<
FILE: afregion.cpp

DESCRIPTION:  Affect region modifier

CREATED BY: Rolf Berteig

HISTORY: 10/16/96

*>	Copyright (c) 1994, All Rights Reserved.
**********************************************************************/

#pragma once

#include "DllEntry.h"

#include "iparamm2.h"
#include "MaxIcon.h"
#include "resource.h"

#include "ifnpub.h"
#include <maxscript/maxscript.h>
#include <maxscript/util/listener.h>
#include <maxscript/maxwrapper/mxsobjects.h>
#include "imacroscript.h"
#include <Util/StaticAssert.h>
#include <SingleWeakRefMaker.h>


#define ScriptPrint (the_listener->edit_stream->printf)

#define PBLOCK_REF	0

// Modifier class ID
#define VISUALVECTORMATH_CLASS_ID		0x14f44

//Interface ID
#define VISUALVECTORMATH_INTERFACE Interface_ID(0xDE21AA1f, 0x8223300)


class VisualVectorMathData : public LocalModData
{
public:
	virtual LocalModData *Clone();
};

/*! IMeshDeformPWMod is the interface into the Skin Wrap(Mesh) modifier */
class IVisualVectorMath : public FPMixinInterface
{
public:
	FPInterfaceDesc* GetDesc();

	// Rollup IDs
	enum {
		rollup_params,		// parameter rollup
		rollup_functions,
	};


	//Param Block IDs
	enum {
		pb_float_variables,		// list of variables
		pb_vector_variables,	// list of vector variables
	};

	//Maxcsript IDs
	enum {
		script_testfunction
	};


	//Function Publishing System
	//Function Map For Mixin Interface
	//*************************************************
	BEGIN_FUNCTION_MAP
		FN_1(script_testfunction, TYPE_INT, fnTestFunction, TYPE_INDEX);


	END_FUNCTION_MAP


	virtual int fnTestFunction(int index) = 0;

};

//Modifier class 
class VisualVectorMath : public Modifier, public IVisualVectorMath {
public:



	VisualVectorMath();
	~VisualVectorMath();

	// From Animatable
	void DeleteThis() { delete this; }
	void GetClassName(TSTR& s) { s = GetString(IDS_VISUALVECTORMATH); }
	virtual Class_ID ClassID() { return Class_ID(VISUALVECTORMATH_CLASS_ID, 0); }
	void BeginEditParams(IObjParam  *ip, ULONG flags, Animatable *prev);
	void EndEditParams(IObjParam *ip, ULONG flags, Animatable *next);
	RefTargetHandle Clone(RemapDir& remap);
	const TCHAR *GetObjectName() { return GetString(IDS_VISUALVECTORMATH); }

	// From modifier
	ChannelMask ChannelsUsed() { return GEOM_CHANNEL | TOPO_CHANNEL | SELECT_CHANNEL | SUBSEL_TYPE_CHANNEL | TEXMAP_CHANNEL | VERTCOLOR_CHANNEL; }
	ChannelMask ChannelsChanged() { return GEOM_CHANNEL | TEXMAP_CHANNEL | VERTCOLOR_CHANNEL | SELECT_CHANNEL; }
	Class_ID InputType() { return defObjectClassID; }
	
	void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node);
	Interval LocalValidity(TimeValue t);


	int NumRefs() { return 1; }
	RefTargetHandle GetReference(int i);


	// From BaseObject
	int Display(TimeValue t, INode* inode, ViewExp *vpt, int flagst, ModContext *mc);
	void GetWorldBoundBox(TimeValue t, INode* inode, ViewExp *vpt, Box3& box, ModContext *mc);

	int NumSubs() { return 1; }
	Animatable* SubAnim(int i) { return GetReference(i); }
	TSTR SubAnimName(int i) { return GetString(IDS_PARAMS); }

	RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget,
		PartID& partID, RefMessage message, BOOL propagate);

	int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
	IParamBlock2* GetParamBlock(int i) { return pblock; } // return i'th ParamBlock
	IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock


	 // Loading/Saving
	IOResult Load(ILoad *iload);
	IOResult Save(ISave *isave);

	IOResult SaveLocalData(ISave *isave, LocalModData *pld);
	IOResult LoadLocalData(ILoad *iload, LocalModData **pld);

	CreateMouseCallBack* GetCreateMouseCallBack() { return NULL; }

	static IObjParam *ip;
	static IParamMap *pmapParam;
	static VisualVectorMath *editMod;

private:
	virtual void SetReference(int i, RefTargetHandle rtarg);
	IParamBlock2 *pblock;




//published functions
//Function Publishing method (Mixin Interface)
//******************************
	BaseInterface* GetInterface(Interface_ID id)
	{
		if (id == (Interface_ID)VISUALVECTORMATH_INTERFACE)
			return (IVisualVectorMath*)this;
		BaseInterface* pInterface = FPMixinInterface::GetInterface(id);
		if (pInterface != NULL)
		{
			return pInterface;
		}
		// return the GetInterface() of its super class
		return Modifier::GetInterface(id);
	}

public:
	int fnTestFunction(int index);

};




//--- ClassDescriptor and class vars ---------------------------------

class VisualVectorMathClassDesc :public ClassDesc2 {
public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE);
	const TCHAR *	ClassName() { return GetString(IDS_VISUALVECTORMATH); }
	SClass_ID		SuperClassID() { return OSM_CLASS_ID; }
	Class_ID		ClassID() { return Class_ID(VISUALVECTORMATH_CLASS_ID, 0); }
	const TCHAR* 	Category() { return GetString(IDS_DEFORMATIONS); }

	const TCHAR*	InternalName() { return _T("VisualVectorMath"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle

};


