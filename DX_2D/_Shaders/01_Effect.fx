//---------------------------------------------------
//-------------------����ü ����---------------------
//---------------------------------------------------
struct VertexInput
{
	float4 Position : Position;
	float4 Color : Color;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float4 Color : Color;
};

//---------------------------------------------------
//--------------cpu -> gpu�� ���� �޴°�--------------
//---------------------------------------------------
VertexOutput VS(VertexInput input)
{
	VertexInput output;
	output.Position = input.Position;
	output.Color = input.Color;

	return output;
}

//---------------------------------------------------
//------------------Pixel ó�� �ϴ°�-----------------
//---------------------------------------------------
float4 Color = float4(1, 0, 0, 1);
float4 PS(VertexOutput input) : SV_Target
{
	//return Color;
	return input.Color;
}

//---------------------------------------------------
//-------Pass ��ȣ�� ���ؼ� ���� �� ���-------------
//---------------------------------------------------
// ���̴� ������� ���� c++ ���� blob�� ���ؼ� �����ؼ� ��� �Ѱ� gpu ���� �ƿ� ����
// technique : pass ���� ����
// pass : vs, ps ó���ϴ� ���
technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}