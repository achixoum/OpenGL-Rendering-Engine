#version 330 core
out vec4 FragColor;
uniform vec4 ourColor2;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightingSource;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    //ambient lighting
    float ambientStrengh = 0.3f;
    vec3 ambient = ambientStrengh * lightingSource;

    //diffuse lighting
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);
    float impact = max(dot(normal, lightDirection),0.0f);
    vec3 diffuse = impact*lightingSource;

    //specular lighting
    float speculatStregth = 0.6f;
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float impact2 = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);
    vec3 specular = impact2 * lightingSource * speculatStregth;

    vec3 result = ambient + diffuse + specular;
    //FragColor = vec4(1.0f,1.0f,0.0f,1.0f);
    FragColor = mix(texture(texture2, TexCoord), texture(texture1, TexCoord), 1.0f) * vec4(result,1.0f);
}