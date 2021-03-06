/*
 * Copyright 2003-2009, Axel Dörfler, axeld@pinc-software.de.
 * Distributed under the terms of the MIT License.
 */


#include <Language.h>

#include <iostream>

#include <Catalog.h>
#include <Locale.h>
#include <LocaleRoster.h>
#include <Path.h>
#include <String.h>
#include <FindDirectory.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <ICUWrapper.h>

#include <unicode/locid.h>


static const char *gBuiltInStrings[] = {
	"Yesterday",
	"Today",
	"Tomorrow",
	"Future",

	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",

	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",

	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December",

	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec",

	"^[yY]",
	"^[nN]",
	"yes",
	"no"
};


BLanguage::BLanguage(const char* language)
	:
	fDirection(B_LEFT_TO_RIGHT)
{
	fICULocale = new icu_4_2::Locale(language);

	for (int32 i = B_NUM_LANGUAGE_STRINGS;i-- > 0;)
		fStrings[i] = NULL;
}


BLanguage::~BLanguage()
{
	delete fICULocale;

	for (int32 i = B_NUM_LANGUAGE_STRINGS;i-- > 0;)
		free(fStrings[i]);
}


void
BLanguage::Default()
{
	fICULocale = new icu_4_2::Locale("en");
	fDirection = B_LEFT_TO_RIGHT;

	for (int32 i = B_NUM_LANGUAGE_STRINGS;i-- > 0;) {
		free(fStrings[i]);
		fStrings[i] = strdup(gBuiltInStrings[i]);
	}
}


uint8
BLanguage::Direction() const
{
	return fDirection;
}


const char*
BLanguage::GetString(uint32 id) const
{
	if (id < B_LANGUAGE_STRINGS_BASE
		|| id > B_LANGUAGE_STRINGS_BASE + B_NUM_LANGUAGE_STRINGS)
		return NULL;

	return fStrings[id - B_LANGUAGE_STRINGS_BASE];
}


status_t
BLanguage::GetName(BString& name) const
{
	UnicodeString string;
	fICULocale->getDisplayName(*fICULocale, string);

	name.Truncate(0);
	BStringByteSink converter(&name);
	string.toUTF8(converter);

	return B_OK;
}


status_t
BLanguage::GetTranslatedName(BString& name) const
{
	BMessage preferredLanguage;
	be_locale_roster->GetPreferredLanguages(&preferredLanguage);

	BString appLanguage;
	preferredLanguage.FindString("language", 0, &appLanguage);

	UnicodeString string;
	fICULocale->getDisplayName(Locale(appLanguage), string);

	name.Truncate(0);
	BStringByteSink converter(&name);
	string.toUTF8(converter);

	return B_OK;
}


const char*
BLanguage::Code() const
{
	return fICULocale->getLanguage();
}


const char*
BLanguage::Country() const
{
	const char* country = fICULocale->getCountry();
	if (country == NULL || country[0] == '\0')
		return NULL;

	return country;
}


const char*
BLanguage::Variant() const
{
	const char* variant = fICULocale->getVariant();
	if (variant == NULL || variant[0] == '\0')
		return NULL;

	return variant;
}


const char*
BLanguage::ID() const
{
	return fICULocale->getName();
}


bool
BLanguage::IsCountrySpecific() const
{
	return Country() != NULL;
}


bool
BLanguage::IsVariant() const
{
	return Variant() != NULL;
}
