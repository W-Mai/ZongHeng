//
// ZongHeng - Main umbrella header
//
// This is the new modular entry point for ZongHeng library.
// Include this file instead of the old Qin.h
//

#ifndef ZONGHENG_H
#define ZONGHENG_H

// Core components
#include "core/ZongHengBase.h"

// Node types
#include "nodes/Yi.h"
#include "nodes/Qin.h"

// Operations
#include "operations/Operators.h"
#include "operations/Combinators.h"

// Utilities
#include "QinUtils.h"

// Clean up macros
#undef FORWARD_CONSTRAINT

#endif // ZONGHENG_H
