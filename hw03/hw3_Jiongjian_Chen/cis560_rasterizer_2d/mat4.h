//Base code written by Jan Allbeck, Chris Czyzewicz, Cory Boatright, Tiantian Liu, Benedict Brown, and Adam Mally
//University of Pennsylvania

//The mat4 class is a 4x4 matrix containing a floating point number in each cell.
//Each mat4 is column-major, meaning the [] operator should return the Nth column of the matrix.
 
#ifndef MAT4_H
#define MAT4_H

#include <iostream>
#include "vec4.h"

class mat4 {
 private:
    ///The set of vec4s that represent the columns of the matrix
	vec4 data[4];
 public:
	///----------------------------------------------------------------------
	/// Constructors
	///----------------------------------------------------------------------
	/// Default Constructor.  Initialize to identity matrix.
	mat4();

	/// Initializes the diagonal values of the matrix to diag. All other values are 0.
	mat4(float diag);

    /// Initializes matrix with each vector representing a column in the matrix
    mat4(const vec4 &col0, const vec4 &col1, const vec4 &col2, const vec4& col3);

    mat4(const mat4 &m2); // copy constructor

	///----------------------------------------------------------------------
	/// Getters
	///----------------------------------------------------------------------	
	/// Returns the values of the column at the index
    vec4  operator[](unsigned int index) const;

	/// Returns a reference to the column at the index
    vec4& operator[](unsigned int index);

	///----------------------------------------------------------------------
	/// Static Initializers
	///----------------------------------------------------------------------	
	/// Creates a 3-D rotation matrix.
	/// Takes an angle in degrees and an axis represented by its xyz components, and outputs a 4x4 rotation matrix
	/// You may choose to only handle the three cardinal axes of rotation.
	static mat4 rotate(float angle, float x, float y, float z);

	/// Takes an xyz displacement and outputs a 4x4 translation matrix
	static mat4 translate(float x, float y, float z);

	/// Takes an xyz scale and outputs a 4x4 scale matrix
	static mat4 scale(float x, float y, float z);

	/// Generates a 4x4 identity matrix
	static mat4 identity();	


	///----------------------------------------------------------------------
	/// Operator Functions
	///----------------------------------------------------------------------

    /// Assign m2 to this and return it
    mat4& operator=(const mat4 &m2);

    /// Test for equality
 	bool operator==(const mat4 &m2) const;

	/// Test for inequality
	bool operator!=(const mat4 &m2) const;

	/// Element-wise arithmetic
    /// e.g. += adds the elements of m2 to this and returns this (like regular +=)
    ///      +  returns a new matrix whose elements are the sums of this and v2
    mat4& operator+=(const mat4 &m2);
    mat4& operator-=(const mat4 &m2);
    mat4& operator*=(float c);                 // multiplication by a scalar
    mat4& operator/=(float c);                 // division by a scalar
    mat4  operator+(const mat4 &m2) const;
    mat4  operator-(const mat4 &m2) const;
    mat4  operator*(float c) const;             // multiplication by a scalar
    mat4  operator/(float c) const;             // division by a scalar

	/// Matrix multiplication (m1 * m2)
	mat4 operator*(const mat4 &m2) const;

	/// Matrix/vector multiplication (m * v)
	/// Assume v is a column vector (ie. a 4x1 matrix)
	vec4 operator*(const vec4 &v) const;
};

///----------------------------------------------------------------------
/// Matrix Operations
///----------------------------------------------------------------------	
/// Returns the transpose of the input matrix (v_ij == v_ji)
mat4 transpose(const mat4 &m);

/// Returns a row of the input matrix
vec4 row(const mat4 &m, unsigned int index);

/// Scalar multiplication (c * m)
mat4 operator*(float c, const mat4 &m);
	
/// Vector/matrix multiplication (v * m)
/// Assume v is a row vector (ie. a 1x4 matrix)
vec4 operator*(const vec4 &v, const mat4 &m);

/// Prints the matrix to a stream in a nice format
std::ostream &operator<<(std::ostream &o, const mat4 &m);

#endif /* MAT4_H */
