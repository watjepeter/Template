#include "VisualVectorMath.h"


// Loading/Saving
IOResult VisualVectorMath::Load(ILoad *iload)
{
	//TODO: Add code to allow plugin to load its data
	IOResult	res;
	ULONG		nb = 0;

	Modifier::Load(iload);

	while (IO_OK == (res = iload->OpenChunk()))
	{
		switch (iload->CurChunkID())
		{
/*
		case INITIALBASETM_DATA:
		{
			useOldLocalBaseTM = TRUE;
			res = oldLocalBaseTM.Load(iload);
			break;
		}

		//old data
		case INITIALMESHTM_DATA:
		{
			if (wrapMeshData.Count() == 0)
			{
				wrapMeshData.SetCount(1);
				wrapMeshData[0] = new WrapMeshData();
			}

			res = wrapMeshData[0]->initialMeshTM.Load(iload);
			break;
		}

*/
		default:
			break;
		}
		iload->CloseChunk();
		if (res != IO_OK) return res;
	}


	return IO_OK;
}



IOResult VisualVectorMath::Save(ISave *isave)
{
	//TODO: Add code to allow plugin to save its data
	IOResult	res = IO_OK;
	ULONG		nb  = 0;

	Modifier::Save(isave);

	/*
	isave->BeginChunk(NUMBEROFWRAPMESHES_DATA);
	int ct = wrapMeshData.Count();
	res = isave->Write(&ct, sizeof(ct), &nb);
	isave->EndChunk();
	*/

	return IO_OK;
}

IOResult VisualVectorMath::SaveLocalData(ISave *isave, LocalModData *pld)
{
	VisualVectorMathData *p;
	IOResult	res = IO_OK;
	ULONG		nb = 0;

	p = (VisualVectorMathData*)pld;

	/*
	isave->BeginChunk(MESHCOUNT_DATA);
	ct = p->numFaces;
	res = isave->Write(&ct, sizeof(ct), &nb);
	isave->EndChunk();
	*/
	return IO_OK;
}
IOResult VisualVectorMath::LoadLocalData(ILoad *iload, LocalModData **pld)
{
	IOResult	res = IO_OK;
	ULONG		nb = 0;

	VisualVectorMathData *p = new VisualVectorMathData();
	
	while (IO_OK == (res = iload->OpenChunk()))
	{
		switch (iload->CurChunkID())
		{
/*
			case MESHCOUNT_DATA:
			{
				int ct;
				iload->Read(&ct, sizeof(int), &nb);
				p->numFaces = ct;
				break;
			}
*/
			default:
				break;
		}
		iload->CloseChunk();
		if (res != IO_OK) return res;
	}

	*pld = p;


	return IO_OK;

}



