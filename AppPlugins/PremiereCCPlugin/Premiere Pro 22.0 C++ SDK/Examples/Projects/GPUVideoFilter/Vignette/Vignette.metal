#include <metal_math>

/*
**
*/
void Vignette_delegate(
    device float4* const destImg,
    int inDestPitch,
    int in16f,
    int inWidth,
    int inHeight,
    float    amountF,
    float    outer_aaF,
    float    outer_bbF,
    float    outer_abF,
    float    inner_aaF,
    float    inner_bbF,
    float    inner_aabbF,
    float    inner_abF,
    float    x_t,
    float    y_t,
    uint2 inXY);

/*
**
*/
typedef struct
{
    int destPitch;
    int is16f;
    int width;
    int height;
    float    amountF;
    float    outer_aaF;
    float    outer_bbF;
    float    outer_abF;
    float    inner_aaF;
    float    inner_bbF;
    float    inner_aabbF;
    float    inner_abF;
    float    x_t;
    float    y_t;
} VignetteParams;

/*
**
*/
kernel void kVignette(
    device float4* const destImg,
    device VignetteParams* inValues,
    uint2 inXY [[ thread_position_in_grid ]])
{
    Vignette_delegate(destImg,
                      inValues->destPitch,
                      inValues->is16f,
                      inValues->width,
                      inValues->height,
                      inValues->amountF,
                      inValues->outer_aaF,
                      inValues->outer_bbF,
                      inValues->outer_abF,
                      inValues->inner_aaF,
                      inValues->inner_bbF,
                      inValues->inner_aabbF,
                      inValues->inner_abF,
                      inValues->x_t,
                      inValues->y_t,
                      inXY);
}

/*
**
*/
void Vignette_delegate(
    device float4* const destImg,
    int inDestPitch,
    int in16f,
    int inWidth,
    int inHeight,
    float    amountF,
    float    outer_aaF,
    float    outer_bbF,
    float    outer_abF,
    float    inner_aaF,
    float    inner_bbF,
    float    inner_aabbF,
    float    inner_abF,
    float    x_t,
    float    y_t,
    uint2    inXY)
{
    if ( inXY.x >= (uint)inWidth || inXY.y >= (uint)inHeight )
    {
        return;
    }
    uint x = inXY.x;
    uint y = inXY.y;
    
    uint index = (y * inDestPitch) + x;
    
    // Read
    float4 dest;
    if(in16f)
    {
        half4 half_dest = ((device half4*)destImg)[index];
        dest = float4((float)half_dest.x,(float)half_dest.y,(float)half_dest.z,(float)half_dest.w);
    }
    else
    {
        dest = destImg[index];
    }
    
    // Modify
    x_t = x - x_t;
    y_t = y - y_t;
        
    float f = 0.0f;
    
    if (x_t != 0 || y_t != 0)
    {

        float xx = x_t * x_t;
        float yy = y_t * y_t;

        if (inner_aaF == outer_aaF)
        {
            f = xx*inner_bbF + yy * inner_aaF < inner_aabbF ? 0.0f : 1.0f;
        }
        else
        {
            float R = metal::sqrt(xx + yy),
                r_i = inner_abF / metal::sqrt(inner_bbF * xx + inner_aaF * yy) * R,
                r_o = outer_abF / metal::sqrt(outer_bbF * xx + outer_aaF * yy) * R;
            f = metal::min(1.0f, metal::max(0.0f, (R - r_i) / (r_o - r_i)));
        }
    }
    
    f *= amountF;
    dest.x = metal::max(0.0f, dest.x + f);
    dest.y = metal::max(0.0f, dest.y + f);
    dest.z = metal::max(0.0f, dest.z + f);
    
        
    // write
    if(in16f)
    {
        half4 write_value = half4((half)dest.x,(half)dest.y,(half)dest.z,(half)dest.w);
        ((device half4*)destImg)[index] = write_value;
    }
    else
    {
        destImg[index] = dest;
    }
}
