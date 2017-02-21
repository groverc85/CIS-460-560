#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

uniform int u_Time;

//in vec4 fs_Pos;

in vec4 fs_Col;
out vec4 out_Col;

void main()
{
    // Copy the color; there is no shading.
//    vec4 color = vec4(sin(u_Time * fs_Pos.xyz), 1);
    out_Col = fs_Col;
//    out_Col = color;
}
