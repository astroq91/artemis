struct VSInput {
  float3 pos;
};

struct VSOutput {
  float4 pos : SV_Position;
};

[shader("vertex")]
VSOutput vert_main(VSInput input) {
  VSOutput output;
  output.pos = float4(input.pos, 1.0);
  return output;
}

[shader("fragment")]
float4 frag_main(VSOutput vert_in) : SV_Target {
  return float4(1.0);
}
