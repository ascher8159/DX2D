//--WVP
matrix World, View, Projection;

//---------------------------------------------------
//-------------------구조체 정의---------------------
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
//--------------cpu -> gpu에 값을 받는곳--------------
//---------------------------------------------------
VertexOutput VS(VertexInput input)
{
	VertexInput output;
	//output.Position = input.Position; //사용 안함
    output.Position = mul(input.Position, World);			// NDC * World 
	output.Position = mul(output.Position, View);			// World * ViewSpace(Camera)
	output.Position = mul(output.Position, Projection);		// ViewSpace * ViewOption (2D에서는 Orthgraphic)
	
	output.Color = input.Color;

	return output;
}

//---------------------------------------------------
//------------------Pixel 처리 하는곳-----------------
//---------------------------------------------------
//Color 색상
float4 Color = float4(1, 0, 0, 1);
float4 PS(VertexOutput input) : SV_Target
{
    return Color;
}

//정점 색상
float4 Diffuse_PS(VertexOutput input) : SV_Target
{
    return float4(input.Color);
}

RasterizerState RS
{
    FillMode = WireFrame;
};

//---------------------------------------------------
//-------Pass 번호를 통해서 계산된 값 출력-------------
//---------------------------------------------------
// 쉐이더 결과값을 기존 c++ 에서 blob을 통해서 저장해서 사용 한걸 gpu 에서 아예 셋팅
// technique : pass 관련 관리
// pass : vs, ps 처리하는 방식

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