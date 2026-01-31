#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include "math_core.h"

namespace nb = nanobind;

void double_values_wrapper(nb::ndarray<double, nb::ndim<1>, nb::device::cpu, nb::c_contig> arr){

    // prepare the c++ function arguments
    double* data_ptr = arr.data();
    size_t count = arr.shape(0);

    // call the actual c++ function
    double_values(data_ptr, count);

}

// Binding code
NB_MODULE(my_math, m){
    m.def("double_values", &double_values_wrapper, "Double the values in a numpy array");
}
