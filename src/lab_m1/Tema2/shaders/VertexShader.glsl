#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;
uniform mat4 car_position;

// Output value to fragment shader
out vec3 color;


void main()
{
    // sends color output to fragment shader
    color = vec3(1);
    color = object_color;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    gl_Position.y = gl_Position.y - (length((car_position[0] - gl_Position).xyz) * length((car_position[0] - gl_Position).xyz)) * 0.03;
}
