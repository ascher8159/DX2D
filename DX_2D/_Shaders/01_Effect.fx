//---------------------------------------------------
//-------------------구조체 정의---------------------
//---------------------------------------------------
struct VertexInput
{
	float4 Position : Position;
	float3 Color : Color;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float3 Color : Color;
};

//---------------------------------------------------
//--------------cpu -> gpu에 값을 받는곳--------------
//---------------------------------------------------
VertexOutput VS(VertexInput input)
{
	VertexInput output;
	output.Position = input.Position;
	output.Color = input.Color;

	return output;
}

//---------------------------------------------------
//------------------Pixel 처리 하는곳-----------------
//---------------------------------------------------
//float4 Color = float4(1, 0, 0, 1);
float4 PS(VertexOutput input) : SV_Target
{
	//return Color;
	return float4(input.Color, 1);
}

//---------------------------------------------------
//-------Pass 번호를 통해서 계산된 값 출력-------------
//---------------------------------------------------
technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}