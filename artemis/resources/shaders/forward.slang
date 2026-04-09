struct VSInput {
  float3 pos;
  float4 model0;
  float4 model1;
  float4 model2;
  float4 model3;
};

struct VSOutput {
  float4 pos : SV_Position;
};

struct CameraBuffer {
  float4x4 vp;
};

ConstantBuffer<CameraBuffer> cb;

[shader("vertex")]
VSOutput vert_main(VSInput input) {
  VSOutput output;

  // Lazy fix:
  // The model matrices are currently sent as raw bytes,
  // which means (by the virtue of GLM) model0, model1, ...,
  // are columns and hence need to be transposed.
  // Fix this in the future!
  float4x4 model = transpose(float4x4(
    input.model0,
    input.model1,
    input.model2,
    input.model3,
  ));

  output.pos = mul(cb.vp, mul(model, float4(input.pos, 1.0)));
  return output;
}

[shader("fragment")]
float4 frag_main(VSOutput vert_in) : SV_Target {
  return float4(1.0);
}
