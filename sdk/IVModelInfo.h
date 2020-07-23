#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/engine/ivmodelinfo.h

struct mstudiobbox_t
{
	int		bone;				// 0x0000 
	int		group;				// 0x0004 
	Vector3D	min;				// 0x0008 
	Vector3D	max;				// 0x0014 
	int		nameidx;			// 0x0020 
	char	pad_0x0024[0xC];	// 0x0024
	float	radius;				// 0x0030 
	char	pad_0x0034[0x10];	// 0x0034
};
struct mstudiohitboxset_t
{
	int                                             sznameindex;
	inline char* const              GetName(void) const { return ((char*)this) + sznameindex; }
	int                                             numhitboxes;
	int                                             hitboxindex;
	inline mstudiobbox_t* GetHitbox(int i) const { return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i; };
};
struct mstudiobone_t
{
	int                                     sznameindex;
	inline char* const GetName(void) const { return ((char*)this) + sznameindex; }
	int                                     parent;
	int                                     bonecontroller[6];

	Vector3D                          pos;
	float                           quat[4];
	Vector3D                          rot;
	Vector3D                          posscale;
	Vector3D                          rotscale;

	matrix3x4_t                     poseToBone;
	float                           qAlignment[4];
	int                                     flags;
	int                                     proctype;
	int                                     procindex;              // procedural rule
	mutable int                     physicsbone;    // index into physically simulated bone
	inline void* GetProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((BYTE*)this) + procindex); };
	int                                     surfacepropidx; // index into string tablefor property name
	inline char* const GetSurfaceProps(void) const { return ((char*)this) + surfacepropidx; }
	int                                     contents;               // See BSPFlags.h for the contents flags

	int                                     unused[8];              // remove as appropriate
};
struct studiohdr_t
{
	int                                     id;
	int                                     version;

	int                                     checksum;

	char                            name[64];
	int                                     Length;


	Vector3D                          eyeposition;

	Vector3D                          illumposition;

	Vector3D                          hull_min;
	Vector3D                          hull_max;

	Vector3D                          view_bbmin;
	Vector3D                          view_bbmax;

	int                                     flags;

	int                                     numbones;
	int                                     boneindex;

	inline mstudiobone_t* GetBone(int i) const { return (mstudiobone_t*)(((BYTE*)this) + boneindex) + i; };
	//	inline mstudiobone_t *pBone(int i) const { Assert(i >= 0 && i < numbones); return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };

	int                                     numbonecontrollers;
	int                                     bonecontrollerindex;

	int                                     numhitboxsets;
	int                                     hitboxsetindex;

	mstudiohitboxset_t* GetHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((BYTE*)this) + hitboxsetindex) + i;
	}

	inline mstudiobbox_t* GetHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet(set);

		if (!s)
			return NULL;

		return s->GetHitbox(i);
	}

	inline int GetHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet(set);

		if (!s)
			return 0;

		return s->numhitboxes;
	}

	int                                     numlocalanim;
	int                                     localanimindex;

	int                                     numlocalseq;
	int                                     localseqindex;

	mutable int                     activitylistversion;
	mutable int                     eventsindexed;

	int                                     numTextures;
	int                                     Textureindex;

	int                                     numcdTextures;
	int                                     cdTextureindex;

	int                                     numskinref;
	int                                     numskinfamilies;
	int                                     skinindex;

	int                                     numbodyparts;
	int                                     bodypartindex;

	int                                     numlocalattachments;
	int                                     localattachmentindex;

	int                                     numlocalnodes;
	int                                     localnodeindex;
	int                                     localnodenameindex;

	int                                     numflexdesc;
	int                                     flexdescindex;

	int                                     numflexcontrollers;
	int                                     flexcontrollerindex;

	int                                     numflexrules;
	int                                     flexruleindex;

	int                                     numikchains;
	int                                     ikchainindex;

	int                                     nummouths;
	int                                     mouthindex;

	int                                     numlocalposeparameters;
	int                                     localposeparamindex;

	int                                     surfacepropindex;

	int                                     keyvalueindex;
	int                                     keyvaluesize;


	int                                     numlocalikautoplaylocks;
	int                                     localikautoplaylockindex;

	float                           mass;
	int                                     contents;

	int                                     numincludemodels;
	int                                     includemodelindex;

	mutable void* virtualModel;

	int                                     szanimblocknameindex;
	int                                     numanimblocks;
	int                                     animblockindex;

	mutable void* animblockModel;

	int                                     bonetablebynameindex;

	void* pVertexBase;
	void* pIndexBase;

	BYTE                            constdirectionallightdot;

	BYTE                            rootLOD;

	BYTE                            numAllowedRootLODs;

	BYTE                            unused[1];

	int                                     unused4;

	int                                     numflexcontrollerui;
	int                                     flexcontrolleruiindex;
	float                           flVertAnimFixedPointScale;
	int                                     unused3[1];
	int                                     studiohdr2index;
	int                                     unused2[1];
};
struct model_t;
class IVModelInfo {
public:
	// returns name of model
	virtual_fn( get_model_name( const void* model ), 3, const char* ( __thiscall* )( void*, const void* ), model ); // 3

	inline void* get_model(int Index) {
		using Fn = void* (__thiscall*)(IVModelInfo*, int);
		return (*(Fn**)this)[1](this, Index);
	}
	int	GetModelIndex(const char* name)
	{
		using Fn = int(__thiscall*)(IVModelInfo*, const char*);
		return (*(Fn**)this)[2](this, name);
	}
	studiohdr_t* GetStudioModel(const model_t* mod)
	{
		using Fn = studiohdr_t * (__thiscall*)(IVModelInfo*, const model_t*);
		return (*(Fn**)this)[32](this, mod);
	}
};