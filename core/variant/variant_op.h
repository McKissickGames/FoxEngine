/*************************************************************************/
/*  variant_op.h                                                         */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Fox Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef VARIANT_OP_H
#define VARIANT_OP_H

#include "variant.h"

#include "core/core_string_names.h"
#include "core/debugger/engine_debugger.h"
#include "core/object/class_db.h"

template <class R, class A, class B>
class OperatorEvaluatorAdd {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a + b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) + *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) + PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorSub {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a - b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) - *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) - PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorMul {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a * b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) * *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) * PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorXForm {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a.xform(b);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = VariantGetInternalPtr<A>::get_ptr(left)->xform(*VariantGetInternalPtr<B>::get_ptr(right));
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left).xform(PtrToArg<B>::convert(right)), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorXFormInv {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = b.xform_inv(a);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = VariantGetInternalPtr<B>::get_ptr(right)->xform_inv(*VariantGetInternalPtr<A>::get_ptr(left));
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<B>::convert(right).xform_inv(PtrToArg<A>::convert(left)), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorDiv {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a / b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) / *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) / PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorDivNZ {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		if (b == 0) {
			r_valid = false;
			*r_ret = "Division by zero error";
			return;
		}
		*r_ret = a / b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) / *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) / PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorMod {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a % b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) % *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) % PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorModNZ {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		if (b == 0) {
			r_valid = false;
			*r_ret = "Module by zero error";
			return;
		}
		*r_ret = a % b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) % *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) % PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A>
class OperatorEvaluatorNeg {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		*r_ret = -a;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = -*VariantGetInternalPtr<A>::get_ptr(left);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(-PtrToArg<A>::convert(left), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A>
class OperatorEvaluatorPos {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		*r_ret = a;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorShiftLeft {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);

#if defined(DEBUG_ENABLED)
		if (b < 0 || a < 0) {
			*r_ret = "Invalid operands for bit shifting. Only positive operands are supported.";
			r_valid = false;
			return;
		}
#endif
		*r_ret = a << b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) << *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) << PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorShiftRight {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);

#if defined(DEBUG_ENABLED)
		if (b < 0 || a < 0) {
			*r_ret = "Invalid operands for bit shifting. Only positive operands are supported.";
			r_valid = false;
			return;
		}
#endif
		*r_ret = a >> b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) >> *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) >> PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorBitOr {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a | b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) | *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) | PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorBitAnd {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a & b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) & *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) & PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A, class B>
class OperatorEvaluatorBitXor {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a ^ b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) ^ *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(PtrToArg<A>::convert(left) ^ PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class R, class A>
class OperatorEvaluatorBitNeg {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		*r_ret = ~a;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<R>::get_ptr(r_ret) = ~*VariantGetInternalPtr<A>::get_ptr(left);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<R>::encode(~PtrToArg<A>::convert(left), r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<R>::VARIANT_TYPE; }
};

template <class A, class B>
class OperatorEvaluatorEqual {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a == b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) == *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<A>::convert(left) == PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorEqualObject {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Object *a = p_left.get_validated_object();
		const Object *b = p_right.get_validated_object();
		*r_ret = a == b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const Object *a = left->get_validated_object();
		const Object *b = right->get_validated_object();
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = a == b;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Object *>::convert(left) == PtrToArg<Object *>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorEqualObjectNil {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Object *a = p_left.get_validated_object();
		*r_ret = a == nullptr;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const Object *a = left->get_validated_object();
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = a == nullptr;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Object *>::convert(left) == nullptr, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorEqualNilObject {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Object *b = p_right.get_validated_object();
		*r_ret = nullptr == b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const Object *b = right->get_validated_object();
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = nullptr == b;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(nullptr == PtrToArg<Object *>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A, class B>
class OperatorEvaluatorNotEqual {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a != b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) != *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<A>::convert(left) != PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorNotEqualObject {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		Object *a = p_left.get_validated_object();
		Object *b = p_right.get_validated_object();
		*r_ret = a != b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		Object *a = left->get_validated_object();
		Object *b = right->get_validated_object();
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = a != b;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Object *>::convert(left) != PtrToArg<Object *>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorNotEqualObjectNil {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		Object *a = p_left.get_validated_object();
		*r_ret = a != nullptr;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		Object *a = left->get_validated_object();
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = a != nullptr;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Object *>::convert(left) != nullptr, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorNotEqualNilObject {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		Object *b = p_right.get_validated_object();
		*r_ret = nullptr != b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		Object *b = right->get_validated_object();
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = nullptr != b;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(nullptr != PtrToArg<Object *>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A, class B>
class OperatorEvaluatorLess {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a < b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) < *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<A>::convert(left) < PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A, class B>
class OperatorEvaluatorLessEqual {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a <= b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) <= *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<A>::convert(left) <= PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A, class B>
class OperatorEvaluatorGreater {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a > b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) > *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<A>::convert(left) > PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A, class B>
class OperatorEvaluatorGreaterEqual {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a >= b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) >= *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<A>::convert(left) >= PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A, class B>
class OperatorEvaluatorAnd {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a && b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) && *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<A>::convert(left) && PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A, class B>
class OperatorEvaluatorOr {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = a || b;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = *VariantGetInternalPtr<A>::get_ptr(left) || *VariantGetInternalPtr<B>::get_ptr(right);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<A>::convert(left) || PtrToArg<B>::convert(right), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

#define XOR_OP(m_a, m_b) (((m_a) || (m_b)) && !((m_a) && (m_b)))
template <class A, class B>
class OperatorEvaluatorXor {
public:
	_FORCE_INLINE_ static bool xor_op(const A &a, const B &b) {
		return ((a) || (b)) && !((a) && (b));
	}
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);
		*r_ret = xor_op(a, b);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = xor_op(*VariantGetInternalPtr<A>::get_ptr(left), *VariantGetInternalPtr<B>::get_ptr(right));
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(xor_op(PtrToArg<A>::convert(left), PtrToArg<B>::convert(right)), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A>
class OperatorEvaluatorNot {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		*r_ret = !a;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = !*VariantGetInternalPtr<A>::get_ptr(left);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(!PtrToArg<A>::convert(left));
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

//// CUSTOM ////

class OperatorEvaluatorAddArray {
public:
	_FORCE_INLINE_ static void _add_arrays(Array &sum, const Array &array_a, const Array &array_b) {
		int asize = array_a.size();
		int bsize = array_b.size();
		sum.resize(asize + bsize);
		for (int i = 0; i < asize; i++) {
			sum[i] = array_a[i];
		}
		for (int i = 0; i < bsize; i++) {
			sum[i + asize] = array_b[i];
		}
	}
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Array &array_a = *VariantGetInternalPtr<Array>::get_ptr(&p_left);
		const Array &array_b = *VariantGetInternalPtr<Array>::get_ptr(&p_right);
		Array sum;
		_add_arrays(sum, array_a, array_b);
		*r_ret = sum;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		_add_arrays(*VariantGetInternalPtr<Array>::get_ptr(r_ret), *VariantGetInternalPtr<Array>::get_ptr(left), *VariantGetInternalPtr<Array>::get_ptr(right));
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		Array ret;
		_add_arrays(ret, PtrToArg<Array>::convert(left), PtrToArg<Array>::convert(right));
		PtrToArg<Array>::encode(ret, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::ARRAY; }
};

template <class T>
class OperatorEvaluatorAppendArray {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Vector<T> &array_a = *VariantGetInternalPtr<Vector<T>>::get_ptr(&p_left);
		const Vector<T> &array_b = *VariantGetInternalPtr<Vector<T>>::get_ptr(&p_right);
		Vector<T> sum = array_a;
		sum.append_array(array_b);
		*r_ret = sum;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<Vector<T>>::get_ptr(r_ret) = *VariantGetInternalPtr<Vector<T>>::get_ptr(left);
		VariantGetInternalPtr<Vector<T>>::get_ptr(r_ret)->append_array(*VariantGetInternalPtr<Vector<T>>::get_ptr(right));
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		Vector<T> sum = PtrToArg<Vector<T>>::convert(left);
		sum.append_array(PtrToArg<Vector<T>>::convert(right));
		PtrToArg<Vector<T>>::encode(sum, r_ret);
	}
	static Variant::Type get_return_type() { return GetTypeInfo<Vector<T>>::VARIANT_TYPE; }
};

class OperatorEvaluatorStringModNil {
public:
	_FORCE_INLINE_ static String do_mod(const String &s, bool *r_valid) {
		Array values;
		values.push_back(Variant());

		String a = s.sprintf(values, r_valid);
		if (r_valid) {
			*r_valid = !*r_valid;
		}
		return a;
	}
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const String &a = *VariantGetInternalPtr<String>::get_ptr(&p_left);
		*r_ret = do_mod(a, &r_valid);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<String>::get_ptr(r_ret) = do_mod(*VariantGetInternalPtr<String>::get_ptr(left), nullptr);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<String>::encode(do_mod(PtrToArg<String>::convert(left), nullptr), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::STRING; }
};

class OperatorEvaluatorStringModArray {
public:
	_FORCE_INLINE_ static String do_mod(const String &s, const Array &p_values, bool *r_valid) {
		String a = s.sprintf(p_values, r_valid);
		if (r_valid) {
			*r_valid = !*r_valid;
		}
		return a;
	}
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const String &a = *VariantGetInternalPtr<String>::get_ptr(&p_left);
		*r_ret = do_mod(a, *VariantGetInternalPtr<Array>::get_ptr(&p_right), &r_valid);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<String>::get_ptr(r_ret) = do_mod(*VariantGetInternalPtr<String>::get_ptr(left), *VariantGetInternalPtr<Array>::get_ptr(right), nullptr);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<String>::encode(do_mod(PtrToArg<String>::convert(left), PtrToArg<Array>::convert(right), nullptr), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::STRING; }
};

class OperatorEvaluatorStringModObject {
public:
	_FORCE_INLINE_ static String do_mod(const String &s, const Object *p_object, bool *r_valid) {
		Array values;
		values.push_back(p_object);
		String a = s.sprintf(values, r_valid);
		if (r_valid) {
			*r_valid = !*r_valid;
		}

		return a;
	}
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const String &a = *VariantGetInternalPtr<String>::get_ptr(&p_left);
		*r_ret = do_mod(a, p_right.get_validated_object(), &r_valid);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<String>::get_ptr(r_ret) = do_mod(*VariantGetInternalPtr<String>::get_ptr(left), right->get_validated_object(), nullptr);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<String>::encode(do_mod(PtrToArg<String>::convert(left), PtrToArg<Object *>::convert(right), nullptr), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::STRING; }
};

template <class T>
class OperatorEvaluatorStringModT {
public:
	_FORCE_INLINE_ static String do_mod(const String &s, const T &p_value, bool *r_valid) {
		Array values;
		values.push_back(p_value);
		String a = s.sprintf(values, r_valid);
		if (r_valid) {
			*r_valid = !*r_valid;
		}
		return a;
	}
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const String &a = *VariantGetInternalPtr<String>::get_ptr(&p_left);
		*r_ret = do_mod(a, *VariantGetInternalPtr<T>::get_ptr(&p_right), &r_valid);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<String>::get_ptr(r_ret) = do_mod(*VariantGetInternalPtr<String>::get_ptr(left), *VariantGetInternalPtr<T>::get_ptr(right), nullptr);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<String>::encode(do_mod(PtrToArg<String>::convert(left), PtrToArg<T>::convert(right), nullptr), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::STRING; }
};

template <Variant::Operator op, Variant::Type type_left, Variant::Type type_right>
class OperatorEvaluatorAlwaysTrue {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		*r_ret = true;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = true;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(true, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <Variant::Operator op, Variant::Type type_left, Variant::Type type_right>
class OperatorEvaluatorAlwaysFalse {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		*r_ret = false;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = false;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(false, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

///// OR ///////

_FORCE_INLINE_ static bool _operate_or(bool p_left, bool p_right) {
	return p_left || p_right;
}

_FORCE_INLINE_ static bool _operate_and(bool p_left, bool p_right) {
	return p_left && p_right;
}

_FORCE_INLINE_ static bool _operate_xor(bool p_left, bool p_right) {
	return (p_left || p_right) && !(p_left && p_right);
}

_FORCE_INLINE_ static bool _operate_get_nil(const Variant *p_ptr) {
	return p_ptr->get_validated_object() != nullptr;
}

_FORCE_INLINE_ static bool _operate_get_bool(const Variant *p_ptr) {
	return *VariantGetInternalPtr<bool>::get_ptr(p_ptr);
}

_FORCE_INLINE_ static bool _operate_get_int(const Variant *p_ptr) {
	return *VariantGetInternalPtr<int64_t>::get_ptr(p_ptr) != 0;
}

_FORCE_INLINE_ static bool _operate_get_float(const Variant *p_ptr) {
	return *VariantGetInternalPtr<double>::get_ptr(p_ptr) != 0.0;
}

_FORCE_INLINE_ static bool _operate_get_object(const Variant *p_ptr) {
	return p_ptr->get_validated_object() != nullptr;
}

_FORCE_INLINE_ static bool _operate_get_ptr_nil(const void *p_ptr) {
	return false;
}

_FORCE_INLINE_ static bool _operate_get_ptr_bool(const void *p_ptr) {
	return PtrToArg<bool>::convert(p_ptr);
}

_FORCE_INLINE_ static bool _operate_get_ptr_int(const void *p_ptr) {
	return PtrToArg<int64_t>::convert(p_ptr) != 0;
}

_FORCE_INLINE_ static bool _operate_get_ptr_float(const void *p_ptr) {
	return PtrToArg<double>::convert(p_ptr) != 0.0;
}

_FORCE_INLINE_ static bool _operate_get_ptr_object(const void *p_ptr) {
	return PtrToArg<Object *>::convert(p_ptr) != nullptr;
}

#define OP_EVALUATOR(m_class_name, m_left, m_right, m_op)                                                                    \
	class m_class_name {                                                                                                     \
	public:                                                                                                                  \
		static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {                 \
			*r_ret = m_op(_operate_get_##m_left(&p_left), _operate_get_##m_right(&p_right));                                 \
			r_valid = true;                                                                                                  \
		}                                                                                                                    \
                                                                                                                             \
		static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {                   \
			*VariantGetInternalPtr<bool>::get_ptr(r_ret) = m_op(_operate_get_##m_left(left), _operate_get_##m_right(right)); \
		}                                                                                                                    \
                                                                                                                             \
		static void ptr_evaluate(const void *left, const void *right, void *r_ret) {                                         \
			PtrToArg<bool>::encode(m_op(_operate_get_ptr_##m_left(left), _operate_get_ptr_##m_right(right)), r_ret);         \
		}                                                                                                                    \
                                                                                                                             \
		static Variant::Type get_return_type() {                                                                             \
			return Variant::BOOL;                                                                                            \
		}                                                                                                                    \
	};

// OR

// nil
OP_EVALUATOR(OperatorEvaluatorNilXBoolOr, nil, bool, _operate_or)
OP_EVALUATOR(OperatorEvaluatorBoolXNilOr, bool, nil, _operate_or)

OP_EVALUATOR(OperatorEvaluatorNilXIntOr, nil, int, _operate_or)
OP_EVALUATOR(OperatorEvaluatorIntXNilOr, int, nil, _operate_or)

OP_EVALUATOR(OperatorEvaluatorNilXFloatOr, nil, float, _operate_or)
OP_EVALUATOR(OperatorEvaluatorFloatXNilOr, float, nil, _operate_or)

OP_EVALUATOR(OperatorEvaluatorObjectXNilOr, object, nil, _operate_or)
OP_EVALUATOR(OperatorEvaluatorNilXObjectOr, nil, object, _operate_or)

// bool
OP_EVALUATOR(OperatorEvaluatorBoolXBoolOr, bool, bool, _operate_or)

OP_EVALUATOR(OperatorEvaluatorBoolXIntOr, bool, int, _operate_or)
OP_EVALUATOR(OperatorEvaluatorIntXBoolOr, int, bool, _operate_or)

OP_EVALUATOR(OperatorEvaluatorBoolXFloatOr, bool, float, _operate_or)
OP_EVALUATOR(OperatorEvaluatorFloatXBoolOr, float, bool, _operate_or)

OP_EVALUATOR(OperatorEvaluatorBoolXObjectOr, bool, object, _operate_or)
OP_EVALUATOR(OperatorEvaluatorObjectXBoolOr, object, bool, _operate_or)

// int
OP_EVALUATOR(OperatorEvaluatorIntXIntOr, int, int, _operate_or)

OP_EVALUATOR(OperatorEvaluatorIntXFloatOr, int, float, _operate_or)
OP_EVALUATOR(OperatorEvaluatorFloatXIntOr, float, int, _operate_or)

OP_EVALUATOR(OperatorEvaluatorIntXObjectOr, int, object, _operate_or)
OP_EVALUATOR(OperatorEvaluatorObjectXIntOr, object, int, _operate_or)

// float
OP_EVALUATOR(OperatorEvaluatorFloatXFloatOr, float, float, _operate_or)

OP_EVALUATOR(OperatorEvaluatorFloatXObjectOr, float, object, _operate_or)
OP_EVALUATOR(OperatorEvaluatorObjectXFloatOr, object, float, _operate_or)

// object
OP_EVALUATOR(OperatorEvaluatorObjectXObjectOr, object, object, _operate_or)

// AND

// nil
OP_EVALUATOR(OperatorEvaluatorNilXBoolAnd, nil, bool, _operate_and)
OP_EVALUATOR(OperatorEvaluatorBoolXNilAnd, bool, nil, _operate_and)

OP_EVALUATOR(OperatorEvaluatorNilXIntAnd, nil, int, _operate_and)
OP_EVALUATOR(OperatorEvaluatorIntXNilAnd, int, nil, _operate_and)

OP_EVALUATOR(OperatorEvaluatorNilXFloatAnd, nil, float, _operate_and)
OP_EVALUATOR(OperatorEvaluatorFloatXNilAnd, float, nil, _operate_and)

OP_EVALUATOR(OperatorEvaluatorObjectXNilAnd, object, nil, _operate_and)
OP_EVALUATOR(OperatorEvaluatorNilXObjectAnd, nil, object, _operate_and)

// bool
OP_EVALUATOR(OperatorEvaluatorBoolXBoolAnd, bool, bool, _operate_and)

OP_EVALUATOR(OperatorEvaluatorBoolXIntAnd, bool, int, _operate_and)
OP_EVALUATOR(OperatorEvaluatorIntXBoolAnd, int, bool, _operate_and)

OP_EVALUATOR(OperatorEvaluatorBoolXFloatAnd, bool, float, _operate_and)
OP_EVALUATOR(OperatorEvaluatorFloatXBoolAnd, float, bool, _operate_and)

OP_EVALUATOR(OperatorEvaluatorBoolXObjectAnd, bool, object, _operate_and)
OP_EVALUATOR(OperatorEvaluatorObjectXBoolAnd, object, bool, _operate_and)

// int
OP_EVALUATOR(OperatorEvaluatorIntXIntAnd, int, int, _operate_and)

OP_EVALUATOR(OperatorEvaluatorIntXFloatAnd, int, float, _operate_and)
OP_EVALUATOR(OperatorEvaluatorFloatXIntAnd, float, int, _operate_and)

OP_EVALUATOR(OperatorEvaluatorIntXObjectAnd, int, object, _operate_and)
OP_EVALUATOR(OperatorEvaluatorObjectXIntAnd, object, int, _operate_and)

// float
OP_EVALUATOR(OperatorEvaluatorFloatXFloatAnd, float, float, _operate_and)

OP_EVALUATOR(OperatorEvaluatorFloatXObjectAnd, float, object, _operate_and)
OP_EVALUATOR(OperatorEvaluatorObjectXFloatAnd, object, float, _operate_and)

// object
OP_EVALUATOR(OperatorEvaluatorObjectXObjectAnd, object, object, _operate_and)

// XOR

// nil
OP_EVALUATOR(OperatorEvaluatorNilXBoolXor, nil, bool, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorBoolXNilXor, bool, nil, _operate_xor)

OP_EVALUATOR(OperatorEvaluatorNilXIntXor, nil, int, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorIntXNilXor, int, nil, _operate_xor)

OP_EVALUATOR(OperatorEvaluatorNilXFloatXor, nil, float, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorFloatXNilXor, float, nil, _operate_xor)

OP_EVALUATOR(OperatorEvaluatorObjectXNilXor, object, nil, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorNilXObjectXor, nil, object, _operate_xor)

// bool
OP_EVALUATOR(OperatorEvaluatorBoolXBoolXor, bool, bool, _operate_xor)

OP_EVALUATOR(OperatorEvaluatorBoolXIntXor, bool, int, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorIntXBoolXor, int, bool, _operate_xor)

OP_EVALUATOR(OperatorEvaluatorBoolXFloatXor, bool, float, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorFloatXBoolXor, float, bool, _operate_xor)

OP_EVALUATOR(OperatorEvaluatorBoolXObjectXor, bool, object, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorObjectXBoolXor, object, bool, _operate_xor)

// int
OP_EVALUATOR(OperatorEvaluatorIntXIntXor, int, int, _operate_xor)

OP_EVALUATOR(OperatorEvaluatorIntXFloatXor, int, float, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorFloatXIntXor, float, int, _operate_xor)

OP_EVALUATOR(OperatorEvaluatorIntXObjectXor, int, object, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorObjectXIntXor, object, int, _operate_xor)

// float
OP_EVALUATOR(OperatorEvaluatorFloatXFloatXor, float, float, _operate_xor)

OP_EVALUATOR(OperatorEvaluatorFloatXObjectXor, float, object, _operate_xor)
OP_EVALUATOR(OperatorEvaluatorObjectXFloatXor, object, float, _operate_xor)

// object
OP_EVALUATOR(OperatorEvaluatorObjectXObjectXor, object, object, _operate_xor)

class OperatorEvaluatorNotBool {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		*r_ret = !*VariantGetInternalPtr<bool>::get_ptr(&p_left);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = !*VariantGetInternalPtr<bool>::get_ptr(left);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(!PtrToArg<bool>::convert(left), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorNotInt {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		*r_ret = !*VariantGetInternalPtr<int64_t>::get_ptr(&p_left);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = !*VariantGetInternalPtr<int64_t>::get_ptr(left);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(!PtrToArg<int64_t>::convert(left), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorNotFloat {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		*r_ret = !*VariantGetInternalPtr<double>::get_ptr(&p_left);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = !*VariantGetInternalPtr<double>::get_ptr(left);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(!PtrToArg<double>::convert(left), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorNotObject {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		*r_ret = p_left.get_validated_object() == nullptr;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = left->get_validated_object() == nullptr;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Object *>::convert(left) == nullptr, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

////

class OperatorEvaluatorInStringFind {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const String &str_a = *VariantGetInternalPtr<String>::get_ptr(&p_left);
		const String &str_b = *VariantGetInternalPtr<String>::get_ptr(&p_right);

		*r_ret = str_b.find(str_a) != -1;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const String &str_a = *VariantGetInternalPtr<String>::get_ptr(left);
		const String &str_b = *VariantGetInternalPtr<String>::get_ptr(right);
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = str_b.find(str_a) != -1;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<String>::convert(right).find(PtrToArg<String>::convert(left)) != -1, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A, class B>
class OperatorEvaluatorInArrayFind {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(&p_right);

		*r_ret = b.find(a) != -1;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const A &a = *VariantGetInternalPtr<A>::get_ptr(left);
		const B &b = *VariantGetInternalPtr<B>::get_ptr(right);
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = b.find(a) != -1;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<B>::convert(right).find(PtrToArg<A>::convert(left)) != -1, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorInArrayFindNil {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Array &b = *VariantGetInternalPtr<Array>::get_ptr(&p_right);
		*r_ret = b.find(Variant()) != -1;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const Array &b = *VariantGetInternalPtr<Array>::get_ptr(right);
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = b.find(Variant()) != -1;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Array>::convert(right).find(Variant()) != -1, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorInArrayFindObject {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Array &b = *VariantGetInternalPtr<Array>::get_ptr(&p_right);
		*r_ret = b.find(p_left) != -1;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const Array &b = *VariantGetInternalPtr<Array>::get_ptr(right);
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = b.find(*left) != -1;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Array>::convert(right).find(PtrToArg<Object *>::convert(left)) != -1, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

template <class A>
class OperatorEvaluatorInDictionaryHas {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Dictionary &b = *VariantGetInternalPtr<Dictionary>::get_ptr(&p_right);
		const A &a = *VariantGetInternalPtr<A>::get_ptr(&p_left);

		*r_ret = b.has(a);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const Dictionary &b = *VariantGetInternalPtr<Dictionary>::get_ptr(right);
		const A &a = *VariantGetInternalPtr<A>::get_ptr(left);
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = b.has(a);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Dictionary>::convert(right).has(PtrToArg<A>::convert(left)), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorInDictionaryHasNil {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Dictionary &b = *VariantGetInternalPtr<Dictionary>::get_ptr(&p_right);

		*r_ret = b.has(Variant());
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const Dictionary &b = *VariantGetInternalPtr<Dictionary>::get_ptr(right);
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = b.has(Variant());
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Dictionary>::convert(right).has(Variant()), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorInDictionaryHasObject {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		const Dictionary &b = *VariantGetInternalPtr<Dictionary>::get_ptr(&p_right);

		*r_ret = b.has(p_left);
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		const Dictionary &b = *VariantGetInternalPtr<Dictionary>::get_ptr(right);
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = b.has(*left);
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		PtrToArg<bool>::encode(PtrToArg<Dictionary>::convert(right).has(PtrToArg<Object *>::convert(left)), r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorObjectHasPropertyString {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		Object *b = p_right.get_validated_object();
		if (!b) {
			*r_ret = "Invalid base object for 'in'";
			r_valid = false;
			return;
		}

		const String &a = *VariantGetInternalPtr<String>::get_ptr(&p_left);

		bool exist;
		b->get(a, &exist);
		*r_ret = exist;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		Object *l = right->get_validated_object();
		ERR_FAIL_COND(l == nullptr);
		const String &a = *VariantGetInternalPtr<String>::get_ptr(left);

		bool valid;
		l->get(a, &valid);
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = valid;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		bool valid;
		PtrToArg<Object *>::convert(right)->get(PtrToArg<String>::convert(left), &valid);
		PtrToArg<bool>::encode(valid, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

class OperatorEvaluatorObjectHasPropertyStringName {
public:
	static void evaluate(const Variant &p_left, const Variant &p_right, Variant *r_ret, bool &r_valid) {
		Object *b = p_right.get_validated_object();
		if (!b) {
			*r_ret = "Invalid base object for 'in'";
			r_valid = false;
			return;
		}

		const StringName &a = *VariantGetInternalPtr<StringName>::get_ptr(&p_left);

		bool exist;
		b->get(a, &exist);
		*r_ret = exist;
		r_valid = true;
	}
	static inline void validated_evaluate(const Variant *left, const Variant *right, Variant *r_ret) {
		Object *l = right->get_validated_object();
		ERR_FAIL_COND(l == nullptr);
		const StringName &a = *VariantGetInternalPtr<StringName>::get_ptr(left);

		bool valid;
		l->get(a, &valid);
		*VariantGetInternalPtr<bool>::get_ptr(r_ret) = valid;
	}
	static void ptr_evaluate(const void *left, const void *right, void *r_ret) {
		bool valid;
		PtrToArg<Object *>::convert(right)->get(PtrToArg<StringName>::convert(left), &valid);
		PtrToArg<bool>::encode(valid, r_ret);
	}
	static Variant::Type get_return_type() { return Variant::BOOL; }
};

#endif // VARIANT_OP_H
