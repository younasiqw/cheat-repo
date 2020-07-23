#pragma once
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/materialsystem/imaterialsystem.h
#include "../key_values.h"
class IMaterial;
class IWorldRenderList;
typedef unsigned short MaterialHandle_t;
class IMaterialSystem {
public:
	// returns a particular material
	virtual_fn( get_material( const char* material_name, const char* Texture_group_name, bool complain = true, const char* complain_prefix = nullptr ), 84, IMaterial* ( __thiscall* )( void*, const char*, const char*, bool, const char* ), material_name, Texture_group_name, complain, complain_prefix );

	virtual_fn(first_material( void ), 86, MaterialHandle_t (__thiscall*)(void*));

	virtual_fn(invalid_material(void), 88, MaterialHandle_t (__thiscall*)(void*));

	virtual_fn(get_material(MaterialHandle_t material), 89, IMaterial* (__thiscall*)(void*, MaterialHandle_t), material);

	virtual_fn(next_material(MaterialHandle_t material), 87, MaterialHandle_t (__thiscall*)(void*, MaterialHandle_t), material);

	virtual_fn(create_material(const char* material_name, KeyValues* vmt_key_values), 83, IMaterial*(__thiscall*)(void*, const char*, KeyValues*), material_name, vmt_key_values);

};

class IVRenderView
{
public:

	// Draw normal brush model.
	// If pMaterialOverride is non-null, then all the faces of the bmodel will
	// set this material rather than their regular material.
	virtual void			DrawBrushModel(
		C_CSPlayer* baseentity,
		model_t* model,
		const Vector3D& origin,
		const Vector3D& angles,
		bool bUnused) = 0;

	// Draw brush model that has no origin/angles change ( uses identity transform )
	// FIXME, Material proxy IClientEntity *baseentity is unused right now, use DrawBrushModel for brushes with
	//  proxies for now.
	virtual void			DrawIdentityBrushModel(IWorldRenderList* pList, model_t* model) = 0;

	// Mark this dynamic light as having changed this frame ( so light maps affected will be recomputed )
	virtual void			TouchLight(struct dlight_t* light) = 0;
	// Draw 3D Overlays
	virtual void			Draw3DDebugOverlays(void) = 0;
	// Sets Global blending fraction
	virtual void			SetBlend(float blend) = 0;
	virtual float			GetBlend(void) = 0;

	// Sets Global color modulation
	virtual void			SetColorModulation(float const* blend) = 0;
	virtual void			GetColorModulation(float* blend) = 0;
};

/**typedef void* FileHandle_t;
typedef void* GetSymbolProc_t;

// single byte identifies a xbox kv file in binary format
// strings are pooled from a searchpath/zip mounted symbol table
#define KV_BINARY_POOLED_FORMAT 0xAA


#define FOR_EACH_SUBKEY(kvRoot, kvSubKey) \
	for (KeyValues * kvSubKey = kvRoot->GetFirstSubKey(); kvSubKey != NULL; kvSubKey = kvSubKey->GetNextKey())

#define FOR_EACH_TRUE_SUBKEY(kvRoot, kvSubKey) \
	for (KeyValues * kvSubKey = kvRoot->GetFirstTrueSubKey(); kvSubKey != NULL; kvSubKey = kvSubKey->GetNextTrueSubKey())

#define FOR_EACH_VALUE(kvRoot, kvValue) \
	for (KeyValues * kvValue = kvRoot->GetFirstValue(); kvValue != NULL; kvValue = kvValue->GetNextValue())

class KeyValues
{
public:
	static void SetUseGrowableStringTable(bool bUseGrowableTable);

	KeyValues(const char* setName) {}
	class AutoDelete
	{
	public:
		explicit inline AutoDelete(KeyValues* pKeyValues) : m_pKeyValues(pKeyValues) {}
		explicit inline AutoDelete(const char* pchKVName) : m_pKeyValues(new KeyValues(pchKVName)) {}
		inline ~AutoDelete(void) { if (m_pKeyValues) m_pKeyValues->deleteThis(); }
		inline void Assign(KeyValues* pKeyValues) { m_pKeyValues = pKeyValues; }
		KeyValues* operator->() { return m_pKeyValues; }
		operator KeyValues* () { return m_pKeyValues; }
	private:
		AutoDelete(AutoDelete const& x); // forbid
		AutoDelete& operator= (AutoDelete const& x); // forbid
		KeyValues* m_pKeyValues;
	};
	KeyValues(const char* setName, const char* firstKey, const char* firstValue);
	KeyValues(const char* setName, const char* firstKey, const wchar_t* firstValue);
	KeyValues(const char* setName, const char* firstKey, int firstValue);
	KeyValues(const char* setName, const char* firstKey, const char* firstValue, const char* secondKey, const char* secondValue);
	KeyValues(const char* setName, const char* firstKey, int firstValue, const char* secondKey, int secondValue);
	const char* GetName() const;
	void SetName(const char* setName);
	int GetNameSymbol() const { return m_iKeyName; }
	void UsesEscapeSequences(bool state); // default false
	void UsesConditionals(bool state); // default true
	bool LoadFromFile(void* filesystem, const char* resourceName, const char* pathID = NULL);
	bool SaveToFile(void* filesystem, const char* resourceName, const char* pathID = NULL, bool sortKeys = false, bool bAllowEmptyString = false);
	bool LoadFromBuffer(char const* resourceName, const char* pBuffer, void* pFileSystem = NULL, const char* pPathID = NULL);
	bool LoadFromBuffer(char const* resourceName, void* buf, void* pFileSystem = NULL, const char* pPathID = NULL);
	KeyValues* FindKey(const char* keyName, bool bCreate = false);
	KeyValues* FindKey(int keySymbol) const;
	KeyValues* CreateNewKey();		// creates a new key, with an autogenerated name.  name is guaranteed to be an integer, of value 1 higher than the highest other integer key name
	void AddSubKey(KeyValues* pSubkey);	// Adds a subkey. Make sure the subkey isn't a child of some other keyvalues
	void RemoveSubKey(KeyValues* subKey);	// removes a subkey from the list, DOES NOT DELETE IT
	KeyValues* GetFirstSubKey() { return m_pSub; }	// returns the first subkey in the list
	KeyValues* GetNextKey() { return m_pPeer; }		// returns the next subkey
	void SetNextKey(KeyValues* pDat);
	KeyValues* FindLastSubKey();	// returns the LAST subkey in the list.  This requires a linked list iteration to find the key.  Returns NULL if we don't have any children
	KeyValues* GetFirstTrueSubKey();
	KeyValues* GetNextTrueSubKey();
	KeyValues* GetFirstValue();	// When you get a value back, you can use GetX and pass in NULL to get the value.
	KeyValues* GetNextValue();
	int   GetInt(const char* keyName = NULL, int defaultValue = 0);
	uint64_t GetUint64(const char* keyName = NULL, uint64_t defaultValue = 0);
	float GetFloat(const char* keyName = NULL, float defaultValue = 0.0f);
	const char* GetString(const char* keyName = NULL, const char* defaultValue = "");
	const wchar_t* GetWString(const char* keyName = NULL, const wchar_t* defaultValue = L"");
	void* GetPtr(const char* keyName = NULL, void* defaultValue = (void*)0);
	bool GetBool(const char* keyName = NULL, bool defaultValue = false);
	color GetColor(const char* keyName = NULL /* default value is all black );
	bool  IsEmpty(const char* keyName = NULL);
	int   GetInt(int keySymbol, int defaultValue = 0);
	float GetFloat(int keySymbol, float defaultValue = 0.0f);
	const char* GetString(int keySymbol, const char* defaultValue = "");
	const wchar_t* GetWString(int keySymbol, const wchar_t* defaultValue = L"");
	void* GetPtr(int keySymbol, void* defaultValue = (void*)0);
	color GetColor(int keySymbol /* default value is all black );
	bool  IsEmpty(int keySymbol);
	void SetWString(const char* keyName, const wchar_t* value);
	void SetString(const char* keyName, const char* value);
	void SetInt(const char* keyName, int value);
	void SetUint64(const char* keyName, uint64_t value);
	void SetFloat(const char* keyName, float value);
	void SetPtr(const char* keyName, void* value);
	void SetColor(const char* keyName, color value);
	void SetBool(const char* keyName, bool value) { SetInt(keyName, value ? 1 : 0); }
	void ChainKeyValue(KeyValues* pChain);
	void RecursiveSaveToFile(void* buf, int indentLevel, bool sortKeys = false, bool bAllowEmptyString = false);
	bool WriteAsBinary(void* buffer);
	bool ReadAsBinary(void* buffer, int nStackDepth = 0);
	KeyValues* MakeCopy(void) const;
	void CopySubkeys(KeyValues* pParent) const;
	void Clear(void);
	enum types_t
	{
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_NUMTYPES,
	};
	types_t GetDataType(const char* keyName = NULL);
	void deleteThis();
	void SetStringValue(char const* strValue);
	void UnpackIntoStructure(struct KeyValuesUnpackStructure const* pUnpackTable, void* pDest, size_t DestSizeInBytes);
	bool ProcessResolutionKeys(const char* pResString);
	bool Dump(class IKeyValuesDumpConText* pDump, int nIndentLevel = 0);
	void RecursiveMergeKeyValues(KeyValues* baseKV);
private:
	KeyValues(KeyValues&);
	~KeyValues();
	KeyValues* CreateKey(const char* keyName);
	KeyValues* CreateKeyUsingKnownLastChild(const char* keyName, KeyValues* pLastChild);
	void AddSubkeyUsingKnownLastChild(KeyValues* pSubKey, KeyValues* pLastChild);
	void RecursiveCopyKeyValues(KeyValues& src);
	void RemoveEverything();
	void RecursiveSaveToFile(void* filesystem, FileHandle_t f, void* pBuf, int indentLevel, bool sortKeys, bool bAllowEmptyString);
	void SaveKeyToFile(KeyValues* dat, void* filesystem, FileHandle_t f, void* pBuf, int indentLevel, bool sortKeys, bool bAllowEmptyString);
	void WriteConvertedString(void* filesystem, FileHandle_t f, void* pBuf, const char* pszString);
	void RecursiveLoadFromBuffer(char const* resourceName, void* buf);
	void AppendIncludedKeys(void* includedKeys);
	void ParseIncludedKeys(char const* resourceName, const char* filetoinclude,
		void* pFileSystem, const char* pPathID, void* includedKeys);
	void MergeBaseKeys(void* baseKeys);
	void InternalWrite(void* filesystem, FileHandle_t f, void* pBuf, const void* pData, int len);
	void Init();
	const char* ReadToken(void* buf, bool& wasQuoted, bool& wasConditional);
	void WriteIndents(void* filesystem, FileHandle_t f, void* pBuf, int indentLevel);
	void FreeAllocatedValue();
	void AllocateValueBlock(int size);
	int m_iKeyName;
	char* m_sValue;
	wchar_t* m_wsValue;
	union
	{
		int m_iValue;
		float m_flValue;
		void* m_pValue;
		unsigned char m_Color[4];
	};
	char	   m_iDataType;
	char	   m_bHasEscapeSequences; // true, if while parsing this KeyValue, Escape Sequences are used (default false)
	char	   m_bEvaluateConditionals; // true, if while parsing this KeyValue, conditionals blocks are evaluated (default true)
	char	   unused[1];
	KeyValues* m_pPeer;	// pointer to next key in list
	KeyValues* m_pSub;	// pointer to Start of a new sub key list
	KeyValues* m_pChain;// Search here if it's not in our list

private:
	static int(*s_pfGetSymbolForString)(const char* name, bool bCreate);
	static const char* (*s_pfGetStringForSymbol)(int symbol);
	static void* s_pGrowableStringTable;

public:
	// Functions that invoke the default behavior
	static int GetSymbolForStringClassic(const char* name, bool bCreate = true);
	static const char* GetStringForSymbolClassic(int symbol);

	// Functions that use the growable string table
	static int GetSymbolForStringGrowable(const char* name, bool bCreate = true);
	static const char* GetStringForSymbolGrowable(int symbol);

	// Functions to get external access to whichever of the above functions we're going to call.
	static int CallGetSymbolForString(const char* name, bool bCreate = true) { return s_pfGetSymbolForString(name, bCreate); }
	static const char* CallGetStringForSymbol(int symbol) { return s_pfGetStringForSymbol(symbol); }
}; */