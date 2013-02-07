
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
	float intensity = abs(sin(3.14/2*applicationTime));
	diffuse = vec4(intensity,1,0,1);
}
//<!-- FS stop -->
