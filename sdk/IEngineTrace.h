#pragma once
struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

struct cplane_t
{
	Vector3D normal;
	float dist;
	byte type;
	byte signbits;
	byte pad[2];
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY, // NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY, // NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS, // NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};
class CTraceFilterWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(void* /*pServerEntity*/, int /*contentsMask*/)
	{
		return false;
	}
	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_WORLD_ONLY;
	}
};
class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle != pSkip1);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip1;
};
class CTraceFilterOneEntity : public ITraceFilter
{
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle == pEntity);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pEntity;
};
enum contents {
	contents_empty = 0x0,
	contents_solid = 0x1,
	contents_window = 0x2,
	contents_aux = 0x4,
	contents_grate = 0x8,
	contents_slime = 0x10,
	contents_water = 0x20,
	contents_blocklos = 0x40,
	contents_opaque = 0x80,
	contents_testfogvolume = 0x100,
	contents_unused = 0x200,
	contents_blocklight = 0x400,
	contents_team1 = 0x800,
	contents_team2 = 0x1000,
	contents_ignore_nodraw_opaque = 0x2000,
	contents_moveable = 0x4000,
	contents_areaportal = 0x8000,
	contents_playerclip = 0x10000,
	contents_monsterclip = 0x20000,
	contents_current0 = 0x40000,
	contents_current90 = 0x80000,
	contents_current180 = 0x100000,
	contents_current270 = 0x200000,
	contents_current_up = 0x400000,
	contents_current_down = 0x800000,
	contents_origin = 0x1000000,
	contents_monster = 0x2000000,
	contents_debris = 0x4000000,
	contents_detail = 0x8000000,
	contents_translucent = 0x10000000,
	contents_ladder = 0x20000000,
	contents_hitbox = 0x40000000,

	last_visible_contents = contents_opaque,
	all_visible_contents = last_visible_contents | last_visible_contents - 1
};

enum mask {
	mask_all = 0xFFFFFFFF,
	mask_solid = (contents_solid | contents_moveable | contents_window | contents_monster | contents_grate),
	mask_playersolid = (contents_solid | contents_moveable | contents_playerclip | contents_window | contents_monster | contents_grate),
	mask_npcsolid = (contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster | contents_grate),
	mask_npcfluid = (contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster),
	mask_water = (contents_water | contents_moveable | contents_slime),
	mask_opaque = (contents_water | contents_moveable | contents_opaque),
	mask_opaque_npc = (mask_opaque | contents_monster),
	mask_blocklos = (contents_solid | contents_moveable | contents_slime),
	mask_blocklos_npc = (mask_blocklos | contents_monster),
	mask_visible = (mask_opaque | contents_ignore_nodraw_opaque),
	mask_visible_npc = (mask_opaque_npc | contents_ignore_nodraw_opaque),
	mask_shot = (contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_hitbox),
	mask_shot_brushonly = (contents_solid | contents_moveable | contents_window | contents_debris),
	mask_shot_hull = (contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate),
	mask_shot_portal = (contents_solid | contents_moveable | contents_window | contents_monster),
	mask_solid_brushonly = (contents_solid | contents_moveable | contents_window | contents_grate),
	mask_playersolid_brushonly = (contents_solid | contents_moveable | contents_window | contents_playerclip | contents_grate),
	mask_npcsolid_brushonly = (contents_solid | contents_moveable | contents_window | contents_monsterclip | contents_grate),
	mask_npcworldstatic = (contents_solid | contents_window | contents_monsterclip | contents_grate),
	mask_npcworldstatic_fluid = (contents_solid | contents_window | contents_monsterclip),
	mask_splitareaportal = (contents_water | contents_slime),
	mask_current = (contents_current0 | contents_current90 | contents_current180 | contents_current270 | contents_current_up | contents_current_down),
	mask_deadsolid = (contents_solid | contents_playerclip | contents_window | contents_grate)
};

struct C_Ray
{
	Vector3Daligned m_Start; // starting point, centered within the extents
	Vector3Daligned m_Delta; // direction + length of the ray
	Vector3Daligned m_StartOffset; // Add this to m_Start to get the actual ray start
	Vector3Daligned m_Extents; // Describes an axis aligned box extruded along a ray
	const matrix3x4_t* m_pWorldAxisTransform;
	//const matrix3x4_t *m_pWorldAxisTransform;
	bool m_IsRay; // are the extents zero?
	bool m_IsSwept; // is delta != 0?


	C_Ray( ) : m_pWorldAxisTransform( NULL ) {}

	C_Ray(Vector3D _start, Vector3D _end)
	{
		Init(_start, _end);
	}

	C_Ray(Vector3D _start, Vector3D _end, Vector3D _mins, Vector3D _maxs)
	{
		Init(_start, _end, _mins, _maxs);
	}

	void Init(const Vector3D& vecStart, const Vector3D& vecEnd)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.length_sqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;
		m_IsRay = true;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

		m_Start = vecStart;
	}

	void Init(Vector3D& vecStart, Vector3D& vecEnd, Vector3D min, Vector3D max)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.length_sqr() != 0);

		m_Extents.x = (max.x - min.x);
		m_Extents.y = (max.y - min.y);
		m_Extents.z = (max.z - min.z);
		m_IsRay = false;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

		m_Start = vecStart + ((max + min) * 0.5f);
	}
};

struct C_Trace;
//
class IEngineTrace
{
public:
	virtual int		GetPointContents(const Vector3D& vecAbsPosition, int contentsMask = mask_all, IHandleEntity** ppEntity = 0) = 0;
	virtual int		GetPointContents_WorldOnly(const Vector3D& vecAbsPosition, int contentsMask = mask_all) = 0;
	virtual int		GetPointContents_Collideable() = 0;
	virtual void	ClipRayToEntity(const C_Ray& uRay, int fMask, C_CSPlayer* pEntity, C_Trace* pTrace) = 0;
	virtual void	ClipRayToCollideable() = 0;
	virtual void	TraceRay(const C_Ray& ray, unsigned int fMask, ITraceFilter* pTraceFilter, C_Trace* pTrace) = 0;
};


