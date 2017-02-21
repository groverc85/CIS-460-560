Q1: 
In mat4.h, there are several declarations of the operator* function. One declaration is vec4 operator*(const vec4 &v) const (line 89), while another is vec4 operator*(const vec4 &v, const mat4 &m) (line 106). Explain the mathematical difference in the output of these functions. Additionally, explain how these functions differ in scope from a code standpoint.

A1:
Line “vec4 operator*(const vec4 &v) const” is a matrix/vector multiplication form that takes vector v and post multiply matrix m, while line “vec4 operator*(const vec4 &v, const mat4 &m);” is a vector/matrix multiplication form that uses vector to pre-multiply the matrix, in this case v is a row vector. Given m is a m*n matrix and v is a n*1 column vector(first case) or 1*m row vector(second case), in the first case the result will be a m*1 matrix while in the second case it’s a 1*n matrix.

Difference from a code standpoint:
The result of Line “vec4 operator*(const vec4 &v) const” is a column vector while result of line “vec4 operator*(const vec4 &v, const mat4 &m);” is a row vector, therefore there would be some storage difference during coding.

Q2: 
In vec4.h, there are two declarations of the operator[] function. Describe a situation in which only the first function (line 25) can be used, and describe a situation in which only the second function (line 28) can be used.

A2:
“float operator[](unsigned int index) const;” const modifies the function itself, which means the function is read-only, it will not modify any object within scope. Therefore, if we don’t want index variable be modified within the function, we can only use this line of code.

The return type of “float& operator[](unsigned int index);” is a reference, so it can only be assign to a reference variable type float &.
