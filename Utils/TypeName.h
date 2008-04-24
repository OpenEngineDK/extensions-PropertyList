/*
TypeName.h: resolves typenames to human readable strings

Copyright (C) 2007  Enrico Schnepel

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Usage:

instead of 

	int i=5;
	cout << typeid(i).name();

just write

	int i=5;
	cout << typeName(i);

which is
- shorter and
- smarter ;-)

*/

#ifndef TYPENAME_H_
#define TYPENAME_H_

#include <string>
#include <typeinfo>

#define TYPE_COMPARE_IF(t) if(*info == typeid(t)) return #t;else

#define TYPE_COMPARE_CONST(t) \
	TYPE_COMPARE_IF(t) \
	TYPE_COMPARE_IF(const t)

#define TYPE_COMPARE(t) \
	TYPE_COMPARE_CONST(t) \
	TYPE_COMPARE_CONST(t*) \
	TYPE_COMPARE_CONST(t**) \
	TYPE_COMPARE_CONST(t*&)
	
template<typename TYPE>
std::string typeName(TYPE type) {
	const std::type_info *info = &typeid(type);

	TYPE_COMPARE(std::string)

	TYPE_COMPARE(char)
	TYPE_COMPARE(short)
	TYPE_COMPARE(int)
	TYPE_COMPARE(long)

	TYPE_COMPARE(void)

	TYPE_COMPARE(float)
	TYPE_COMPARE(double)
	TYPE_COMPARE(long double)

	TYPE_COMPARE(unsigned char)
	TYPE_COMPARE(unsigned short)
	TYPE_COMPARE(unsigned int)
	TYPE_COMPARE(unsigned long)

	return (*info).name();
}

#undef TYPE_COMPARE
#undef TYPE_COMPARE_CONST
#undef TYPE_COMPARE_IF

#endif /*TYPENAME_H_*/
