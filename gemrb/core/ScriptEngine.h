/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2003 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 */

#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include "Plugin.h"
#include "System/String.h"

#include <cstdint>
#include <map>
#include <typeinfo>
#include <vector>

namespace GemRB {

class Point;

using ScriptingId = uint64_t;
using ScriptingClassId = std::string;
using ScriptingGroup_t = FixedSizeString<15, strnicmp>;
static_assert(sizeof(ScriptingGroup_t) <= 16, "Please try to keep this sensibly small. 16 bytes fits in 2 64bit registers.");

class ScriptingRefBase {
public:
	const ScriptingId Id; // unique id for each object in a ScriptingGroup

	explicit ScriptingRefBase(ScriptingId id)
	: Id(id) {}

	virtual ~ScriptingRefBase() = default;

	// key to separate groups of objects for faster searching and id collision prevention
	virtual const ScriptingGroup_t& ScriptingGroup() const=0;
	// class to instantiate on the script side (Python)
	virtual ScriptingClassId ScriptingClass() const=0;
};

template <class T>
class ScriptingRef : public ScriptingRefBase {
private:
	T* ref;
public:
	using RefType = T*;
	
	ScriptingRef(T* ref, ScriptingId id)
	: ScriptingRefBase(id), ref(ref) {}

	T* GetObject() const { return ref; }
};


class GEM_EXPORT ScriptEngine : public Plugin {
public:
	using ScriptingDefinitions = std::map<ScriptingId, const ScriptingRefBase*>;
	
private:
	using ScriptingDict = std::map<ScriptingGroup_t, ScriptingDefinitions>;
	static ScriptingDict GUIDict;

public:
	static bool RegisterScriptingRef(const ScriptingRefBase* ref);
	static bool UnregisterScriptingRef(const ScriptingRefBase* ref);

	static ScriptingDefinitions GetScriptingGroup(ScriptingGroup_t groupId)
	{
		return GUIDict[groupId];
	}

	static const ScriptingRefBase* GetScripingRef(ScriptingGroup_t group, ScriptingId id)
	{
		const ScriptingRefBase* ref = NULL;
		ScriptingDefinitions::iterator it = GUIDict[group].find(id);
		if (it != GUIDict[group].end()) {
			ref = it->second;
		}
		return ref;
	}

	class Parameter {
		struct TypeInterface {
			virtual ~TypeInterface() = default;
			virtual TypeInterface* Clone() const = 0;
			virtual const std::type_info& Type() const = 0;
		};

		template <typename T>
		struct ConcreteType : public TypeInterface {
			T value;
			explicit ConcreteType(T value) : value(value) {}

			TypeInterface *Clone() const override
			{
				return new ConcreteType(value);
			}

			const std::type_info& Type() const override {
				return typeid(T);
			}
		};

		TypeInterface* ptr;

	public:
		template <typename T>
		explicit Parameter(T value) {
			ptr = new ConcreteType<T>(value);
		}

		Parameter() : ptr(NULL) {}

		Parameter( const Parameter& s ) {
			ptr = (s.ptr) ? s.ptr->Clone() : NULL;
		}

		Parameter& Swap(Parameter& rhs) {
			std::swap(ptr, rhs.ptr);
			return *this;
		}

		Parameter& operator=(const Parameter& rhs) {
			Parameter tmp(rhs);
			return Swap(tmp);
		}

		~Parameter() {
			delete ptr;
		}

		const std::type_info& Type() const {
			return ptr ? ptr->Type() : typeid(void);
		}

		template <typename T>
		const T& Value() const {
			ConcreteType<T>* type = dynamic_cast<ConcreteType<T>*>(ptr);
			if (type) {
				return type->value;
			}
			// default
			static T t;
			return t;
		}
	};

	using FunctionParameters = std::vector<Parameter>;

	static const ScriptingId InvalidId = static_cast<ScriptingId>(-1);

public:
	ScriptEngine() = default;
	/** Initialization Routine */
	virtual bool Init(void) = 0;
	/** Load Script */
	virtual bool LoadScript(const char* filename) = 0;
	/** Run Function */
	virtual bool RunFunction(const char* Modulename, const char* FunctionName, const FunctionParameters& params, bool report_error = true) = 0;
	// TODO: eleminate these RunFunction variants.
	virtual bool RunFunction(const char *ModuleName, const char* FunctionName, bool report_error=true, int intparam=-1) = 0;
	virtual bool RunFunction(const char* Modulename, const char* FunctionName, bool report_error, Point) = 0;
	/** Exec a single String */
	virtual bool ExecString(const char* string, bool feedback) = 0;
};

}

#endif
