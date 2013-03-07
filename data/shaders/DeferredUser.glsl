
//<!-- VS start -->
void VS()
{

}
//<!-- VS stop -->

//<!-- GS start -->
void GS(int i)
{

}
//<!-- GS stop -->

//<!-- FS start -->
void FS()
{
	float intensity = abs(sin(time));
	lightmap = vec4(intensity,0,0,1);
	//diffuse = vec4(0);
	diffuse = lightmap;
}
//<!-- FS stop -->
