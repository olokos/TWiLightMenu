#include "date.h"

#include "graphics/fontHandler.h"

#include <ctime>
#include <cstdio>
#include <malloc.h>

#include <string>
#include "common/tonccpy.h"

extern bool show12hrClock;

using std::string;

tm iTimeParts;

/**
 * Get the current date as a C string.
 * @param format Date format.
 * @param buf Output buffer.
 * @param size Size of the output buffer.
 * @return Number of bytes written, excluding the NULL terminator.
 * @return Current date. (Caller must free() this string.)
 */
size_t GetDate(DateFormat format, char *buf, size_t size)
{
	time_t Raw;
	time(&Raw);
	const struct tm *Time = localtime(&Raw);

	switch (format) {
		case FORMAT_YDM:
			return strftime(buf, size, "%Y-%d-%m_%k-%M", Time);
		case FORMAT_YMD:
			return strftime(buf, size, "%Y-%m-%d_%k-%M", Time);
		case FORMAT_DM:
			return strftime(buf, size, "%d/%m", Time); // Ex: 26/12
		case FORMAT_MD:
			return strftime(buf, size, "%m/%d", Time); // Ex: 12/26
		case FORMAT_M_D:
			return strftime(buf, size, "%d.%m.", Time); // Ex: 26.12.
		case FORMAT_MY:
			return strftime(buf, size, "%m  %Y", Time);
		case FORMAT_M:
			return strftime(buf, size, "%m", Time);
		case FORMAT_Y:
			return strftime(buf, size, "%Y", Time);
		default:
			break;
	}

	// Invalid format.
	// Should not get here...
	if (size > 0) {
		*buf = 0;
	}
	return 0;
}

/**
 * Get the current time formatted for the top bar.
 * @return std::string containing the time.
 */
string RetTime()
{
	time_t epochTime;
  if(time(&epochTime)==(time_t)-1)
  {
    toncset(&iTimeParts,0,sizeof(iTimeParts));
  }
  else
  {
    localtime_r(&epochTime,&iTimeParts);
  }

    u8 hours = iTimeParts.tm_hour;
    u8 minutes = iTimeParts.tm_min;
    if (show12hrClock)
    {
        if (hours > 12)
            hours -= 12;
        if (hours == 0)
            hours = 12;
    }

    u8 number1 = hours / 10;
    u8 number2 = hours % 10;
    u8 number3 = minutes / 10;
    u8 number4 = minutes % 10;

	char Tmp[24];
	snprintf(Tmp, sizeof(Tmp), "%i%i : %i%i", number1, number2, number3, number4);

	return string(Tmp);
}

/**
 * Draw the date using the specified format.
 * @param Xpos X position.
 * @param Ypos Y position.
 * @param size Text size.
 * @param format Date format.
 */
char* DrawDateF(DateFormat format)
{
	char date_str[24];
	GetDate(format, date_str, sizeof(date_str));
	if (date_str[0] == 0)
		return "";
	return date_str;
}

/**
 * Draw the date.
 * Date format depends on language setting.
 * @param screen Top or Bottom screen.
 * @param Xpos X position.
 * @param Ypos Y position.
 * @param size Text size.
 */
char* DrawDate()
{
	// Date formats.
	// - Index: Language ID.
	// - Value: Date format.
	static const uint8_t date_fmt[8] = {
		FORMAT_MD,	// Japanese
		FORMAT_MD,	// English
		FORMAT_DM,	// French
		FORMAT_M_D,	// German
		FORMAT_DM,	// Italian
		FORMAT_DM,	// Spanish
		FORMAT_MD,	// Chinese
		FORMAT_MD,	// Korean
	};

	return DrawDateF((DateFormat)date_fmt[PersonalData->language]);
}
