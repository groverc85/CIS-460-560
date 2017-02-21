#version 150
// ^ Change this to version 130 if you have compatibility issues

// This is a fragment shader. If you've opened this file first, please
// open and read lambert.vert.glsl before reading on.
// Unlike the vertex shader, the fragment shader actually does compute
// the shading of geometry. For every pixel in your program's output
// screen, the fragment shader is run for every bit of geometry that
// particular pixel overlaps. By implicitly interpolating the position
// data passed into the fragment shader by the vertex shader, the fragment shader
// can compute what color to apply to its pixel based on things like vertex
// position, light position, and vertex color.

uniform vec4 u_Color; // The color with which to render this instance of geometry.

// These are the interpolated values out of the rasterizer, so you can't know
// their specific values without knowing the vertices that contributed to them
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec4 fs_Col;
//in vec4 uv;

out vec4 out_Col; // This is the final output color that you will see on your
                  // screen for the pixel that is currently being processed.


void main()
{

//    out_Col = normalize(abs(fs_Nor));
                                      // Color each fragment using the interpolated surface normal's XYZ as RGB
                                      // You must add code to this main function that sets out_Col to the
                                      // Lambertian-shaded color of the geometry instead.
                                      // If your rendered images remain unchanged after altering out_Col,
                                      // try cleaning and rebuilding your project, then running qmake
                                      // (under the Build taskbar)
    float scalar = dot(abs(normalize(fs_Nor)), abs(normalize(fs_LightVec)));
    out_Col = scalar * u_Color;

}
