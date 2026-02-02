#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <Python.h>
#include <iostream>
#include <vector>

namespace nb = nanobind;

int main(){

    Py_Initialize(); // python interpreter
    
    // add current dir to sys.path (for python)
    nb::module_ sys = nb::module_::import_("sys");

    // std::string version = nb::cast<std::string>(sys.attr("version"));
    // std::string executable = nb::cast<std::string>(sys.attr("executable"));

    // std::cout << "--- Python Debug Info ---" << std::endl;
    // std::cout << "Version: " << version << std::endl;
    // std::cout << "Executable: " << executable << std::endl;
    // std::cout << "-------------------------" << std::endl;
    sys.attr("path").attr("append")(".");

    // --- THE FIX: FORCE NUMPY SYMBOLS TO BE GLOBAL ---
    // 1. Import 'os' to get the flag constants
    nb::module_ os = nb::module_::import_("os");

    // 2. Get the current flags (so we can restore them later)
    int original_flags = nb::cast<int>(sys.attr("getdlopenflags")());

    // 3. Set the new flags: RTLD_GLOBAL | RTLD_LAZY
    //    RTLD_GLOBAL = os.RTLD_GLOBAL
    int global_flag = nb::cast<int>(os.attr("RTLD_GLOBAL"));
    int lazy_flag = nb::cast<int>(os.attr("RTLD_LAZY"));

    sys.attr("setdlopenflags")(global_flag | lazy_flag);

    // 4. Import NumPy NOW (while flags are set)
    //    This loads the C-API symbols globally so Nanobind can see them.
    nb::module_::import_("numpy");

    // 5. Restore the original flags (good hygiene)
    sys.attr("setdlopenflags")(original_flags);

    std::cout << "[C++] NumPy loaded with RTLD_GLOBAL." << std::endl;

    // FORCE check NumPy version
    nb::module_ np = nb::module_::import_("numpy");
    std::cout << "NumPy Version: " << nb::cast<const char*>(nb::str(np.attr("__version__"))) << std::endl;

    try {
            // Prepare some C++ Data
            std::vector<double> test_arr = {1.0, 2.0, 3.0, -1.0, -2.0};
            
            std::cout << "[C++] Initial data: ";
            for (double v : test_arr){
                std::cout << v << " ";
            }
            std::cout << std::endl;

            // wrap C++ data into a nanobind ndarray
            size_t shape[1] = { test_arr.size() };
            std::cout << "[c++] shape: " << shape[0] << std::endl;

            // ndarray constructor: nb::ndarray<...>(ptr, ndim, shape, OWNER);
            auto view = nb::ndarray<double>(
                test_arr.data(), 
                1, 
                shape, 
                nb::handle() // doesnt inc the python reference count, avoids python freeing the data
            );
            std::cout << "[c++] Created ndarray view over C++ data" << std::endl;

            std::cout << "[c++] attempting to cast ndarray to py::object" << std::endl;
            nb::object py_obj = nb::cast(view);
            std::cout << "[c++] Successfully casted ndarray to py::object" << std::endl;


            // important to know that if we send nb::handle as the base/owner of the ndarray, we are acknowledging that
            // the C++ side is responsible for managing the memory of the data, so best to initially create the data using an
            // object that has data ownership like std::vector

            // import the python script as a module
            nb::module_ script = nb::module_::import_("embed_script");
            std::cout << "[c++] Imported python script as module" << std::endl;
            script.attr("divide_by_two")(view); // call the relu function that we wrote in python
            std::cout << "[c++] Called python function to process the data" << std::endl;
            // verify the results back in C++
            std::cout << "[c++] Processed data: ";
            for (double v : test_arr){
                std::cout << v << " ";
            }
            std::cout << std::endl;

        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        Py_Finalize();
        return 0;
    }
