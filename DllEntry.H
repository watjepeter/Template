/**********************************************************************
 *<
	FILE: bevelmod.h

	DESCRIPTION:

	CREATED BY: Steve Anderson (based on mods.h)

	HISTORY:

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/

#pragma once

#include "Max.h"
#include "iparamm2.h"
#include "resource.h"

#define MIN_FLOAT		float(-1.0E30)
#define MAX_FLAOT		float(1.0E30)

#define ScriptPrint (the_listener->edit_stream->printf)

extern ClassDesc2 *GetVisualVectorMathDesc();

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;




