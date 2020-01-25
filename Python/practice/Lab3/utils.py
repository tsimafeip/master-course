import timeit
import time
import contextlib


class timer():
    def __init__(self):
        self.start_time = timeit.default_timer()

    def __enter__(self):
        pass

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.end_time = timeit.default_timer()
        print('Inside timer: {}'.format(self.end_time - self.start_time))

# @contextlib.contextmanager
# def timer():
#     start_time = timeit.default_timer()
#     try:
#         yield
#     finally:
#         print("Execution time: {0}".format(timeit.default_timer() - start_time))


def profile(f):
    def wrapper(*args, **kwargs):
        start_time = timeit.default_timer()
        res = f()
        exec_time = timeit.default_timer() - start_time
        print(exec_time)
        return res
    return wrapper


@profile
def some_function():
    time.sleep(1)
    return sum(range(100000))


if __name__ == '__main__':
    result = some_function()  # return a value and print execution time
    print(result)
    with timer():
        time.sleep(1)
        print(sum(range(100000)))

