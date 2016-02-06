#include "VisualVectorMath.h"


IParamMap       *VisualVectorMath::pmapParam = NULL;
IObjParam       *VisualVectorMath::ip = NULL;
VisualVectorMath          *VisualVectorMath::editMod = NULL;



//our static instance of the class descriptions
static VisualVectorMathClassDesc visualVectorMathDesc;
extern ClassDesc2* GetVisualVectorMathDesc() { return &visualVectorMathDesc; }

//paramblock description
static ParamBlockDesc2 visualVectorMath_param_blk(IVisualVectorMath::rollup_params, _T("params"), 0, &visualVectorMathDesc,
	P_AUTO_CONSTRUCT + P_AUTO_UI + P_MULTIMAP, PBLOCK_REF,
	2,
	//rollout
	IVisualVectorMath::rollup_params, IDD_ADD, IDS_VARIABLES, 0, 0, NULL,
	IVisualVectorMath::rollup_functions, IDD_FUNCTIONS, IDS_FUNCTIONS, 0, 0, NULL,
	// params

	IVisualVectorMath::pb_float_variables, _T("float_variables"), TYPE_FLOAT_TAB, P_ANIMATABLE | P_VARIABLE_SIZE, IDS_FLOAT_VARIABLES,
	p_end,

	IVisualVectorMath::pb_vector_variables, _T("vector_variables"), TYPE_POINT3_TAB, P_ANIMATABLE | P_VARIABLE_SIZE, IDS_VECTOR_VARIABLES,
	p_end,

	p_end
	);

//Function Publishing descriptor for Mixin interface
//*****************************************************
static FPInterfaceDesc visualVectorMath_Interface(
	VISUALVECTORMATH_INTERFACE, _T("VisualVectorMath"), 0, &visualVectorMathDesc, FP_MIXIN,

	IVisualVectorMath::script_testfunction, _T("testFunction"), 0, TYPE_INT, 0, 1,
	_T("index"), 0, TYPE_INDEX,

	p_end
	);

void *VisualVectorMathClassDesc::Create(BOOL loading)
{
	AddInterface(&visualVectorMath_Interface);
	return  new VisualVectorMath();
}

//access to the script interface description
FPInterfaceDesc* IVisualVectorMath::GetDesc()
{
	return &visualVectorMath_Interface;
}



//--- Affect region mod methods -------------------------------

VisualVectorMath::VisualVectorMath()
{
	pblock = NULL;
	GetVisualVectorMathDesc()->MakeAutoParamBlocks(this);
}

VisualVectorMath::~VisualVectorMath()
{
}


void VisualVectorMath::BeginEditParams(
	IObjParam  *ip, ULONG flags, Animatable *prev)
{
	this->ip = ip;
	editMod = this;

	// Add our sub object type
	// TSTR type1(GetString(IDS_RB_AFRPOINTS));
	// const TCHAR *ptype[] = {type1};
	// This call is obsolete. Please see BaseObject::NumSubObjTypes() and BaseObject::GetSubObjType()
	// ip->RegisterSubObjectTypes(ptype, 1);

	// Create sub object editing modes.
	//moveMode = new MoveModBoxCMode(this, ip);

	TimeValue t = ip->GetTime();
	NotifyDependents(Interval(t, t), PART_ALL, REFMSG_BEGIN_EDIT);
	NotifyDependents(Interval(t, t), PART_ALL, REFMSG_MOD_DISPLAY_ON);
	SetAFlag(A_MOD_BEING_EDITED);

	SetAFlag(A_MOD_BEING_EDITED);

	visualVectorMathDesc.BeginEditParams(ip, this, flags, prev);
	//visualVectorMath_param_blk.SetUserDlgProc(pb_float_variables, new MeshDeformParamsMapDlgProc(this));

}

void VisualVectorMath::EndEditParams(
	IObjParam *ip, ULONG flags, Animatable *next)
{
	this->ip = NULL;
	editMod = NULL;

	TimeValue t = ip->GetTime();

	// NOTE: This flag must be cleared before sending the REFMSG_END_EDIT
	ClearAFlag(A_MOD_BEING_EDITED);

	NotifyDependents(Interval(t, t), PART_ALL, REFMSG_END_EDIT);
	NotifyDependents(Interval(t, t), PART_ALL, REFMSG_MOD_DISPLAY_OFF);

	visualVectorMathDesc.EndEditParams(ip, this, flags, next);
	//ip->DeleteMode(moveMode);
	//if (moveMode) delete moveMode;
	//moveMode = NULL;

}

RefTargetHandle VisualVectorMath::Clone(RemapDir& remap)
{
	VisualVectorMath *mod = new VisualVectorMath();
	mod->ReplaceReference(PBLOCK_REF, remap.CloneRef(pblock));
	BaseClone(this, mod, remap);
	return mod;
}


void VisualVectorMath::ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node) 
{
}

Interval VisualVectorMath::LocalValidity(TimeValue t)
{
	// to forces a cache to be built after previous modifier.
	if (TestAFlag(A_MOD_BEING_EDITED))
		return NEVER;

	Interval iv = FOREVER;
	//compute interval
	return iv;
}



int VisualVectorMath::Display(
	TimeValue t, INode* inode, ViewExp *vpt,
	int flagst, ModContext *mc)
{
	GraphicsWindow *gw = vpt->getGW();
	Point3 pt[4];
	int savedLimits;

	gw->setRndLimits((savedLimits = gw->getRndLimits()) & ~GW_ILLUM);
	Matrix3 tm(1);
	gw->setTransform(tm);


	gw->setRndLimits(savedLimits);
	return 0;
}

void VisualVectorMath::GetWorldBoundBox(
	TimeValue t, INode* inode, ViewExp *vpt,
	Box3& box, ModContext *mc)
{

	if (!vpt || !vpt->IsAlive())
	{
		// why are we here
		DbgAssert(!_T("Invalid viewport!"));
		return;
	}

}


RefTargetHandle VisualVectorMath::GetReference(int i)
{
	switch (i) {
	case PBLOCK_REF: return pblock;
	default: return NULL;
	}
}

void VisualVectorMath::SetReference(int i, RefTargetHandle rtarg)
{
	switch (i) {
	case PBLOCK_REF: pblock = (IParamBlock2*)rtarg; break;
	}
}


RefResult VisualVectorMath::NotifyRefChanged(
	const Interval& changeInt, RefTargetHandle hTarget,
	PartID& partID, RefMessage message, BOOL propagate)
{
	switch (message) {
	case REFMSG_CHANGE:
		if (editMod == this && pmapParam) pmapParam->Invalidate();
		break;
	default:
		break;
		
	}
	return REF_SUCCEED;
}


