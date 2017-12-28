
// TODO: Add OpenCL kernel code here.


kernel void process_buffer(
	global const float4*    restrict p_src, // src RGBA buffer
	global float4*          restrict p_dst, // dst RGBA buffer
	const int2              size,           // size of both input pictures
	const int               dst_pitch)      // distance between line in pixels for dst buffer
{
	int x = (int)get_global_id(0);
	int y = (int)get_global_id(1);
	// make gamma correction out=power(inp,1/4);
	p_dst[y*dst_pitch + x] = sqrt(sqrt(p_src[y*size.x + x]));
}

const sampler_t smp = CLK_FILTER_LINEAR | CLK_ADDRESS_CLAMP | CLK_NORMALIZED_COORDS_FALSE;
kernel void process_image(
	global float3*			src,            // src RGB buffer
	global float4*          restrict p_dst, // dst RGBA buffer
	global float3*			p_dst_info,		// info of each p
	const int				width,
	const int               height,           // size of both image
	)              // vector for affine transform
{
	int x = (int)get_global_id(0);
	int y = (int)get_global_id(1);
	float2 p = (float2)(x, y);
	float2 s;
	s.x = M.x*p.x + M.y*p.y + T.x;
	s.y = M.z*p.x + M.w*p.y + T.y;
	p_dst[y*size.x + x] = read_imagef(src, smp, s);
}
