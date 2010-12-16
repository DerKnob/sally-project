////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DateHelper.cpp
///
/// \brief	Implements the date helper class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.org/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DateHelper.h"

using namespace SallyAPI::Date;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string DateHelper::GetCurrentDateString(bool withMilliseconds)
///
/// \brief	Gets a current date string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	withMilliseconds	true to with milliseconds. 
///
/// \return	The current date string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string DateHelper::GetCurrentDateString(bool withMilliseconds)
{
	SYSTEMTIME		currentTime;

	GetLocalTime(&currentTime);

	return GetDateString(currentTime, withMilliseconds);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string DateHelper::GetDateString(SYSTEMTIME currentTime, bool withMilliseconds)
///
/// \brief	Gets a date string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	currentTime			Time of the current. 
/// \param	withMilliseconds	true to with milliseconds. 
///
/// \return	The date string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string DateHelper::GetDateString(SYSTEMTIME currentTime, bool withMilliseconds)
{
	std::string		time;
	char			temp[12];

	_itoa_s(currentTime.wYear, (LPSTR) temp, (size_t) 11, 10);
	time.append(temp);
	time.append("-");

	_itoa_s(currentTime.wMonth, (LPSTR) temp, (size_t) 11, 10);
	if (currentTime.wMonth < 10)
	{
		time.append("0");
	}
	time.append(temp);
	time.append("-");

	_itoa_s(currentTime.wDay, (LPSTR) temp, (size_t) 11, 10);
	if (currentTime.wDay < 10)
	{
		time.append("0");
	}
	time.append(temp);
	time.append(" ");

	_itoa_s(currentTime.wHour, (LPSTR) temp, (size_t) 11, 10);
	if (currentTime.wHour < 10)
	{
		time.append("0");
	}
	time.append(temp);
	time.append(":");

	_itoa_s(currentTime.wMinute, (LPSTR) temp, (size_t) 11, 10);
	if (currentTime.wMinute < 10)
	{
		time.append("0");
	}
	time.append(temp);
	time.append(":");

	_itoa_s(currentTime.wSecond, (LPSTR) temp, (size_t) 11, 10);
	if (currentTime.wSecond < 10)
	{
		time.append("0");
	}
	time.append(temp);

	if (withMilliseconds)
	{
		time.append("-");

		_itoa_s(currentTime.wMilliseconds, (LPSTR) temp, (size_t) 11, 10);
		if (currentTime.wMilliseconds < 100)
		{
			time.append("0");
		}
		if (currentTime.wMilliseconds < 10)
		{
			time.append("0");
		}
		time.append(temp);
	}

	return time;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool DateHelper::IsLeapYear(const UINT uJahr)
///
/// \brief	Query if 'uJahr' is leap year. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	uJahr	The u jahr. 
///
/// \return	true if leap year, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool DateHelper::IsLeapYear(const UINT uJahr)
{
	// Die Regel lautet: Alles, was durch 4 teilbar ist, ist ein Schaltjahr.
	// Es sei denn, das Jahr ist durch 100 teilbar, dann ist es keins.
	// Aber wenn es durch 400 teilbar ist, ist es doch wieder eins.

	if ((uJahr % 400) == 0)
		return true;
	else if ((uJahr % 100) == 0)
		return false;
	else if ((uJahr % 4) == 0)
		return true;

	return false;
}     

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	short DateHelper::GetDaysInMonth(const UINT uMonat, const UINT uJahr)
///
/// \brief	Gets the days in month. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	uMonat	The u monat. 
/// \param	uJahr	The u jahr. 
///
/// \return	The days in month. 
////////////////////////////////////////////////////////////////////////////////////////////////////

short DateHelper::GetDaysInMonth(const UINT uMonat, const UINT uJahr)
{
	//                     ungült,Jan,Feb,Mrz,Apr,Mai,Jun,Jul,Aug,Sep,Okt,Nov,Dez
	int arrTageImMonat[13] = {  0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (uMonat == 2)
	{
		// Februar: Schaltjahr unterscheiden
		if (IsLeapYear(uJahr))
			return 29;
		else
			return 28;
	}

	if ((uMonat >= 1) && (uMonat <= 12))
		return arrTageImMonat[uMonat];
	else
	{
		_ASSERT(0); // ungültiger Monat !
		return 0;
	}
}  

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	short DateHelper::GetDaysInYears(const UINT uJahr)
///
/// \brief	Gets the days in years. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	uJahr	The u jahr. 
///
/// \return	The days in years. 
////////////////////////////////////////////////////////////////////////////////////////////////////

short DateHelper::GetDaysInYears(const UINT uJahr)
{
	return (IsLeapYear(uJahr)) ? 366 : 365;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	short DateHelper::GetWeekday(const UINT uTag, const UINT uMonat, const UINT uJahr)
///
/// \brief	Gets a weekday. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	uTag	The u tag. 
/// \param	uMonat	The u monat. 
/// \param	uJahr	The u jahr. 
///
/// \return	The weekday. 
////////////////////////////////////////////////////////////////////////////////////////////////////

short DateHelper::GetWeekday(const UINT uTag, const UINT uMonat, const UINT uJahr)
{
	//                       ungült Jan Feb Mrz Apr Mai Jun Jul Aug Sep Okt Nov Dez 
	BYTE arrMonatsOffset[13] = {  0,  1,  4,  3,  6,  1,  4,  6,  2,  5,  0,  3,  5};

	BYTE cbJahrhundertOffset = 0;

	_ASSERT(uTag > 0);
	_ASSERT(uTag <= 31);
	_ASSERT(uMonat > 0);
	_ASSERT(uMonat <= 12);
	_ASSERT(uJahr >= 1600);

	// Wir können erst ab 1. März 1600 rechnen.
	// Monat / Tag - Plausi prüfen:
	if (   (uTag > 31) || (uMonat > 12) || (uMonat == 0) 
		|| (uTag == 0) || (uJahr < 1600))
		return -1;

	if ((uJahr == 1600) && (uMonat <= 2))
		return -1;

	// Jahrhundertoffset berechnen:
	if (  (uJahr < 1700) 
		||  ((uJahr == 1700) && (uMonat <= 2)) )
	{
		// 01.03.1600 - 28.02.1700
		cbJahrhundertOffset = 6;
	}
	else if (  (uJahr < 1800)
		||  ((uJahr == 1800) && (uMonat <= 2)) )
	{
		// 01.03.1700 - 28.02.1800
		cbJahrhundertOffset = 4;
	}
	else if (  (uJahr < 1900)
		||  ((uJahr == 1900) && (uMonat <= 2)) )
	{
		// 01.03.1800 - 28.02.1900
		cbJahrhundertOffset = 2;
	}
	else if (  (uJahr < 2000)
		||  ((uJahr == 2000) && (uMonat <= 2)) )
	{
		// 01.03.1900 - 29.02.2000
		cbJahrhundertOffset = 0;
	}
	else if (  (uJahr < 2100)
		||  ((uJahr == 2100) && (uMonat <= 2)) )
	{
		// 01.03.2000 - 28.02.2100
		cbJahrhundertOffset = 6;
	}
	else if (  (uJahr < 2200)
		||  ((uJahr == 2200) && (uMonat <= 2)) )
	{
		// 01.03.2100 - 28.02.2200
		cbJahrhundertOffset = 4;
	}
	else if (  (uJahr < 2300)
		||  ((uJahr == 2300) && (uMonat <= 2)) )
	{
		// 01.03.2200 - 28.02.2300
		cbJahrhundertOffset = 2;
	}
	else if (  (uJahr < 2400)
		||  ((uJahr == 2400) && (uMonat <= 2)) )
	{
		// 01.03.2400 - 29.02.2400
		cbJahrhundertOffset = 0;
	}

	UINT uLocalJahr = uJahr;

	if (uMonat <= 2)
		uLocalJahr--;

	UINT uJmod100 = uLocalJahr % 100;

	short nErgebnis = (uTag + uJmod100 + (uJmod100 / 4) 
		+ arrMonatsOffset[uMonat] + cbJahrhundertOffset) % 7;

	// Ergebnis:
	// 0 = Sonntag
	// 1 = Montag
	// 2 = Dienstag
	// 3 = Mittwoch
	// 4 = Donnerstag
	// 5 = Freitag
	// 6 = Samstag
	return nErgebnis;
}   

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	short DateHelper::GetDayOfYear(const UINT uTag, const UINT uMonat, const UINT uJahr)
///
/// \brief	Gets a day of year. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	uTag	The u tag. 
/// \param	uMonat	The u monat. 
/// \param	uJahr	The u jahr. 
///
/// \return	The day of year. 
////////////////////////////////////////////////////////////////////////////////////////////////////

short DateHelper::GetDayOfYear(const UINT uTag, const UINT uMonat, const UINT uJahr)
{
	// Der wievielte Tag des Jahres ist dieser Tag
	if ((uMonat == 0) || (uMonat > 12))
	{
		_ASSERT(0);
		return -1;
	}

	UINT uLokalTag = uTag;
	UINT uLokalMonat = uMonat;

	while (uLokalMonat > 1)
	{
		uLokalMonat--;
		uLokalTag += GetDaysInMonth(uLokalMonat, uJahr);
	}

	return uLokalTag;
}               

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	short DateHelper::GetCalendarWeek(const UINT uTag, const UINT uMonat, const UINT uJahr)
///
/// \brief	Gets a calendar week. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	uTag	The u tag. 
/// \param	uMonat	The u monat. 
/// \param	uJahr	The u jahr. 
///
/// \return	The calendar week. 
////////////////////////////////////////////////////////////////////////////////////////////////////

short DateHelper::GetCalendarWeek(const UINT uTag, const UINT uMonat, const UINT uJahr)
{
	// Berechnung erfolgt analog DIN 1355, welche besagt:
	// Der erste Donnerstag im neuen Jahr liegt immer in der KW 1.
	// "Woche" ist dabei definiert als [Mo, ..., So].

	short nTagDesJahres = GetDayOfYear(uTag, uMonat, uJahr);

	// Berechnen des Wochentags des 1. Januar:
	short nWochentag1Jan = GetWeekday(1, 1, uJahr) - 1; 

	// Sonderfälle Freitag und Samstag
	if (nWochentag1Jan >= 4) 
		nWochentag1Jan -= 7;

	// Sonderfälle "Jahresanfang mit KW - Nummer aus dem Vorjahr"
	// Danke an Frank Hamann von Delphi für den Hinweis :-)
	if ( (nTagDesJahres + nWochentag1Jan) <= 0)
		return GetCalendarWeek(31, 12, uJahr - 1);

	short nKalenderWoche = ((nTagDesJahres - 1 + nWochentag1Jan) / 7) + 1;

	_ASSERT(nKalenderWoche >= 1);
	_ASSERT(nKalenderWoche <= 53); 

	// 53 Kalenderwochen hat grundsätzlich nur ein Jahr, 
	// welches mit einem Donnerstag anfängt !
	// In Schaltjahren ist es auch mit einem Mittwoch möglich, z.B. 1992
	// Danke an Andreas Gut für den Hinweis :-)
	// Andernfalls ist diese KW schon die KW1 des Folgejahres.
	if (nKalenderWoche == 53)
	{
		if (  (nWochentag1Jan == 3)
			||  ((nWochentag1Jan == 2) && IsLeapYear(uJahr))
			)
		{
			; // Das ist korrekt und erlaubt
		}
		else
			nKalenderWoche = 1; // Korrektur des Wertes
	}

	return nKalenderWoche;
}   

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void DateHelper::GetEasterDate(const UINT uJahr, UINT & uTag, UINT & uMonat)
///
/// \brief	Gets an easter date. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	uJahr			The u jahr. 
/// \param [in,out]	uTag	The u tag. 
/// \param [in,out]	uMonat	The u monat. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void DateHelper::GetEasterDate(const UINT uJahr, UINT & uTag, UINT & uMonat)
{
	// Berechnet für ein beliebiges Jahr das Osterdatum.

	// Quelle des Gauss - Algorithmus: Stefan Gerth, 
	// "Die Gauß'sche Osterregel", Nürnberg, Februar 2003. 
	// http://krapfen.org/docs/Facharbeit/ostern.pdf

	UINT a = uJahr % 19;
	UINT b = uJahr %  4;
	UINT c = uJahr %  7;

	int k = uJahr / 100;
	int q = k / 4;
	int p = ((8 * k) + 13) / 25;
	UINT Egz = (38 - (k - q) + p) % 30; // Die Jahrhundertepakte
	UINT M = (53 - Egz) % 30;
	UINT N = (4 + k - q) % 7;

	UINT d = ((19 * a) + M) % 30;
	UINT e = ((2 * b) + (4 * c) + (6 * d) + N) % 7;

	// Ausrechnen des Ostertermins:
	if ((22 + d + e) <= 31)
	{
		uTag = 22 + d + e;
		uMonat = 3;
	}
	else
	{
		uTag = d + e - 9;
		uMonat = 4;

		// Zwei Ausnahmen berücksichtigen:
		if (uTag == 26)
			uTag = 19;
		else if ((uTag == 25) && (d == 28) && (a > 10))
			uTag = 18;
	}

	// Offsets für andere Feiertage:

	// Schwerdonnerstag / Weiberfastnacht -52
	// Rosenmontag -48
	// Fastnachtsdienstag -47
	// Aschermittwoch -46
	// Gründonnerstag -3
	// Karfreitag -2
	// Ostersonntag 0
	// Ostermontag +1
	// Christi Himmelfahrt +39
	// Pfingstsonntag +49
	// Pfingstmontag +50
	// Fronleichnam +60

	// Mariä Himmelfahrt ist stets am 15. August (Danke an Michael Plugge!)

}  

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void DateHelper::GetFourthAdvent(const UINT uJahr, UINT & uTag, UINT & uMonat)
///
/// \brief	Gets a fourth advent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	uJahr			The u jahr. 
/// \param [in,out]	uTag	The u tag. 
/// \param [in,out]	uMonat	The u monat. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void DateHelper::GetFourthAdvent(const UINT uJahr, UINT & uTag, UINT & uMonat)
{
	// Berechnet für ein beliebiges Jahr das Datum des 4. Advents-Sonntags.
	// Der 4. Adventssonntag ist stets der Sonntag vor dem 1. Weihnachtsfeiertag,
	// muß also stets in der Periode [18. - 24.12.] liegen:

	uMonat = 12; // Das steht jedes Jahr fest :-)

	short nWoTag = GetWeekday(24, 12, uJahr); // Wochentag des 24.12. ermitteln

	uTag = 24 - nWoTag;

	// Offsets: Der Buß- und Bettag liegt stets 32 Tage vor dem  4. Advent
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	long DateHelper::TimeDifferneceInYears(const SYSTEMTIME & Startzeit,
/// const SYSTEMTIME & Endezeit)
///
/// \brief	Time differnece in years. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	Startzeit	The startzeit. 
/// \param	Endezeit	The endezeit. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

long DateHelper::TimeDifferneceInYears(const SYSTEMTIME & Startzeit, 
								  const SYSTEMTIME & Endezeit)
{
	if (Endezeit.wMonth > Startzeit.wMonth)
		return Endezeit.wYear - Startzeit.wYear;

	if (Endezeit.wMonth < Startzeit.wMonth)
		return Endezeit.wYear - Startzeit.wYear - 1;

	// Monate sind identisch.

	if (Endezeit.wDay > Startzeit.wDay)
		return Endezeit.wYear - Startzeit.wYear;

	if (Endezeit.wDay < Startzeit.wDay)
		return Endezeit.wYear - Startzeit.wYear - 1;

	// Tag ist bei beiden identisch.

	if (Endezeit.wHour > Startzeit.wHour)
		return Endezeit.wYear - Startzeit.wYear;

	if (Endezeit.wHour < Startzeit.wHour)
		return Endezeit.wYear - Startzeit.wYear - 1;

	// Stunde ist identisch.

	if (Endezeit.wMinute > Startzeit.wMinute)
		return Endezeit.wYear - Startzeit.wYear;

	if (Endezeit.wMinute < Startzeit.wMinute)
		return Endezeit.wYear - Startzeit.wYear - 1;

	if (Endezeit.wSecond > Startzeit.wSecond)
		return Endezeit.wYear - Startzeit.wYear;

	if (Endezeit.wSecond < Startzeit.wSecond)
		return Endezeit.wYear - Startzeit.wYear - 1;

	if (Endezeit.wMilliseconds >= Startzeit.wMilliseconds)
		return Endezeit.wYear - Startzeit.wYear;
	else
		return Endezeit.wYear - Startzeit.wYear - 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	long DateHelper::TimeDifferenceInDays(const SYSTEMTIME & Startzeit,
/// const SYSTEMTIME & Endezeit)
///
/// \brief	Time difference in days. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	Startzeit	The startzeit. 
/// \param	Endezeit	The endezeit. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

long DateHelper::TimeDifferenceInDays(const SYSTEMTIME & Startzeit, 
								 const SYSTEMTIME & Endezeit)
{
	// Ist die Ende-Uhrzeit auch hinter der Startuhrzeit ? Das ist der Normalfall.
	// Andernfalls müssen wir nachher noch eins abziehen!

	long lAbzug = 0;

	if (  (Endezeit.wHour < Startzeit.wHour)
		||  ((Endezeit.wHour == Startzeit.wHour) 
		&& (Endezeit.wMinute  < Startzeit.wMinute))
		||  ((Endezeit.wHour == Startzeit.wHour) 
		&& (Endezeit.wMinute == Startzeit.wMinute) 
		&& (Endezeit.wSecond  < Startzeit.wSecond))
		||  ((Endezeit.wHour == Startzeit.wHour) 
		&& (Endezeit.wMinute == Startzeit.wMinute) 
		&& (Endezeit.wSecond == Startzeit.wSecond) 
		&& (Endezeit.wMilliseconds < Startzeit.wMilliseconds))
		)
	{
		lAbzug = 1;
	}

	// Im gleichen Jahr ? Dann nutzen wir "getTagDesJahres":
	if (Endezeit.wYear == Startzeit.wYear)
		return GetDayOfYear(Endezeit.wDay, Endezeit.wMonth, Endezeit.wYear) 
		- GetDayOfYear(Startzeit.wDay, Startzeit.wMonth, Startzeit.wYear) 
		- lAbzug;

	else if (Endezeit.wYear > Startzeit.wYear)
	{
		// Wir starten mit der Anzahl Tage im Endejahr:
		long lErgebnis = GetDayOfYear(Endezeit.wDay, Endezeit.wMonth, 
			Endezeit.wYear) - lAbzug;

		UINT uJahr = Endezeit.wYear - 1;

		// Jetzt summieren wir alle dazwischenliegenden Jahre:
		while (uJahr > Startzeit.wYear)
		{
			lErgebnis += GetDaysInYears(uJahr);
			uJahr--;
		}

		// und addieren schließlich noch die Anzahl der Tage aus dem Start - Jahr:
		lErgebnis += (GetDaysInYears(Startzeit.wYear) 
			- GetDayOfYear(Startzeit.wDay, 
			Startzeit.wMonth, Startzeit.wYear));

		return lErgebnis;
	}

	else
		return -1; // Die Endezeit liegt vor der Startzeit !

} 

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SYSTEMTIME DateHelper::TimeDifferenceInDays( const SYSTEMTIME & Startzeit,
/// const long & Endezeit )
///
/// \brief	Time difference in days. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	Startzeit	The startzeit. 
/// \param	Endezeit	The endezeit. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SYSTEMTIME DateHelper::TimeDifferenceInDays( const SYSTEMTIME & Startzeit, const long & Endezeit )
{
	SYSTEMTIME resultTime;
	resultTime = Startzeit;

	int i = Endezeit;
	while (i != 0)
	{
		short sDays = GetDaysInMonth(resultTime.wMonth, resultTime.wYear);

		if (sDays <= i)
		{
			i -= sDays;

			resultTime.wDay = 1;
			if (resultTime.wMonth == 12)
			{
				++resultTime.wYear;
				resultTime.wMonth = 1;
			}
			else
			{
				++resultTime.wMonth;
			}
		}
		else
		{
			resultTime.wDay += i;
			i -= i;
		}
	}

	return resultTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ULONGLONG DateHelper::GetMilliseconds()
///
/// \brief	Gets the milliseconds. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The milliseconds. 
////////////////////////////////////////////////////////////////////////////////////////////////////

ULONGLONG DateHelper::GetMilliseconds()
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	FILETIME fileTime;
	SystemTimeToFileTime(&systemTime, &fileTime);

	ULARGE_INTEGER uli;
	uli.LowPart = fileTime.dwLowDateTime;
	uli.HighPart = fileTime.dwHighDateTime;

	ULONGLONG systemTimeIn_ms(uli.QuadPart/10000);

	return systemTimeIn_ms; 
}