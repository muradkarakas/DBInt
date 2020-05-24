/**
 * This file is part of Sodium Language project
 *
 * Copyright © 2020 Murad Karakaþ <muradkarakas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License v3.0
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 *	https://choosealicense.com/licenses/gpl-3.0/
 */

#pragma once

#include "db-interface.h"

 
// delay-loaded fail functions
DBInt_Connection  *	DBInt_CreateDBConnectionNotImplemented(HANDLE heapHandle, DBInt_SupportedDatabaseType dbType, const char * dbName, const char * userName, const char * password);
void				DBInt_DestroyDBConnectionNotImplemented(DBInt_Connection *mkDBConnection);

