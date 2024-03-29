/*
 * Copyright 2001-2016, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm <bpmagic@columbus.rr.com>
 *		Axel Dörfler, axeld@pinc-software.de
 */


/*!	Manages font families and styles */


#include "FontManager.h"

#include <new>

#include <Debug.h>

#include "FontFamily.h"


//#define TRACE_FONT_MANAGER
#ifdef TRACE_FONT_MANAGER
#	define FTRACE(x) printf x
#else
#	define FTRACE(x) ;
#endif


FT_Library gFreeTypeLibrary;

//	#pragma mark -


int
FontManagerBase::compare_font_families(const FontFamily* a, const FontFamily* b)
{
	return strcmp(a->Name(), b->Name());
}


//! Initializes FreeType if requested
FontManagerBase::FontManagerBase(bool init_freetype, const char* className)
	: BLooper(className),
	fFamilies(20),
	fNextID(0),
	fHasFreetypeLibrary(init_freetype)
{
	if (init_freetype == true)
		fInitStatus = FT_Init_FreeType(&gFreeTypeLibrary) == 0 ? B_OK : B_ERROR;
}


//! Shuts down FreeType if it was initialized
FontManagerBase::~FontManagerBase()
{
	for (int32 i = fFamilies.CountItems(); i-- > 0;)
		delete fFamilies.RemoveItemAt(i);

	fStyleHashTable.Clear();

	if (fHasFreetypeLibrary == true)
		FT_Done_FreeType(gFreeTypeLibrary);
}


/*!	\brief Finds and returns the first valid charmap in a font

	\param face Font handle obtained from FT_Load_Face()
	\return An FT_CharMap or NULL if unsuccessful
*/
FT_CharMap
FontManagerBase::_GetSupportedCharmap(const FT_Face& face)
{
	for (int32 i = 0; i < face->num_charmaps; i++) {
		FT_CharMap charmap = face->charmaps[i];

		switch (charmap->platform_id) {
			case 3:
				// if Windows Symbol or Windows Unicode
				if (charmap->encoding_id == 0 || charmap->encoding_id == 1)
					return charmap;
				break;

			case 1:
				// if Apple Unicode
				if (charmap->encoding_id == 0)
					return charmap;
				break;

			case 0:
				// if Apple Roman
				if (charmap->encoding_id == 0)
					return charmap;
				break;

			default:
				break;
		}
	}

	return NULL;
}



/*!	\brief Counts the number of font families available
	\return The number of unique font families currently available
*/
int32
FontManagerBase::CountFamilies()
{
	return fFamilies.CountItems();
}


/*!	\brief Counts the number of styles available in a font family
	\param family Name of the font family to scan
	\return The number of font styles currently available for the font family
*/
int32
FontManagerBase::CountStyles(const char *familyName)
{
	FontFamily *family = GetFamily(familyName);
	if (family != NULL)
		return family->CountStyles();

	return 0;
}


/*!	\brief Counts the number of styles available in a font family
	\param family Name of the font family to scan
	\return The number of font styles currently available for the font family
*/
int32
FontManagerBase::CountStyles(uint16 familyID)
{
	FontFamily *family = GetFamily(familyID);
	if (family != NULL)
		return family->CountStyles();

	return 0;
}


FontFamily*
FontManagerBase::FamilyAt(int32 index) const
{
	ASSERT(IsLocked());

	return fFamilies.ItemAt(index);
}


/*!	\brief Locates a FontFamily object by name
	\param name The family to find
	\return Pointer to the specified family or NULL if not found.
*/
FontFamily*
FontManagerBase::GetFamily(const char* name)
{
	if (name == NULL)
		return NULL;

	return _FindFamily(name);
}


FontFamily*
FontManagerBase::GetFamily(uint16 familyID) const
{
	FontKey key(familyID, 0);
	FontStyle* style = fStyleHashTable.Get(key);
	if (style != NULL)
		return style->Family();

	return NULL;
}


FontStyle*
FontManagerBase::GetStyleByIndex(const char* familyName, int32 index)
{
	FontFamily* family = GetFamily(familyName);
	if (family != NULL)
		return family->StyleAt(index);

	return NULL;
}


FontStyle*
FontManagerBase::GetStyleByIndex(uint16 familyID, int32 index)
{
	FontFamily* family = GetFamily(familyID);
	if (family != NULL)
		return family->StyleAt(index);

	return NULL;
}


/*!	\brief Retrieves the FontStyle object
	\param family ID for the font's family
	\param style ID of the font's style
	\return The FontStyle having those attributes or NULL if not available
*/
FontStyle*
FontManagerBase::GetStyle(uint16 familyID, uint16 styleID) const
{
	ASSERT(IsLocked());

	FontKey key(familyID, styleID);
	return fStyleHashTable.Get(key);
}


/*!	\brief Retrieves the FontStyle object that comes closest to the one
		specified.

	\param family The font's family or NULL in which case \a familyID is used
	\param style The font's style or NULL in which case \a styleID is used
	\param familyID will only be used if \a family is NULL (or empty)
	\param styleID will only be used if \a style is NULL (or empty)
	\param face is used to specify the style if both \a style is NULL or empty
		and styleID is 0xffff.

	\return The FontStyle having those attributes or NULL if not available
*/
FontStyle*
FontManagerBase::GetStyle(const char* familyName, const char* styleName,
	uint16 familyID, uint16 styleID, uint16 face)
{
	ASSERT(IsLocked());

	FontFamily* family;

	// find family

	if (familyName != NULL && familyName[0])
		family = GetFamily(familyName);
	else
		family = GetFamily(familyID);

	if (family == NULL)
		return NULL;

	// find style

	if (styleName != NULL && styleName[0])
		return family->GetStyle(styleName);

	if (styleID != 0xffff)
		return family->GetStyleByID(styleID);

	// try to get from face
	return family->GetStyleMatchingFace(face);
}


/*!	\brief If you don't find your preferred font style, but are anxious
		to have one fitting your needs, you may want to use this method.
*/
FontStyle*
FontManagerBase::FindStyleMatchingFace(uint16 face) const
{
	int32 count = fFamilies.CountItems();

	for (int32 i = 0; i < count; i++) {
		FontFamily* family = fFamilies.ItemAt(i);
		FontStyle* style = family->GetStyleMatchingFace(face);
		if (style != NULL)
			return style;
	}

	return NULL;
}


/*!	\brief This call is used by the FontStyle class - and the FontStyle class
		only - to remove itself from the font manager.
	At this point, the style is already no longer available to the user.
*/
void
FontManagerBase::RemoveStyle(FontStyle* style)
{
	ASSERT(IsLocked());

	FontFamily* family = style->Family();
	if (family == NULL)
		debugger("family is NULL!");

	FontStyle* check = GetStyle(family->ID(), style->ID());
	if (check != NULL)
		debugger("style removed but still available!");

	if (family->RemoveStyle(style)
		&& family->CountStyles() == 0) {
		fFamilies.RemoveItem(family);
		delete family;
	}
}


FontFamily*
FontManagerBase::_FindFamily(const char* name) const
{
	if (name == NULL)
		return NULL;

	FontFamily family(name, 0);
	return const_cast<FontFamily*>(fFamilies.BinarySearch(family,
		compare_font_families));
}
