

<!-- FS start -->

vec4 bloomcolor = bloom();
in_diffuse = in_diffuse*(1.0 - bloomcolor.a);
diffuse = in_diffuse + bloomcolor;


<!-- FS stop -->
