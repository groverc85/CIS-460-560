#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

uniform int u_Time;

in vec4 vs_Pos;
in vec4 vs_Col;

out vec4 fs_Col;

//out vec4 fs_Pos;

void main()
{
    fs_Col = vs_Col;
    vec4 modelposition = u_Model * vs_Pos;

    //built-in things to pass down the pipeline
//    fs_Pos = u_ViewProj * modelposition;
    gl_Position = u_ViewProj * modelposition;

}
