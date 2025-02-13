/*************************************************************************/
/*  cone_twist_joint_3d_sw.h                                             */
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

/*
Adapted to Fox from the Bullet library.
*/

/*
Bullet Continuous Collision Detection and Physics Library
ConeTwistJointSW is Copyright (c) 2007 Starbreeze Studios

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Written by: Marcus Hennix
*/

#ifndef CONE_TWIST_JOINT_SW_H
#define CONE_TWIST_JOINT_SW_H

#include "servers/physics_3d/joints/jacobian_entry_3d_sw.h"
#include "servers/physics_3d/joints_3d_sw.h"

///ConeTwistJointSW can be used to simulate ragdoll joints (upper arm, leg etc)
class ConeTwistJoint3DSW : public Joint3DSW {
#ifdef IN_PARALLELL_SOLVER
public:
#endif

	union {
		struct {
			Body3DSW *A;
			Body3DSW *B;
		};

		Body3DSW *_arr[2];
	};

	JacobianEntry3DSW m_jac[3]; //3 orthogonal linear constraints

	real_t m_appliedImpulse;
	Transform3D m_rbAFrame;
	Transform3D m_rbBFrame;

	real_t m_limitSoftness;
	real_t m_biasFactor;
	real_t m_relaxationFactor;

	real_t m_swingSpan1;
	real_t m_swingSpan2;
	real_t m_twistSpan;

	Vector3 m_swingAxis;
	Vector3 m_twistAxis;

	real_t m_kSwing;
	real_t m_kTwist;

	real_t m_twistLimitSign;
	real_t m_swingCorrection;
	real_t m_twistCorrection;

	real_t m_accSwingLimitImpulse;
	real_t m_accTwistLimitImpulse;

	bool m_angularOnly;
	bool m_solveTwistLimit;
	bool m_solveSwingLimit;

public:
	virtual PhysicsServer3D::JointType get_type() const override { return PhysicsServer3D::JOINT_TYPE_CONE_TWIST; }

	virtual bool setup(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	ConeTwistJoint3DSW(Body3DSW *rbA, Body3DSW *rbB, const Transform3D &rbAFrame, const Transform3D &rbBFrame);

	void setAngularOnly(bool angularOnly) {
		m_angularOnly = angularOnly;
	}

	void setLimit(real_t _swingSpan1, real_t _swingSpan2, real_t _twistSpan, real_t _softness = 0.8f, real_t _biasFactor = 0.3f, real_t _relaxationFactor = 1.0f) {
		m_swingSpan1 = _swingSpan1;
		m_swingSpan2 = _swingSpan2;
		m_twistSpan = _twistSpan;

		m_limitSoftness = _softness;
		m_biasFactor = _biasFactor;
		m_relaxationFactor = _relaxationFactor;
	}

	inline int getSolveTwistLimit() {
		return m_solveTwistLimit;
	}

	inline int getSolveSwingLimit() {
		return m_solveTwistLimit;
	}

	inline real_t getTwistLimitSign() {
		return m_twistLimitSign;
	}

	void set_param(PhysicsServer3D::ConeTwistJointParam p_param, real_t p_value);
	real_t get_param(PhysicsServer3D::ConeTwistJointParam p_param) const;
};

#endif // CONE_TWIST_JOINT_SW_H
