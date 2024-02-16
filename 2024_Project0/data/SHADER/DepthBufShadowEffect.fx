float4x4 matWorld : world;			// ワールド変換行列
float4x4 matCameraView : view;		// カメラビュー変換行列
float4x4 matCameraProj : projection;	// 射影変換行列
float4x4 matLightView;				// ライトビュー変換行列
float4x4 matLightProj;				// 射影変換行列
float4   m_Ambient = 0.0f;			//色
float4 m_EyePos;                    //視点
float3 m_LightPos;                    //光視点
float3 m_LightVec;                    //光視点
sampler tex0 : register(s0);		// オブジェクトのテクスチャー
texture texShadowMap;				// シャドウマップテクスチャ
float  m_Near = 0.0f;             //フォグの開始位置
float  m_Far = 5000.0f;             //フォグの終了位置
float m_fHigh = 500.0f;
float m_fLow = 0.0f;
float m_dot;
float4 m_FogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
float4 m_FogColor2 = float4(0.4f, 0.4f, 0.34f, 1.0f);

sampler DefSampler = sampler_state	// サンプラーステート
{
    texture = (texShadowMap);
    AddressU  = CLAMP;        
    AddressV  = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};


struct VS_OUTPUT
{
   float4 Pos : POSITION;  			 // 射影変換座標
   float4 ZCalcTex : TEXCOORD1;   	// Z値算出用テクスチャ
   float2 Tex       : TEXCOORD0;
   float4 PosWVP : TEXCOORD2;
   float4 Col : COLOR0;				// ディフューズ色
};


// 頂点シェーダ
VS_OUTPUT DepthBufShadow_VS( float4 Pos : POSITION , float3 Norm : NORMAL, float2 Tex : TEXCOORD0)
{
   VS_OUTPUT Out = (VS_OUTPUT)0;

   // 普通にカメラの目線によるワールドビュー射影変換をする
   float4x4 mat;
   mat  = mul( matWorld, matCameraView );
   mat  = mul( mat, matCameraProj );
   Out.Pos = mul( Pos, mat );
   Out.Tex = Tex;
   // ライトの目線によるワールドビュー射影変換をする
   mat  = mul( matWorld, matLightView );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // 法線とライトの方向から頂点の色を決定
   //  濃くなりすぎないように調節しています
   float3 N = normalize( mul(Norm, matWorld) );
   float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
 /*  float3 L = -LightDirect.xyz;
   float3 N = normalize(Norm.xyz);*/

   Out.Col = max(m_Ambient, (dot(N, -LightDirect)*0.5f));
   Out.Col = float4(0.9f, 0.9f, 0.9f, 0.6) * (0.4 + dot(N, -LightDirect)*(1-0.4f));
   Out.Col.a = 1.0f;
   Out.PosWVP = mul(Pos, matWorld);

   return Out;
}


// ピクセルシェーダ
float4 DepthBufShadow_PS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD1 , VS_OUTPUT In) : COLOR
{
   // ライト目線によるZ値の再算出
   float ZValue = ZCalcTex.z / ZCalcTex.w;
	float d = distance(In.PosWVP.xyz, m_EyePos.xyz);
	
	float f = (m_Far - d) / (m_Far - m_Near);
	float h = In.PosWVP.y /  m_fHigh;
	h = clamp(h, 0.0f, 1.0f);
	f = clamp(f, 0.0f, 1.0f);
	if ((tex2D(tex0, In.Tex).r + tex2D(tex0, In.Tex).g + tex2D(tex0, In.Tex).b) != 0.0f)
	{
	Col = (In.Col * tex2D(tex0, In.Tex));
	}
	
   // テクスチャ座標に変換
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;

   if (TransTexCoord.x >1.0f || TransTexCoord.x < 0.0f || TransTexCoord.y >1.0f || TransTexCoord.y < 0.0f )
   {
	   Col.rgb = Col.rgb * 0.7f;
	   Col = Col * f + m_FogColor * (1.0f - f);
	   Col = Col * (f + h) + m_FogColor2 * (1.0f - (f + h));
	 
	   return Col;
   }
   float3 VtxVec = In.PosWVP.xyz - m_LightPos;
   float3 LightVec = m_LightVec;
   VtxVec = normalize(VtxVec);
   LightVec = normalize(LightVec);
   float dotProduct = dot(VtxVec, LightVec);
 
   // アークコサインを使用して角度を計算
   float angle = dotProduct;
   //= acos(dotProduct)
   angle = abs(angle);
   float4 TexCol = tex2D(DefSampler, TransTexCoord);
   // 同じ座標のZ値を抽出
   float SM_Z = (TexCol.x + (TexCol.y +(TexCol.z /256.0f)/256.0f) /256.0f);
  // color.r + (color.g + (color.b + color.a / 256.0f) / 256.0f) / 256.0f;
   // 算出点がシャドウマップのZ値よりも大きければ影と判断|| angle < 0.925f
   if( ZValue > SM_Z+0.00005f ){
     Col.rgb = Col.rgb * 0.7f;
	
    }


   Col = Col * f + m_FogColor * (1.0f - f);
   Col = Col * (f + h) + m_FogColor2 * (1.0f - (f + h));
   return Col;
}


// テクニック
technique DepthBufShadowTec
{
   pass P0
   {
      VertexShader = compile vs_3_0 DepthBufShadow_VS();
      PixelShader = compile ps_3_0 DepthBufShadow_PS();
   }
}
