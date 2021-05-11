#version 300 es
precision highp float;
precision highp int;
in vec4 fColor;
in vec2 fTexCoords;
in float fTexId;
in float fType;

uniform sampler2D uTextures[8];

out vec4 color;

void main () {
    //color = vec4( 0.6, 1.0, 1.0, 1.0 );
    //color = fColor;
    //color = texture(uTex, fTexCoords);
    //color = fColor * texture(uTextures[1], fTexCoords);
    vec4 tempColor = vec4(1.0,1.0,1.0,1.0);

    if (fTexId == 0.0f) {
        color = fColor;
    }
    else if (fTexId == 1.0f) {
        tempColor = texture(uTextures[1], fTexCoords);
        //tempColor = vec4(vec3(1.0 - texture(uTextures[1], fTexCoords)), 1.0);
    }
    else if (fTexId == 2.0f) {
        tempColor = texture(uTextures[2], fTexCoords);
        //tempColor = vec4(vec3(1.0 - texture(uTextures[1], fTexCoords)), 1.0);
    }
    else if (fTexId == 3.0f) {
        tempColor = texture(uTextures[3], fTexCoords);
    }

    else if (fTexId == 4.0f) {
        tempColor = texture(uTextures[4], fTexCoords);
    }
    else if (fTexId == 5.0f) {
        tempColor = texture(uTextures[5], fTexCoords);
    }

    else if (fTexId == 6.0f) {
        tempColor = texture(uTextures[6], fTexCoords);
    }
    else if (fTexId == 7.0f) {
        tempColor = texture(uTextures[7], fTexCoords);
    }

    if (fType == 0.0f){
        color = fColor * tempColor;
        //color.rgb = (fColor.rgb) + (tempColor.rgb * (1.0f - fColor.a)) ;
        //color.rgb = (tempColor.rgb) + (fColor.rgb * (1.0f - tempColor.a)) ;
    }
    //font
    else if (fType == 1.0f){
        color = fColor * tempColor.a;
    }
}