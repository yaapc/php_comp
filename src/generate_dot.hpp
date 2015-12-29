#pragma once
#ifndef GENERATE_DOT_H
#define GENERATE_DOT_H

#include <iostream>
#include "SymbolTable/Scope.h"

void generate_dot(Scope *, std::ostream &os);

#endif