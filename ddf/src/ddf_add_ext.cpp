#include <ATen/ATen.h>
#include <torch/extension.h>

#include <cmath>
#include <vector>

#ifdef WITH_CUDA
    int ddf_add_forward_cuda(
        at::Tensor features,at::Tensor channel_filter, at::Tensor spatial_filter,
        int kernel_size, int dilation, int stride, int head, at::Tensor output);

    int ddf_add_backward_cuda(
        at::Tensor top_grad, at::Tensor features,
        at::Tensor channel_filter, at::Tensor spatial_filter,
        int kernel_size, int dilation, int stride, int head,
        at::Tensor rtop_grad, at::Tensor rbottom_grad,
        at::Tensor rspatial_filter_grad, at::Tensor bottom_grad,
        at::Tensor channel_filter_grad, at::Tensor spatial_filter_grad);
#endif

int ddf_add_forward(
    at::Tensor features,at::Tensor channel_filter, at::Tensor spatial_filter,
    int kernel_size, int dilation, int stride, int head, at::Tensor output){
    if (features.device().is_cuda()){
        #ifdef WITH_CUDA
            return ddf_add_forward_cuda(
                features, channel_filter, spatial_filter,
                kernel_size, dilation, stride, head, output);
        #else
            AT_ERROR("ddf operation is not compiled with GPU support");
        #endif
    }
    AT_ERROR("ddf operation is not implemented on CPU");
}

int ddf_add_backward(
    at::Tensor top_grad, at::Tensor features,
    at::Tensor channel_filter, at::Tensor spatial_filter,
    int kernel_size, int dilation, int stride, int head,
    at::Tensor rtop_grad, at::Tensor rbottom_grad,
    at::Tensor rspatial_filter_grad, at::Tensor bottom_grad,
    at::Tensor channel_filter_grad, at::Tensor spatial_filter_grad){
    if (top_grad.device().is_cuda()){
        #ifdef WITH_CUDA
            return ddf_add_backward_cuda(
                top_grad, features, channel_filter, spatial_filter,
                kernel_size, dilation, stride, head,
                rtop_grad, rbottom_grad, rspatial_filter_grad,
                bottom_grad, channel_filter_grad, spatial_filter_grad);
        #else
            AT_ERROR("ddf operation is not compiled with GPU support");
        #endif
    }
    AT_ERROR("ddf operation is not implemented on CPU");
}

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
  m.def("forward", &ddf_add_forward, "ddf add forward");
  m.def("backward", &ddf_add_backward, "ddf add backward");
}
