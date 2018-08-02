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
in vec4 fs_Pos;

out vec4 out_Col; // This is the final output color that you will see on your
                  // screen for the pixel that is currently being processed.

void main()
{
    // Material base color (before shading)
        vec4 red = vec4(1,0,0,0);
        vec4 yellow = vec4(.85,.85,0,0);
        vec4 green = vec4(.5,.85,.15,0);
        vec4 diffuseColor = vec4(1,1,1,0);
        float height = clamp(1.f / fs_Pos.y, 0, 1);

        float step1 = 0.0;
        float step2 = 0.1;
        float step3 = 0.16;

        diffuseColor = mix(red, yellow, smoothstep(step1, step2, height));
        diffuseColor = mix(diffuseColor, green, smoothstep(step2, step3, height));

        // Calculate the diffuse term for Lambert shading
        float diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));
        // Avoid negative lighting values        
        diffuseTerm = clamp(diffuseTerm, 0, 1);

        float ambientTerm = 0.2;

        float lightIntensity = diffuseTerm + ambientTerm;   //Add a small float value to the color multiplier
                                                            //to simulate ambient lighting. This ensures that faces that are not
                                                            //lit by our point light are not completely black.

        vec4 shadowCol = vec4(.45, .2, .5, 0);
        diffuseColor = mix(shadowCol, diffuseColor, diffuseTerm);

        // Compute final shaded color
        //out_Col = vec4(diffuseColor.rgb * lightIntensity, diffuseColor.a);
        out_Col = abs(fs_Nor);
}
