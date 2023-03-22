//--WVP
matrix World, View, Projection;

// SRV
Texture2D TextureMap;

//---------------------------------------------------
//-------------------구조체 정의----------------------
// ※semantic기능 : cpu영역의 변수값을 연결 시켜주는 용도
//---------------------------------------------------
struct VertexInput
{
	float4 Position : Position;
	float2 Uv : Uv;
};

struct VertexOutput
{
	float4 Position : SV_Position;
    float2 Uv : Uv;
};

//---------------------------------------------------
//--------------cpu -> gpu에 값을 받는곳--------------
//---------------------------------------------------
VertexOutput VS(VertexInput input)
{
    VertexOutput output; //여태까지 잘못한거 타입 수정
	
    output.Position = mul(input.Position, World);			// NDC * World 
	output.Position = mul(output.Position, View);			// World * ViewSpace(Camera)
	output.Position = mul(output.Position, Projection);		// ViewSpace * ViewOption (2D에서는 Orthgraphic)
	
    output.Uv = input.Uv;

	return output;
}

//---------------------------------------------------
//------------------Pixel 처리 하는곳-----------------
//---------------------------------------------------

RasterizerState RS
{
    FillMode = WireFrame;
};

BlendState Translucent
{
    //c++ D3D11_RENDER_TARGET_BLEND_DESC
    BlendEnable[0] = true;          // 2개의 색상 섞는 여부 (반투명)
    SrcBlend[0] = SRC_ALPHA;        // 원본 이미지 색상 
    DestBlend[0] = INV_SRC_ALPHA;   // 대상 이미지 색상
    BlendOp[0] = ADD;               // 계산 방식 (덧셈, 뺄셈, 곱셈....)

    SrcBlendAlpha[0] = ONE;         
    DestBlendAlpha[0] = ONE;        
    RenderTargetWriteMask[0] = 0x0F;
};
// 선형보간 수식 (픽셀간 중간 색상으로 섞는 공식) 
// 알파 = A(원본) * srcBlend(op) B(대상)  * DestBlend 

BlendState AlphaToCoverage
{
    //알파소팅 (불투명 오브젝트 모두 그리고  알파가 적용된걸 맨 마지막에 그리는 기법)
    AlphaToCoverageEnable = true;
    BlendEnable[0] = true;
    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = INV_SRC_ALPHA;
    BlendOp[0] = ADD;

    SrcBlendAlpha[0] = ONE;
    DestBlendAlpha[0] = ONE;
    RenderTargetWriteMask[0] = 0x0F;
};

SamplerState sample;
float4 PS(VertexOutput input) : SV_Target
{
    return TextureMap.Sample(sample, input.Uv);
}

//MinMap 관련(픽셀 확대, 축소 처리 방법)
SamplerState LinearSm // 부드럽지면 흐릿
{
    Filter = MIN_MAG_MIP_LINEAR;
};

SamplerState PointSm // 선명 하지만 픽셀이 약간 튐
{
    Filter = MIN_MAG_MIP_POINT;
};

uint select = 0;
float4 PS_Filter(VertexOutput input) : SV_Target
{
    if(select == 0)
        return TextureMap.Sample(LinearSm, input.Uv);
    else
        return TextureMap.Sample(PointSm, input.Uv);
}


//---------------------------------------------------
//-------Pass 번호를 통해서 계산된 값 출력-------------
//---------------------------------------------------
// 쉐이더 결과값을 기존 c++ 에서 blob을 통해서 저장해서 사용 한걸 gpu 에서 아예 셋팅
// technique : pass 관련 관리
// pass : vs, ps 처리하는 방식

technique11 T0
{
    pass P0 // 원본
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

	pass P1 // 일반 투명도
	{
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));    

        SetBlendState(Translucent, float4(0, 0, 0, 0), 0xFF);
    }

    pass P2 // 따로 따로 알파값 처리
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));

        SetBlendState(AlphaToCoverage, float4(0, 0, 0, 0), 0xFF);
    }

    pass P3 // Sampler state 처리
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Filter()));

        SetBlendState(Translucent, float4(0, 0, 0, 0), 0xFF);
    }
}