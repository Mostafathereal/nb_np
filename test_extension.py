import numpy as np

## the c++ extension
import my_math


def test_double_values(arr):
    my_math.double_values(arr)

# test array
arr = np.array([1.0, 2.0, 3.0, 4.0], dtype=np.float64)
arr_copy = arr.copy()
print("array before test", arr)
test_double_values(arr)
print("array after test", arr)

if np.all(arr == 2 * arr_copy):
    print("test passed")
else:
    print("test failed")

