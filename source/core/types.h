/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#define NENE_STR_IMPL(x) #x
#define NENE_STR(x) NENE_STR_IMPL(x)

#define NENE_CAT_IMPL(a, b) a##b
#define NENE_CAT(a, b) NENE_CAT_IMPL(a, b)

#include "windll.h"
#include "template/lambda.h"
#include "template/pointer.h"
#include "template/container.h"
#include "template/fundamental.h"
#include "template/utility.h"
#include "template/trait.h"
