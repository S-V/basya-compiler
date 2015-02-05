#ifndef __C4_COMMON_HEADER_SHARED_H__
#define __C4_COMMON_HEADER_SHARED_H__

//------------------------------

#include <assert.h>
//#include <memory.h>
#include <string>
#include <memory>
//#include <stdio.h>
#include <stdarg.h>

#include "defines.h"
#include "debug.h"
#include "typedefs.h"

#include "Stack.h"
#include "List.h"
#include "LinkedList.h"
#include "String.h"
#include "Hash.h"

// common interfaces
#include "ICommon.h"
#include "ICompiler.h"
#include "ITreeVisitor.h"

#include "ui.h"

// main source pool
#include "source/Token.h"
#include "source/Lexer.h"
#include "source/File.h"
#include "source/Object.h"

// ast
#include "source/ast/AST_Node.h"
#include "source/ast/Identifier.h"
#include "source/ast/Types.h"
#include "source/ast/Expression.h"
#include "source/ast/Declaration.h"
#include "source/ast/Statement.h"
#include "source/ast/Unit.h"

// main source pool
#include "source/BasicTypes.h"
#include "source/Parser.h"
//#include "source/Compiler.h"

// semantics
#include "source/Symbol.h"

// backend
//#include "common/x86_codegen.h"
//#include "source/backend/ICodeGenerator.h"
#include "source/backend/Math.h"

#endif // !__C4_COMMON_HEADER_SHARED_H__