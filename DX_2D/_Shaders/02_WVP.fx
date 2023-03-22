//--WVP
matrix World, View, Projection;

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
	//output.Position = input.Position; //��� ����
    output.Position = mul(input.Position, World);			// NDC * World 
	output.Position = mul(output.Position, View);			// World * ViewSpace(Camera)
	output.Position = mul(output.Position, Projection);		// ViewSpace * ViewOption (2D������ Orthgraphic)
	
	output.Color = input.Color;

	return output;
}

//---------------------------------------------------
//------------------Pixel ó�� �ϴ°�-----------------
//---------------------------------------------------
//Color ����
float4 Color = float4(1, 0, 0, 1);
float4 PS(VertexOutput input) : SV_Target
{
    return Color;
}

//���� ����
float4 Diffuse_PS(VertexOutput input) : SV_Target
{
    return float4(input.Color);
}

RasterizerState RS
{
    FillMode = WireFrame;
};

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

	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, Diffuse_PS()));
    }
}