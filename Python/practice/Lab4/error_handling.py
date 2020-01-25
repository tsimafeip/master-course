import functools
import logging
import time
import random
import contextlib
import sys

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
logger.addHandler(logging.StreamHandler(sys.stdout))


def log_exception(err, re_raise, log_traceback):
    if log_traceback:
        logger.exception(err)
    else:
        logger.error(err)

    if re_raise:
        raise


@contextlib.contextmanager
def handle_error_context(re_raise=True, log_traceback=True, exc_type=Exception):
    try:
        yield None
    except exc_type as err:
        log_exception(err, re_raise, log_traceback)


def handle_error(re_raise=True, log_traceback=True, exc_type=Exception, attempts=1, delay=0, backoff=1):
    def wrapper(func):
        @functools.wraps(func)
        def wrapped(*args, **kwargs):
            if attempts and attempts < 1:
                logger.error("Attempts must be a positive number. Function can not be invoked.")
            else:
                remained_attempts = attempts
                real_delay = delay
                while attempts is None or remained_attempts > 0:
                    try:
                        func(*args, **kwargs)
                    except exc_type as err:
                        if remained_attempts:
                            remained_attempts -= 1

                        if remained_attempts == 0:
                            log_exception(err, re_raise, log_traceback)
                        else:
                            time.sleep(real_delay)
                            real_delay *= backoff
                    else:
                        break

        return wrapped

    return wrapper


@handle_error(re_raise=False)
def some_function_one():
    return 1 / 0


@handle_error(re_raise=False, exc_type=KeyError)
def some_function_two():
    return 1 / 0


@handle_error(re_raise=False, attempts=3, delay=0.5, backoff=2, log_traceback=False)
def some_function_three():
    if random.random() < 0.75:
        return 1 / 0


if __name__ == '__main__':
    # some_function_one()
    # print(1)
    #
    # some_function_three()
    # print(3)
    #
    # some_function_two()
    # print(2)

    with handle_error_context(re_raise=False, log_traceback=False, exc_type=ValueError):
        raise ValueError()
