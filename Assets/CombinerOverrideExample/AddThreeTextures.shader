Shader "AddThreeTextures"
{
	Properties
	{
		_TextureA ("Texture A (RGBA)", 2D) = "black" {}
		_TextureB ("Texture B (RGBA)", 2D) = "black" {}
		_TextureC ("Texture C (RGBA)", 2D) = "black" {}
	}

	CGINCLUDE

	struct Vertex
	{
		float4 pos : POSITION;
		float2 uv_TexA : TEXCOORD0;
		float2 uv_TexB : TEXCOORD1;
		float2 uv_TexC : TEXCOORD2;
	};

	ENDCG

	// This shader is used by the editor.
	SubShader
	{
		Pass
		{
			CGPROGRAM

			#pragma vertex vert
			#pragma fragment frag

			uniform sampler2D _TextureA;
			uniform sampler2D _TextureB;
			uniform sampler2D _TextureC;

			Vertex vert(Vertex i)
			{
				Vertex o;
				o.pos = UnityObjectToClipPos(i.pos);
				o.uv_TexA = i.uv_TexA;
				o.uv_TexB = i.uv_TexB;
				o.uv_TexC = i.uv_TexC;
				return o;
			}

			float4 frag(Vertex i) : COLOR
			 {
				float4 colourA = tex2D(_TextureA, i.uv_TexA);
				float4 colourB = tex2D(_TextureB, i.uv_TexA);
				float4 colourC = tex2D(_TextureC, i.uv_TexA);

				return colourA + colourB + colourC;
			}

			ENDCG
		}
	}

	// This is a dummy shader; used only to declare the texture references.
	// The actual texture combiner is supplied in "TextureCombinerOverrides.cpp"
	// Be careful not to create a trivial shader which would get optimized to remove some of the texture references.
	SubShader
	{
		Pass
		{
			SetTexture [_TextureA]
			{
				combine texture
			}
			SetTexture [_TextureB]
			{
				combine texture lerp (texture) previous
			}
			SetTexture [_TextureC]
			{
				combine texture lerp (texture) previous
			}
		}
	}

	Fallback "Diffuse"
}
