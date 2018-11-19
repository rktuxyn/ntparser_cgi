/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#pragma once
#if !defined(_global_h)
#define _global_h
#define SUCCESS 1
#include <iostream>
#ifndef INCLUDE_V8_H_
#include <v8.h>
#endif // !INCLUDE_V8_H_
#if !defined(NTEMPLATE_COMP_)
#include <compiler.h>
#endif//!NTEMPLATE_COMP_
#if !defined(EXP_H)
#include <exp.h>
#endif//!EXP_H
#if !defined(_INC_STDIO)
#include <stdio.h>  /* defines FILENAME_MAX */
#endif//!_INC_STDIO
#ifndef _LIST_
#include <list>
#endif // !_LIST_
#ifndef _MAP_
#include <map>
#endif // !_MAP_
#ifndef _INC_STDLIB
#include <stdlib.h>
#endif // !_INC_STDLIB
#ifndef _REGEX_
#include <regex>
#endif// !_REGEX_
#endif//!_global_h
