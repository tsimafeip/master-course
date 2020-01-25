from abc import abstractmethod
import functools


class BoundedMeta(type):

    def __new__(mcs, name, bases, attrs, **kwargs):
        return super().__new__(mcs, name, bases, attrs)

    def __init__(cls, name, bases, attrs, **kwargs):
        cls._cur_count = 0
        cls._max_instance_count = kwargs.get('max_instance_count', 1)
        super().__init__(name, bases, attrs)

    @property
    def max_instance_count(cls):
        return cls._max_instance_count

    @property
    def cur_count(cls):
        return cls._cur_count

    @cur_count.setter
    def cur_count(cls, count):
        assert count >= cls._cur_count
        cls._cur_count = count

    def __call__(cls):
        cls.cur_count += 1
        if cls.cur_count > cls.max_instance_count:
            raise TypeError
        return super().__call__()


class C(metaclass=BoundedMeta, max_instance_count=2):
    pass


class BoundedBase:
    __instance_count = 0

    def __new__(cls, *args, **kwargs):
        BoundedBase.__instance_count += 1
        max_count = cls.get_max_instance_count()
        if max_count and BoundedBase.__instance_count > max_count:
            raise TypeError

    @abstractmethod
    def get_max_instance_count(self):
        pass


class D(BoundedBase):
    @classmethod
    def get_max_instance_count(cls):
        return 1


def call_counter(func):
    @functools.wraps(func)
    def wrapper():
        wrapper.calls += 1
        return wrapper.calls

    wrapper.calls = 0
    return wrapper


@call_counter
def f():
    pass


if __name__ == '__main__':
    c1 = C()
    c2 = C()
    try:
        c3 = C()
    except TypeError:
        print("everything works fine!")
    else:
        print("something goes wrong!")

    d1 = D()
    try:
        d2 = D()
    except TypeError:
        print("everything works fine #2!")
    else:
        print("something goes wrong #2!")

    for real_call_count in range(1, 5):
        assert f() == real_call_count
