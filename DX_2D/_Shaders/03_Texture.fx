//--WVP
matrix World, View, Projection;

// SRV
Texture2D TextureMap;

//---------------------------------------------------
//-------------------����ü ����----------------------
// ��semantic��� : cpu������ �������� ���� �����ִ� �뵵
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
//--------------cpu -> gpu�� ���� �޴°�--------------
//---------------------------------------------------
VertexOutput VS(VertexInput input)
{
    VertexOutput output; //���±��� �߸��Ѱ� Ÿ�� ����
	
    output.Position = mul(input.Position, World);			// NDC * World 
	output.Position = mul(output.Position, View);			// World * ViewSpace(Camera)
	output.Position = mul(output.Position, Projection);		// ViewSpace * ViewOption (2D������ Orthgraphic)
	
    output.Uv = input.Uv;

	return output;
}

//---------------------------------------------------
//------------------Pixel ó�� �ϴ°�-----------------
//---------------------------------------------------

RasterizerState RS
{
    FillMode = WireFrame;
};

BlendState Translucent
{
    //c++ D3D11_RENDER_TARGET_BLEND_DESC
    BlendEnable[0] = true;          // 2���� ���� ���� ���� (������)
    SrcBlend[0] = SRC_ALPHA;        // ���� �̹��� ���� 
    DestBlend[0] = INV_SRC_ALPHA;   // ��� �̹��� ����
    BlendOp[0] = ADD;               // ��� ��� (����, ����, ����....)

    SrcBlendAlpha[0] = ONE;         
    DestBlendAlpha[0] = ONE;        
    RenderTargetWriteMask[0] = 0x0F;
};
// �������� ���� (�ȼ��� �߰� �������� ���� ����) 
// ���� = A(����) * srcBlend(op) B(���)  * DestBlend 

BlendState AlphaToCoverage
{
    //���ļ��� (������ ������Ʈ ��� �׸���  ���İ� ����Ȱ� �� �������� �׸��� ���)
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

//MinMap ����(�ȼ� Ȯ��, ��� ó�� ���)
SamplerState LinearSm // �ε巴���� �帴
{
    Filter = MIN_MAG_MIP_LINEAR;
};

SamplerState PointSm // ���� ������ �ȼ��� �ణ Ʀ
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
//-------Pass ��ȣ�� ���ؼ� ���� �� ���-------------
//---------------------------------------------------
// ���̴� ������� ���� c++ ���� blob�� ���ؼ� �����ؼ� ��� �Ѱ� gpu ���� �ƿ� ����
// technique : pass ���� ����
// pass : vs, ps ó���ϴ� ���

technique11 T0
{
    pass P0 // ����
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

	pass P1 // �Ϲ� ����
	{
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));    

        SetBlendState(Translucent, float4(0, 0, 0, 0), 0xFF);
    }

    pass P2 // ���� ���� ���İ� ó��
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));

        SetBlendState(AlphaToCoverage, float4(0, 0, 0, 0), 0xFF);
    }

    pass P3 // Sampler state ó��
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Filter()));

        SetBlendState(Translucent, float4(0, 0, 0, 0), 0xFF);
    }
}