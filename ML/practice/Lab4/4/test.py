import numpy as np
import imageio
import scipy
import scipy.misc
import sklearn
import sys
import os


def read_file(fname):
    image = imageio.imread(fname)
    print(type(image))
    assert image.shape == (26, 40, 3)
    print(type(image))
    kek = image[:, :, 0].astype(np.int)
    print(type(kek))
    return kek


print(read_file(r'C:\Users\lybot\OneDrive\Документы\Магистратура\Машинное обучение\practice\Lab4\4\train\cars\0a77ee5f4a74d7e546a1856a5ebe62fc.bmp'))
kek = [1, 2,3,4,5]
print(np.cumsum(kek))