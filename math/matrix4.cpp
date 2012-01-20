/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 */

#include "math/matrix4.h"
#include "math/vector4d.h"

namespace Math {

Matrix<4, 4>::Matrix() :
	MatrixType<4, 4>(), Rotation3D<Matrix4>() {

}

Matrix<4, 4>::Matrix(const MatrixBase<4, 4> &m) :
	MatrixType<4, 4>(m), Rotation3D<Matrix4>() {

}

void Matrix<4, 4>::transform(Vector3d *v, bool trans) const {
	Vector4d m;
	m(0, 0) = v->x();
	m(1, 0) = v->y();
	m(2, 0) = v->z();
	m(3, 0) = (trans ? 1.f : 0.f);

	m = *this * m;

	v->set(m(0, 0), m(1, 0), m(2, 0));
}

Vector3d Matrix<4, 4>::getPosition() const {
	return Vector3d(getValue(0, 3), getValue(1, 3), getValue(2, 3));
}

void Matrix<4, 4>::setPosition(const Vector3d &v) {
	setValue(0, 3, v.x());
	setValue(1, 3, v.y());
	setValue(2, 3, v.z());
}

void Matrix<4, 4>::translate(const Vector3d &vec) {
	Vector3d v(vec);
	transform(&v, false);

	operator()(0, 3) += v.x();
	operator()(1, 3) += v.y();
	operator()(2, 3) += v.z();
}

/**
 * Inverts a matrix in place.
 *	This function avoid having to do generic Gaussian elimination on the matrix
 *	by assuming that the top-left 3x3 part of the matrix is orthonormal
 *	(columns and rows 0, 1 and 2 orthogonal and unit length).
 *	See e.g. Eric Lengyel's Mathematics for 3D Game Programming and Computer Graphics, p. 82.
 */
void Matrix<4, 4>::invertAffineOrthonormal() {
	Matrix<4, 4> m2;
	m2.setValue(0, 0, getValue(0, 0));
	m2.setValue(0, 1, getValue(1, 0));
	m2.setValue(0, 2, getValue(2, 0));
	m2.setValue(1, 0, getValue(0, 1));
	m2.setValue(1, 1, getValue(1, 1));
	m2.setValue(1, 2, getValue(2, 1));
	m2.setValue(2, 0, getValue(0, 2));
	m2.setValue(2, 1, getValue(1, 2));
	m2.setValue(2, 2, getValue(2, 2));
	m2.setValue(3, 0, 0.f);
	m2.setValue(3, 1, 0.f);
	m2.setValue(3, 2, 0.f);
	m2.setValue(3, 3, 1.f);
	m2.setPosition(Math::Vector3d(0,0,0));

	Matrix<4, 1> v;
	v.setValue(0, -getValue(0, 3));
	v.setValue(1, -getValue(1, 3));
	v.setValue(2, -getValue(2, 3));
	v.setValue(3, 0.f);

	m2.transformVector(&v);
	m2.setPosition(Math::Vector3d(v.getData()[0],v.getData()[1],v.getData()[2]));
	*this = m2;
}

void Matrix<4, 4>::setCol(int col, const Math::Vector3d &vec) {
	setValue(0, col, vec.x());
	setValue(1, col, vec.y());
	setValue(2, col, vec.z());
	setValue(3, col, col == 3 ? 1.f : 0.f);
}

void Matrix<4, 4>::setRow(int row, const Math::Vector3d &vec) {
	setValue(row, 0, vec.x());
	setValue(row, 1, vec.y());
	setValue(row, 2, vec.z());
	setValue(row, 3, row == 3 ? 1.f : 0.f);
}

/**
 * Generates a lookat matrix with position at origin. For reference, see
 * http://clb.demon.fi/MathGeoLib/docs/float3x3_LookAt.php
 */
void Matrix<4, 4>::lookAtMatrix(const Math::Vector3d &localForward, const Math::Vector3d &targetDirection,
						   const Math::Vector3d &localUp, const Math::Vector3d &worldUp) {
	Math::Vector3d localRight = Math::Vector3d::crossProduct(localUp, localForward);
	localRight.normalize();
	Math::Vector3d worldRight = Math::Vector3d::crossProduct(worldUp, targetDirection);
	worldRight.normalize();
	Math::Vector3d perpWorldUp = Math::Vector3d::crossProduct(targetDirection, worldRight);
	perpWorldUp.normalize();

	Math::Matrix4 m1;
	m1.setCol(0, worldRight);
	m1.setCol(1, perpWorldUp);
	m1.setCol(2, targetDirection);
	m1.setCol(3, Math::Vector3d(0,0,0));

	Math::Matrix4 m2;
	m2.setRow(0, localRight);
	m2.setRow(1, localUp);
	m2.setRow(2, localForward);
	m2.setRow(3, Math::Vector3d(0,0,0));

	*this = m1 * m2;
}

} // end of namespace Math

